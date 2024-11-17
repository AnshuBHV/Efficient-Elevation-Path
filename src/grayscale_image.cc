#include "grayscale_image.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

GrayscaleImage::GrayscaleImage(const ElevationDataset& dataset):
    image_(dataset.Height(), std::vector<Color>(dataset.Width())) {
  width_ = dataset.Width();
  height_ = dataset.Height();

  for (size_t i = 0; i < height_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      double val = dataset.DatumAt(i, j);
      // std::cout << "val = " << val << std::endl;

      double min = dataset.MinEle();
      double max = dataset.MaxEle();
      // std::cout << "min = " << min << std::endl;
      // std::cout << "max = " << max << std::endl;

      int shade_of_grey = 0;
      if (max != min) {
        shade_of_grey =
            (int)std::round((val - min) / (max - min) * kMaxColorValue);
      }

      // std::cout << "shade_of_grey = " << shade_of_grey << std::endl;

      Color color(shade_of_grey, shade_of_grey, shade_of_grey);
      image_.at(i).at(j) = color;
    }
  }
}

GrayscaleImage::GrayscaleImage(const std::string& filename,
                               size_t width,
                               size_t height):
    image_(height, std::vector<Color>(width)) {
  width_ = width;
  height_ = height;
  ElevationDataset ed(filename, width_, height_);

  for (size_t i = 0; i < height_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      double val = ed.DatumAt(i, j);
      // std::cout << "val = " << val << std::endl;

      double min = ed.MinEle();
      double max = ed.MaxEle();
      // std::cout << "min = " << min << std::endl;
      // std::cout << "max = " << max << std::endl;

      int shade_of_grey = 0;
      if (max != min) {
        shade_of_grey =
            (int)std::round((val - min) / (max - min) * kMaxColorValue);
      }

      // std::cout << "shade_of_grey = " << shade_of_grey << std::endl;

      Color color(shade_of_grey, shade_of_grey, shade_of_grey);
      image_.at(i).at(j) = color;
    }
  }
}

size_t GrayscaleImage::Width() const { return width_; }

size_t GrayscaleImage::Height() const { return height_; }

unsigned int GrayscaleImage::MaxColorValue() const { return kMaxColorValue; }

const Color& GrayscaleImage::ColorAt(int row, int col) const {
  return image_.at(row).at(col);
}

const std::vector<std::vector<Color>>& GrayscaleImage::GetImage() const {
  return image_;
}

void GrayscaleImage::ToPpm(const std::string& name) const {
  std::ofstream ofs{name};
  if (!ofs.is_open()) {
    throw std::runtime_error("File not open.");
  }

  std::string line1 = "P3";
  ofs << line1 << std::endl;
  ofs << width_ << " " << height_ << std::endl;
  ofs << kMaxColorValue << std::endl;
  for (std::vector<Color> color_vec : image_) {
    for (Color c : color_vec) {
      ofs << c.Red() << " " << c.Green() << " " << c.Blue() << " ";
    }
    ofs << std::endl;
  }
  ofs << std::endl;
}