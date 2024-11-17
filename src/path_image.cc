#include "path_image.hpp"

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>

PathImage::PathImage(const GrayscaleImage& image,
                     const ElevationDataset& dataset):
    path_image_(image.Height(), std::vector<Color>(image.Width())),
    height_(image.Height()),
    width_(image.Width()) {
  for (size_t i = 0; i < height_; ++i) {
    for (size_t j = 0; j < width_; ++j) {
      path_image_.at(i).at(j) = image.ColorAt(i, j);
    }
  }

  Helper(dataset);

  size_t best_path_row = INT32_MAX;
  size_t best_path_ele_change = INT32_MAX;
  const Color kGreenPath(31, 253, 13);
  const Color kRedPath(252, 25, 63);
  for (size_t i = 0; i < paths_.size(); ++i) {
    if (paths_.at(i).EleChange() < best_path_ele_change) {
      best_path_row = paths_.at(i).StartingRow();
      best_path_ele_change = paths_.at(i).EleChange();
    }
    if (paths_.at(i).StartingRow() == best_path_row) {
      best_path_row = i;
    }
    for (size_t j = 0; j < paths_.at(i).Length(); ++j) {
      path_image_.at(paths_.at(i).GetPath().at(j)).at(j) = kRedPath;
    }
  }
  for (size_t j = 0; j < paths_.at(best_path_row).Length(); ++j) {
    path_image_.at(paths_.at(best_path_row).GetPath().at(j)).at(j) = kGreenPath;
  }
}

size_t PathImage::Width() const { return width_; }

size_t PathImage::Height() const { return height_; }

unsigned int PathImage::MaxColorValue() const { return kMaxColorValue; }

const std::vector<Path>& PathImage::Paths() const { return paths_; }

const std::vector<std::vector<Color>>& PathImage::GetPathImage() const {
  return path_image_;
}

void PathImage::ToPpm(const std::string& name) const {
  std::ofstream ofs{name};
  if (!ofs.is_open()) {
    throw std::runtime_error("File not open.");
  }

  std::string line1 = "P3";
  ofs << line1 << std::endl;
  ofs << width_ << " " << height_ << std::endl;
  ofs << kMaxColorValue << std::endl;
  for (std::vector<Color> color_vec : path_image_) {
    for (Color c : color_vec) {
      ofs << c.Red() << " " << c.Green() << " " << c.Blue() << " ";
    }
    ofs << std::endl;
  }
  ofs << std::endl;
}

void PathImage::Helper(const ElevationDataset& dataset) {
  for (size_t i = 0; i < height_; ++i) {
    Path current_path(width_, i);
    for (size_t j = 0; j < width_ - 1; ++j) {
      int ne_ele = 0;
      int ne_ele_change = INT32_MAX;
      bool ne_flag = false;
      int e_ele = 0;
      int e_ele_change = INT32_MAX;
      bool e_flag = false;
      int se_ele = 0;
      int se_ele_change = INT32_MAX;
      bool se_flag = false;
      int row_val = current_path.GetPath().at(j);
      int current_ele = dataset.DatumAt(row_val, j);
      if (row_val - 1 >= 0) {
        ne_ele = dataset.DatumAt(row_val - 1, j + 1);
        ne_flag = true;
        ne_ele_change = abs(current_ele - ne_ele);
      }
      e_ele = dataset.DatumAt(row_val, j + 1);
      e_flag = true;
      e_ele_change = abs(current_ele - e_ele);
      if (row_val + 1 <= (int)(height_)-1) {
        se_ele = dataset.DatumAt(row_val + 1, j + 1);
        se_flag = true;
        se_ele_change = abs(current_ele - se_ele);
      }
      int min_val = (int)fmin(ne_ele_change, fmin(e_ele_change, se_ele_change));
      ne_flag = ne_ele_change == min_val;
      e_flag = e_ele_change == min_val;
      se_flag = se_ele_change == min_val;
      HelperFlagCheck(current_path, ne_flag, e_flag, se_flag, j, row_val);
      current_path.IncEleChange(min_val);
    }
    paths_.push_back(current_path);
  }
}

void PathImage::HelperFlagCheck(Path& current_path,
                                bool ne_flag,
                                bool e_flag,
                                bool se_flag,
                                size_t col,
                                int row) {
  if ((ne_flag && e_flag && se_flag) || (ne_flag && e_flag) ||
      (e_flag && se_flag) || (e_flag)) {
    current_path.SetLoc(col + 1, row);
  } else if ((ne_flag && se_flag) || (se_flag)) {
    current_path.SetLoc(col + 1, row + 1);
  } else if (ne_flag) {
    current_path.SetLoc(col + 1, row - 1);
  }
}