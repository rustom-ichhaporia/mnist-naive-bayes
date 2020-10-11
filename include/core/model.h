#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "image_grid.h"

using std::ifstream;
using std::ofstream;
using std::map;
using std::pair;
using std::string;
using std::tuple;
using std::vector;
using std::stoi;

namespace naivebayes {

class Model {
 public:
  void Train(const string& image_path, const string& label_path, size_t image_length);
  void Save(const string& save_path);
  void Load(const string& load_path);

  // friend ifstream& operator>>(ifstream& input, map<int, ImageGrid>& image_cells) {
  //   string current_line;
  //   //size_t image_row_index = 0;

  //   // while (getline(input, current_line)) {
  //   //   if (image_row_index == model.image_length_) {
  //   //     image_row_index = 0;
  //   //     model.image_cells_.push
  //   //   }
  //   // }

  //   return input;
  // }

  // friend ifstream& operator>>(ifstream& input, vector<int>& label_vector) {
  //   string current_line;

  //   while (getline(input, current_line)) {
  //     label_vector.push_back(stoi(current_line));
  //   }

  //   return input;
  // }

  

  // friend ifstream& operator<<(ofstream& output, Model& model) {
  //   return
  // }

 private:
  map<int, ImageGrid> image_cells_;
  vector<int> labels_;
  size_t image_length_;

  // pair<int, int> image_dimensions_;
};

}  // namespace naivebayes