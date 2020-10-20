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

  // Initializes all square values with 0
  for (size_t row = 0; row < image_height_; row++) {
    for (size_t col = 0; col < image_length; col++) {
      shade_grid_[pair<size_t, size_t>(row, col)] = 0;
    }
  }
}

double ImageGrid::GetValue(const pair<int, int>& coordinate) const{
  return shade_grid_.at(coordinate);
}

void ImageGrid::SetValue(const pair<int, int>& coordinate, double probability) {
  shade_grid_.at(coordinate) = probability;
}

void ImageGrid::IncrementValue(const pair<int, int>& coordinate, double increment) {
  shade_grid_.at(coordinate) += increment;
}

map<pair<size_t, size_t>, double> ImageGrid::GetGrid() const {
  return shade_grid_;
}
}  // namespace naivebayes