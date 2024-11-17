#include "elevation_dataset.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

ElevationDataset::ElevationDataset(const std::string& filename,
                                   size_t width,
                                   size_t height):
    data_(height, std::vector<int>(width)) {
  width_ = width;
  height_ = height;
  std::ifstream ifs(filename);
  if (!ifs.is_open()) {
    throw std::runtime_error("File is not open.");
  }

  max_ele_ = INT32_MIN;
  min_ele_ = INT32_MAX;

  int val = 0;
  size_t i = 0;
  size_t j = 0;
  for (i = 0; i < height_; ++i) {
    for (j = 0; j < width_; ++j) {
      ifs >> val;
      if (ifs.fail()) {
        throw std::runtime_error("Formatted read error.");
      }
      data_.at(i).at(j) = val;
      if (val > max_ele_) {
        max_ele_ = val;
      }
      if (val < min_ele_) {
        min_ele_ = val;
      }
    }
  }
  val = INT32_MIN;
  ifs >> val;
  if (!ifs.fail()) {
    throw std::runtime_error("Too much.");
  }
}

size_t ElevationDataset::Width() const { return width_; }

size_t ElevationDataset::Height() const { return height_; }

int ElevationDataset::MaxEle() const { return max_ele_; }

int ElevationDataset::MinEle() const { return min_ele_; }

int ElevationDataset::DatumAt(size_t row, size_t col) const {
  return data_.at(row).at(col);
}

const std::vector<std::vector<int>>& ElevationDataset::GetData() const {
  return data_;
}
