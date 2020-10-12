#include "core/model.h"

#include <math.h>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using math::log;
using std::count;
using std::ifstream;
using std::map;
using std::pair;
using std::string;
using std::vector;

namespace naivebayes {

void Model::Train(const string& image_path, const string& label_path, size_t image_length) {
  ReadLabels(label_path);
  ReadImages(image_path, image_length);

  // label_stream >> labels_;
}

void Model::Save(const string& save_path) {
}

void Model::Load(const string& load_path) {
}

// int Predict(const ImageGrid& image) {
//   return
// }

// double LikelihoodScores() {
//   double score = 0;
//   // score += log()
//   return score;
// }

// vector<double> Model::GetCellProbabilities(double presence) {

// }

double Model::GetCellProbability(pair<int, int> coordinate, double presence, int classification) {
  return (kCellLaplaceSmoother + image_grid_[classification].GetValue(coordinate)) / 
          (2 * kCellLaplaceSmoother + count(labels_.begin(), labels_.end(), classification));
}

double Model::GetClassProbability(int classification) {
  return (kClassLaplaceSmoother + count(labels_.begin(), labels.end()))
}

void Model::ReadLabels(string& label_path) {
  ifstream label_stream(label_path);

  string current_line;

  while (getline(label_stream, current_line)) {
    labels_.push_back(stoi(current_line));
  }
}

void Model::ReadImages(string& image_path, string& image_length) {
  ifstream image_stream(image_path);
  image_stream >> image_grid_;
}

void Model::IncrementGridRow(string& current_line, int current_label, size_t image_row_index) {
  for (size_t col = 0; col < current_line.size(); ++col) {
    if (current_line[col] == model.kDarkChar) {
      image_grid_[current_label].IncrementValue(pair<int, int>(image_row_index - 1, col), kDarkValue);
    } 

    else if (current_line[col] == kMediumChar) {
      image_grid_[current_label].IncrementValue(pair<int, int>(image_row_index - 1, col), kMediumValue);
    }
  }
}

}  // namespace naivebayes