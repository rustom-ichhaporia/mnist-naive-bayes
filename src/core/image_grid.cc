#include "core/image_grid.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::invalid_argument;

using std::map;
namespace naivebayes {

ImageGrid::ImageGrid() {
  
}

ImageGrid::ImageGrid(size_t image_length) {
  image_height_ = image_length;

  shade_grid_ = vector<vector<double>>(image_height_, vector<double>(image_height_, 0));

  // Initializes all square values with 0
  // for (size_t row = 0; row < image_height_; row++) {
  //   for (size_t col = 0; col < image_length; col++) {
  //     shade_grid_[row][col] = 0;
  //   }
  // }
}

double ImageGrid::GetValue(size_t x, size_t y) const {
  return shade_grid_[x][y];
}

void ImageGrid::SetValue(size_t x, size_t y, double probability) {
  shade_grid_[x][y] = probability;
}

void ImageGrid::IncrementValue(size_t x, size_t y, double increment) {
  shade_grid_[x][y] += increment;
}

vector<vector<double>> ImageGrid::GetGrid() const {
  return shade_grid_;
}
}  // namespace naivebayes