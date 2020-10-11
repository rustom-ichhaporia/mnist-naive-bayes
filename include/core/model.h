#pragma once
#include <iostream>
#include <string>
#include <vector>

using std::string;
using std::vector;

namespace naivebayes {

class Model {
 public:
  void Train(const string& image_path, const string& label_path);
  void Save(const string& save_path);
  void Load(const string& load_path);

 private:

  vector<vector<int>> predictions_;
};

}  // namespace naivebayes