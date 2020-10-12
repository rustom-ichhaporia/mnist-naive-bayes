#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

using std::string;
using std::vector;
using std::map;
using std::pair;
using boost::serialization::access;

namespace naivebayes {

class ImageGrid {
 public:
  ImageGrid();
  ImageGrid(size_t image_length);
  double GetValue(pair<int, int> coodinate);
  void SetValue(pair<int, int> coodinate, double probability);
  void IncrementValue(pair<int, int> coordinate, double increment);
  friend class access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& probabilities_;
  }

 private:
  map<pair<int, int>, double> probabilities_;
};
}  // namespace naivebayes