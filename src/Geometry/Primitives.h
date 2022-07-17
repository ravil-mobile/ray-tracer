#ifndef PRIMITIVES_H_
#define PRIMITIVES_H_

#include <boost/gil/typedefs.hpp>
#include <Eigen/Core>


using VectorT = Eigen::Vector3f;
using ColorT = boost::gil::rgb8_pixel_t;


template<typename Type>
struct Rectangle {
  Type width{};
  Type height{};
};

using ScreenT = Rectangle<float>;
using ImageViewT = Rectangle<size_t>;


#endif // PRIMITIVES_H_
