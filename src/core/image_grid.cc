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

double ImageGrid::GetValue(pair<int, int> coordinate) {
  return probabilities_[coordinate];
}

void ImageGrid::SetValue(pair<int, int> coordinate, double probability) {
  probabilities_[coordinate] = probability;
}

void ImageGrid::IncrementValue(pair<int, int> coordinate, double increment) {
  probabilities_[coordinate] += increment;
}

}  // namespace naivebayes