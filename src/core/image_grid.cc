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

double ImageGrid::GetProbability(pair<int, int> coordinate) {
  return probabilities_[coordinate];
}

void ImageGrid::SetProbability(pair<int, int> coordinate, double probability) {
  probabilities_[coordinate] = probability;
}

}  // namespace naivebayes