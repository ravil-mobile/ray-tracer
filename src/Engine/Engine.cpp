#include "Engine/Engine.h"
#include <Eigen/Geometry>
#include <iostream>


void Engine::findLinearMap() {
  const auto& camera = scenario.getCamera();
  VectorT normal = camera.screenNormal;
  VectorT upVector = camera.upDirection;

  normal.normalize();
  upVector.normalize();
  VectorT rightVector = normal.cross(upVector);

  cameraProjection.col(0) = rightVector;
  cameraProjection.col(1) = upVector;
  cameraProjection.col(2) = normal;
  cameraProjection.col(3) = camera.screenNormal;
}


void Engine::castRay() {}

void Engine::run() {
  this->findLinearMap();

  const auto& view = canvas.getImageView();

  auto background =  scenario.getBackgroundColor();
  for(size_t y = 0; y < view.height; ++y) {
    for (size_t x = 0; x < view.width; ++x) {
      canvas.at(x, y) = background;
    }
  }

  ColorT otherColor(0, 255, 255);
  for(size_t y = 50; y < 150; ++y) {
    for (size_t x = 50; x < 150; ++x) {
      canvas.at(x, y) = otherColor;
    }
  }
}