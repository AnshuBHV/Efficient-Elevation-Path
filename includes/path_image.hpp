#ifndef PATH_IMAGE_H
#define PATH_IMAGE_H

#include <cstdlib>  // for size_t
#include <string>
#include <vector>

#include "color.hpp"
#include "elevation_dataset.hpp"
#include "grayscale_image.hpp"
#include "path.hpp"

class PathImage {
public:
  // write behavior declarations here; define in path_image.cc.
  PathImage(const GrayscaleImage& image, const ElevationDataset& dataset);
  size_t Width() const;
  size_t Height() const;
  unsigned int MaxColorValue() const;
  const std::vector<Path>& Paths() const;
  const std::vector<std::vector<Color>>& GetPathImage() const;
  void ToPpm(const std::string& name) const;
  void Helper(const ElevationDataset& dataset);
  void HelperFlagCheck(Path& current_path,
                       bool ne_flag,
                       bool e_flag,
                       bool se_flag,
                       size_t col,
                       int row);

private:
  std::vector<Path> paths_;
  std::vector<std::vector<Color>> path_image_;
  size_t height_ = 0;
  size_t width_ = 0;
  static const int kMaxColorValue = 255;
};

#endif