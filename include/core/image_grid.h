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
  double GetProbability(pair<int, int> coodinate);
  void SetProbability(pair<int, int> coodinate, double probability);

 private:
  map<pair<int, int>, double> probabilities_;
};
}  // namespace naivebayes