#include "Engine/Scenario.h"
#include <iostream>


void Scenario::addPlane(VectorT center,
                        VectorT normal,
                        VectorT heightDirection,
                        float width,
                        float height,
                        const ColorT& color,
                        float shininess) {

  normal.normalize();
  heightDirection.normalize();
  if (std::abs(normal.dot(heightDirection)) < 1e-5) {
    VectorT widthDirection = heightDirection.cross(normal);
    heightDirection = widthDirection.cross(normal);

    auto p1 = center + 0.5 * (-width * widthDirection + height * heightDirection);
    auto p2 = center + 0.5 * (width * widthDirection + height * heightDirection);
    auto p3 = center + 0.5 * (-width * widthDirection - height * heightDirection);
    auto p4 = center + 0.5 * (width * widthDirection - height * heightDirection);

    planes.push_back(Plane(Facet(p1, p2, p3), Facet(p2, p4, p3), color, shininess));
  }
  else {
    throw std::runtime_error("[plane] normal and heightDirection a collinear");
  }
}