#include "Misc/Misc.h"
#include <boost/gil.hpp>
#include <boost/gil/io/write_view.hpp>
#include <boost/gil/extension/io/jpeg.hpp>
#include <fstream>


void save(const std::string& fileName, const Canvas& canvas) {
  const auto& imageView = canvas.getImageView();
  boost::gil::rgb8_image_t img(imageView.width, imageView.height);
  const auto& view = boost::gil::view(img);

  for(std::ptrdiff_t y = 0; y < imageView.height; ++y) {
    auto srcIt = view.row_begin(y);
    for (std::ptrdiff_t x = 0; x < imageView.width; ++x) {
      srcIt[x] = canvas.at(x, y);
    }
  }

  std::ofstream filestream("image.jpeg", std::ios::out | std::ios_base::binary);
  boost::gil::write_view(filestream, view, boost::gil::jpeg_tag());
}