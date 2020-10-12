#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using std::string;
using std::vector;
using std::map;
using std::pair;

namespace naivebayes {

class ImageGrid {
 public:
  double GetValue(pair<int, int> coodinate);
  void SetValue(pair<int, int> coodinate, double probability);
  void IncrementValue(pair<int, int> coordinate, double increment);

 private:
  map<pair<int, int>, double> probabilities_;
};
}  // namespace naivebayes