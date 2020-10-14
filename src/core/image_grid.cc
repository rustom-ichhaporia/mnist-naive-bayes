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
  if (image_length < 0) {
    throw invalid_argument("The input size of the image grid was negative, which is invalid.");
  }

  image_length_ = image_length;

  for (size_t row = 0; row < image_length_; row++) {
    for (size_t col = 0; col < image_length; col++) {
      shade_presence_[pair<int, int>(row, col)] = 0;
    }
  }
}

double ImageGrid::GetValue(const pair<int, int>& coordinate) const{
  return shade_presence_.at(coordinate);
}

void ImageGrid::SetValue(const pair<int, int>& coordinate, double probability) {
  shade_presence_.at(coordinate) = probability;
}

void ImageGrid::IncrementValue(const pair<int, int>& coordinate, double increment) {
  shade_presence_.at(coordinate) += increment;
}

map<pair<int, int>, double> ImageGrid::GetGrid() const {
  return shade_presence_;
}
}  // namespace naivebayes