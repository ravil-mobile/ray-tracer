#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <Geometry/Primitives.h>
#include <cstddef>
#include <cstdint>
#include <vector>


struct Colorable {
  explicit Colorable(const ColorT& color) : color(color) {}
  static ColorT scale(ColorT baseColor, float factor) {
    ColorT updatedColor{};

    #pragma unroll
    for (int i = 0; i < 3; ++i) {
      float channel = (factor * static_cast<float>(baseColor[i]));
      channel = (channel > 255) ? 255 : channel;
      channel = (channel < 0) ? 0 : channel;

      updatedColor[i] = static_cast<uint8_t>(channel);
    }

    return updatedColor;
  }
  ColorT color{};
};


struct Object {
  virtual ~Object() = default;
  explicit Object(VectorT point) : center(std::move(point)) {}
  Object(const Object& other) = default;
  VectorT center{0.0, 0.0, 0.0};
};


struct Light : public Object, public Colorable {
  explicit Light(VectorT point, const ColorT& color) : Object(std::move(point)), Colorable(color) {}
  Light(const Light& light) = default;
};


struct Sphere : public Object, public Colorable {
  explicit Sphere(VectorT center, float radius, const ColorT& color)
    : Object(std::move(center)), Colorable(color), radius(radius) {}
  Sphere(const Sphere& other) = default;
  float radius{};
};


struct Plane : public Object, public Colorable {
  explicit Plane(VectorT center, VectorT normal, float width, float height, const ColorT& color)
    : Object(std::move(center)),
      Colorable(color),
      normal(std::move(normal)),
      width(width),
      height(height) {}
  Plane(const Plane& other) = default;
  VectorT normal{0.0, 0.0, 0.0};
  float width{};
  float height{};
};


struct Camera : public Object {
  explicit Camera(VectorT center, VectorT normal, VectorT upDirection, ScreenT screen)
    : Object(std::move(center)),
      screenNormal(std::move(normal)),
      upDirection(std::move(upDirection)),
      screen(screen) {}
  Camera(const Camera& other) = default;
  VectorT screenNormal{0.0, 0.0, -1.0};
  VectorT upDirection{0.0, 1.0, 0.0};
  ScreenT screen{};
};


class Scenario {
public:
  explicit Scenario(const Camera& camera,
                    const ColorT& color,
                    ImageViewT imageView)
                      : camera(camera), backgroundColor(color), imageView(imageView) {}
  Scenario(const Scenario& screen) = default;
  void addLight(const Light& light) { lights.push_back(light); }
  void addSphere(const Sphere& sphere) { spheres.push_back(sphere); }

  auto getImageView() const { return imageView; }
  const auto& getCamera() const { return camera; }
  const auto& getLights() const { return lights; }
  const auto& getSpheres() const { return spheres; }
  auto getBackgroundColor() const { return backgroundColor; }

private:
  Camera camera;
  std::vector<Light> lights{};
  std::vector<Sphere> spheres{};
  ColorT backgroundColor{};
  ImageViewT imageView{};
};

#endif // GEOMETRY_H_
