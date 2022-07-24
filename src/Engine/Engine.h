#ifndef ENGINE_H_
#define ENGINE_H_

#include "Geometry/Geometry.h"
#include <vector>

class Canvas {
public:
  explicit Canvas(ImageViewT view) : view(view) {
    canvas.resize(view.width * view.height);
  }

  Canvas(const Canvas& other) = default;

  ColorT& at(size_t i, size_t j) {
    return canvas.at(i + j * view.width);
  }

  ColorT at(size_t i, size_t j) const {
    return canvas.at(i + j * view.width);
  }

  const ImageViewT& getImageView() const {
    return view;
  }

private:
  ImageViewT view;
  std::vector<ColorT> canvas{};
};


class Engine {
public:
  explicit Engine(Scenario& scenario) : scenario(scenario), canvas(scenario.getImageView()) {}
  void run();
  const Canvas& getCanvas() { return canvas; }

  using AffineMatrixT = Eigen::Matrix<float, 3, 4>;
private:
  void computeLinearMap();
  VectorT castRay(float x, float y);

  const Scenario& scenario;
  Canvas canvas;
  AffineMatrixT cameraProjection{};
};

#endif // ENGINE_H_