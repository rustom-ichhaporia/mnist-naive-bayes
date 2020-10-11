#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <tuple>
#include <utility>
#include "image_grid.h"

using std::string;
using std::vector;
using std::map;
using std::tuple;
using std::pair;

namespace naivebayes {

class Model {
 public:
  void Train(const string& image_path, const string& label_path);
  void Save(const string& save_path);
  void Load(const string& load_path);

 private:

  map<int, ImageGrid> cells;
  pair<int, int> image_dimensions_;
};

}  // namespace naivebayes