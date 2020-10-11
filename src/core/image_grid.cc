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

double ImageGrid::GetValue(pair<int, int> coordinate) {
  return probabilities_[coordinate];
}

void ImageGrid::SetValue(pair<int, int> coordinate, double probability) {
  probabilities_[coordinate] = probability;
}

}  // namespace naivebayes