#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <Geometry/Primitives.h>
#include <cstddef>
#include <cstdint>
#include <vector>


struct Colorable {
  explicit Colorable(const ColorT& color, float shininess) : color(color), shininess(shininess) {}
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
  float shininess{20.0};
};


struct Object {
  virtual ~Object() = default;
  explicit Object(VectorT point) : center(std::move(point)) {}
  Object(const Object& other) = default;
  VectorT center{0.0, 0.0, 0.0};
};


struct Light : public Object {
  explicit Light(VectorT point) : Object(std::move(point)) {}
  Light(const Light& light) = default;
};


struct Sphere : public Object, public Colorable {
  explicit Sphere(VectorT center, float radius, const ColorT& color, float shininess)
    : Object(std::move(center)), Colorable(color, shininess), radius(radius) {}
  Sphere(const Sphere& other) = default;
  float radius{};
};


struct Facet : public Object, public Colorable {
  Facet(const Facet& other) = default;
  explicit Facet(const VectorT& p1,
                 const VectorT& p2,
                 const VectorT& p3,
                 ColorT color = {255, 255, 255},
                 float shininess = 10.0)
                  : Object((p1 + p2 + p3) / 3.0),
                    Colorable(color, shininess),
                    p1(p1), p2(p2), p3(p3) {
    VectorT side1 = p2 - p1;
    VectorT side2 = p3 - p1;
    normal = side1.cross(side2).normalized();
  }

  VectorT p1{0.0, 0.0, 0.0};
  VectorT p2{0.0, 0.0, 0.0};
  VectorT p3{0.0, 0.0, 0.0};
  VectorT normal{0.0, 0.0, 0.0};
};


struct Plane : public Object, public Colorable {
  explicit Plane(const Facet& facet1, const Facet& facet2, const ColorT& color, float shininess)
    : Object((facet1.center + facet2.center) / 2.0),
      Colorable(color, shininess),
      facet1(facet1),
      facet2(facet2) {
    this->facet1.color = color;
    this->facet1.shininess = shininess;


    this->facet2.color = color;
    this->facet2.shininess = shininess;
  }
  Plane(const Plane& other) = default;

  Facet facet1;
  Facet facet2;
};


struct Camera : public Object {
  explicit Camera(VectorT center, VectorT normal, VectorT upDirection, ScreenT screen)
    : Object(std::move(center)),
      screenNormal(std::move(normal)),
      upDirection(std::move(upDirection)),
      screen(screen),
      ratio(screen.width / (screen.height + 1e-12)) {}
  Camera(const Camera& other) = default;
  VectorT screenNormal{0.0, 0.0, -1.0};
  VectorT upDirection{0.0, 1.0, 0.0};
  ScreenT screen{};
  float ratio{};
};


#endif // GEOMETRY_H_
