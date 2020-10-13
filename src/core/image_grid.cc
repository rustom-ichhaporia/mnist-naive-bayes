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

using std::map;
namespace naivebayes {

ImageGrid::ImageGrid() {
  
}

ImageGrid::ImageGrid(size_t image_length) {
  for (size_t row = 0; row < image_length; row++) {
    for (size_t col = 0; col < image_length; col++) {
      probabilities_[pair<int, int>(row, col)] = 0;
    }
  }
}

double ImageGrid::GetValue(const pair<int, int>& coordinate) const{
  return probabilities_.at(coordinate);
}

void ImageGrid::SetValue(const pair<int, int>& coordinate, double probability) {
  probabilities_[coordinate] = probability;
}

void ImageGrid::IncrementValue(const pair<int, int>& coordinate, double increment) {
  probabilities_[coordinate] += increment;
}

map<pair<int, int>, double> ImageGrid::GetGrid() const {
  return probabilities_;
}

}  // namespace naivebayes