#include "Engine/Engine.h"
#include <Eigen/Geometry>
#include <limits>
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
  return cameraProjection * vector;
}

void Engine::run() {
  this->computeLinearMap();

  const auto& view = canvas.getImageView();
  const auto& camera = scenario.getCamera();
  const auto& light = scenario.getLights()[0];

  float xBegin = -0.5 * camera.screen.width;
  float yBegin = 0.5 * camera.screen.height;
  float dx = camera.screen.width / static_cast<float>(view.width - 1);
  float dy = camera.screen.height / static_cast<float>(view.height - 1);

  auto backgroundColor =  scenario.getBackgroundColor();
  for(size_t j = 0; j < view.height; ++j) {
    for (size_t i = 0; i < view.width; ++i) {
      auto ray = castRay(xBegin + static_cast<float>(i) * dx,
                         yBegin - static_cast<float>(j) * dy);

      ray.normalize();


      const auto& spheres = scenario.getSpheres();
      auto closestHitDistance = std::numeric_limits<float>::max();
      VectorT hitVector{0.0, 0.0, 0.0};
      auto closestSphere = spheres.end();

      for (auto sphere = spheres.begin(); sphere != spheres.end(); ++sphere) {
        float distance = sphere->center.norm();
        float projectedDistance = ray.dot(sphere->center);
        float deviation = distance * distance - projectedDistance * projectedDistance;
        float radius = sphere->radius * sphere->radius;

        if (radius > deviation) {
          float hitDistance = projectedDistance - std::sqrt(radius - deviation);
          if (hitDistance < closestHitDistance) {
            closestHitDistance = hitDistance;
            closestSphere = sphere;
            hitVector = hitDistance * ray;
          }
        }
      }

      if (closestSphere != spheres.end()) {
        VectorT surfaceNormal = (hitVector - closestSphere->center).normalized();
        VectorT lightVector = (light.center - hitVector).normalized();
        float intensity = lightVector.dot(surfaceNormal);
        canvas.at(i, j) = Colorable::scale(closestSphere->color, intensity);
      }
      else {
        canvas.at(i, j) = backgroundColor;
      }
    }
  }
}