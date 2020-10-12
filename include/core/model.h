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
  int Predict(const ImageGrid& image);

  friend ifstream& operator>>(ifstream& input, Model& model) {
    string current_line;
    size_t image_row_index = 0;
    size_t current_label_index = 0;
    int current_label = model.labels_[current_label_index];

    while (getline(input, current_line)) {
      ++image_row_index;

      model.IncrementGridRow(current_line, current_label, image_row_index);

      if (image_row_index == model.image_length_) {
        ++current_label_index;
        current_label = model.labels_[current_label_index];
        image_row_index = 0;
      }
    }

    return input;
  }

 private:
  const double kCellLaplaceSmoother = 1.0;
  const double kClassLaplaceSmoother = 1.0;
  const char kDarkChar = '#';
  const double kDarkValue = 1.0;
  const char kMediumChar = '+';
  const double kMediumValue = 0.5;

  double LikelihoodScores();

  // vector<double> GetCellProbabilities(double presence);


  double GetCellProbability(pair<int, int> coordinate, double presence, int classification);
  double GetClassProbability(int classification);

  void ReadLabels(string& label_path);
  void ReadImages(string& image_path, string& image_length);

  void IncrementGridRow(string& current_line, int current_label, size_t image_row_index);

  map<int, ImageGrid> image_grid_;
  vector<int> labels_;
  size_t image_length_;

  // pair<int, int> image_dimensions_;
};

}  // namespace naivebayes