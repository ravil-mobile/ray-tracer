#include "Engine/Engine.h"
#include "Geometry/Geometry.h"
#include <Eigen/Geometry>
#include <limits>
#include <array>
#include <iostream>


void Engine::computeLinearMap() {
  const auto& camera = scenario.getCamera();
  VectorT normal = camera.screenNormal;
  VectorT upVector = camera.upDirection;

  normal.normalize();
  upVector.normalize();
  VectorT rightVector = normal.cross(upVector);

  Eigen::Matrix<float, 3, 3> matrix{};
  matrix.col(0) = rightVector;
  matrix.col(1) = upVector;
  matrix.col(2) = normal;

  auto inverse = matrix.inverse();

  cameraProjection.col(0) = inverse.col(0);
  cameraProjection.col(1) = inverse.col(1);
  cameraProjection.col(2) = inverse.col(2);
  cameraProjection.col(3) = camera.screenNormal;
}


VectorT Engine::castRay(float x, float y) {
  Eigen::Vector4f vector{x, y, 0.0, 1.0};
  return (cameraProjection * vector).normalized();
}

enum HitObjectType {
  SphereObject,
  FacetObject,
  NoneObject
};

void Engine::run() {
  this->computeLinearMap();

  const auto& view = canvas.getImageView();
  const auto& camera = scenario.getCamera();
  const auto& lights = scenario.getLights();

  float xBegin = -0.5 * camera.screen.width;
  float yBegin = 0.5 * camera.screen.height;
  float dx = camera.screen.width / static_cast<float>(view.width - 1);
  float dy = camera.screen.height / static_cast<float>(view.height - 1);

  const auto& spheres = scenario.getSpheres();
  const auto& planes = scenario.getPlanes();
  auto backgroundColor =  scenario.getBackgroundColor();

  #pragma omp parallel for collapse(2) schedule(dynamic)
  for(size_t j = 0; j < view.height; ++j) {
    for (size_t i = 0; i < view.width; ++i) {
      VectorT ray = castRay(xBegin + static_cast<float>(i) * dx,
                            yBegin - static_cast<float>(j) * dy);

      HitObjectType hitObjectType{HitObjectType::NoneObject};

      auto closestHitDistance = std::numeric_limits<float>::max();
      VectorT hitVector{0.0, 0.0, 0.0};
      Sphere *closestSphere{nullptr};

      // iterate over all spheres
      for (auto sphere = spheres.begin(); sphere != spheres.end(); ++sphere) {
        float distance = sphere->center.norm();
        float projectedDistance = ray.dot(sphere->center);
        float deviation = distance * distance - projectedDistance * projectedDistance;
        float radius = sphere->radius * sphere->radius;

        if (radius > deviation) {
          float hitDistance = projectedDistance - std::sqrt(radius - deviation);
          if (hitDistance < closestHitDistance) {
            hitObjectType = HitObjectType::SphereObject;
            closestHitDistance = hitDistance;
            closestSphere = const_cast<Sphere*>(&(*sphere));
            hitVector = hitDistance * ray;
          }
        }
      }


      // iterate over all planes
      Facet* closestFacet{nullptr};
      for (auto plane = planes.begin(); plane != planes.end(); ++plane) {
        std::array<const Facet*, 2> facets{&(plane->facet1), &(plane->facet2)};
        for (auto* facet: facets) {
          VectorT distanceProjectedToFaceNormal = facet->normal.dot(facet->center) * facet->normal;

          float cosAlpha = ray.dot(-1.0 * facet->normal) + 1e-12;
          if (cosAlpha < 0.0) {
            // the hit point is going to be opposite to the ray i.e., skipping
            continue;
          }

          float hitDistance = (distanceProjectedToFaceNormal.norm()) / cosAlpha;
          VectorT planeHitVector = ray * hitDistance;

          float mainArea = 0.5 * (facet->p2 - facet->p1).cross(facet->p3 - facet->p1).norm();
          float areaSum = 0.5 * ((facet->p2 - facet->p1).cross(planeHitVector - facet->p1)).norm();
          areaSum += 0.5 * ((planeHitVector - facet->p1).cross(facet->p3 - facet->p1)).norm();
          areaSum += 0.5 * ((planeHitVector - facet->p2).cross(facet->p3 - facet->p2)).norm();

          if (std::abs(mainArea - areaSum) < 1e-3) {
            if (hitDistance < closestHitDistance) {
              hitObjectType = HitObjectType::FacetObject;
              closestHitDistance = hitDistance;
              closestFacet = const_cast<Facet*>(facet);
              hitVector = planeHitVector;
            }
          }
        }
      }

      // TODO: make refactoring
      VectorT viewVector = -1.0 * ray.normalized();
      if (hitObjectType == HitObjectType::SphereObject) {
        float lambertIntensity{0.0};
        float specularIntensity{0.0};
        VectorT surfaceNormal = (hitVector - closestSphere->center).normalized();
        for (const auto& light: lights) {
          VectorT lightVector = (light.center - hitVector).normalized();
          lambertIntensity += lightVector.dot(surfaceNormal);

          VectorT reflectionVector = 2.0 * lightVector.dot(surfaceNormal) * surfaceNormal - lightVector;

          specularIntensity += std::pow(reflectionVector.dot(viewVector), closestSphere->shininess);
        }
        canvas.at(i, j) = Colorable::scale(closestSphere->color, lambertIntensity + specularIntensity);
      }
      else if (hitObjectType == HitObjectType::FacetObject) {
        float lambertIntensity{0.0};
        float specularIntensity{0.0};
        VectorT surfaceNormal = closestFacet->normal;
        for (const auto& light: lights) {
          VectorT lightVector = (light.center - hitVector).normalized();
          lambertIntensity += lightVector.dot(surfaceNormal);

          VectorT reflectionVector = 2.0 * lightVector.dot(surfaceNormal) * surfaceNormal - lightVector;

          specularIntensity += std::pow(reflectionVector.dot(viewVector), closestFacet->shininess);
        }
        canvas.at(i, j) = Colorable::scale(closestFacet->color, lambertIntensity + specularIntensity);
      }
      else {
        canvas.at(i, j) = backgroundColor;
      }
    }
  }
}