#ifndef SCENARIO_H_
#define SCENARIO_H_


#include "Geometry/Geometry.h"


class Scenario {
public:
  explicit Scenario(const Camera& camera,
                    const ColorT& color,
                    ImageViewT imageView)
      : camera(camera), backgroundColor(color), imageView(imageView) {}
  Scenario(const Scenario& screen) = default;
  void addLight(const Light& light) { lights.push_back(light); }
  void addSphere(const Sphere& sphere) { spheres.push_back(sphere); }
  void addPlane(VectorT center,
                VectorT normal,
                VectorT hightDirection,
                float width,
                float height,
                const ColorT& color,
                float shininess);

  auto getImageView() const { return imageView; }
  const auto& getCamera() const { return camera; }
  const auto& getLights() const { return lights; }
  const auto& getSpheres() const { return spheres; }
  const auto& getPlanes() const { return planes; }
  auto getBackgroundColor() const { return backgroundColor; }

private:
  Camera camera;
  std::vector<Light> lights{};
  std::vector<Sphere> spheres{};
  std::vector<Plane> planes{};
  ColorT backgroundColor{};
  ImageViewT imageView{};
};

#endif // SCENARIO_H_
