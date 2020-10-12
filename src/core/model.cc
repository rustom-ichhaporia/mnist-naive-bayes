#include "core/model.h"

#include <math.h>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using boost::archive::text_oarchive;
using boost::archive::text_iarchive;
using std::cout;
using std::count;
using std::ifstream;
using std::map;
using std::ofstream;
using std::pair;
using std::string;
using std::vector;
using std::clog;


namespace naivebayes {

void Model::Train(const string& image_path, const string& label_path,
                  size_t image_length) {
  image_length_ = image_length;
  ReadLabels(label_path);
  ReadImages(image_path, image_length);
}

void Model::Save(const string& save_path) {
  ofstream output(save_path);
  // save data to archive
  {
    text_oarchive archive(output);
    // write class instance to archive
    archive << *this;
    // archive and stream closed when destructors are called
  }
  cout << "The model has been cached to: " << save_path;
}

void Model::Load(const string& load_path) {
  {
    // create and open an archive for input
    ifstream ifs(load_path);
    text_iarchive ia(ifs);
    // read class state from archive
    ia >> *this;
    // archive and stream closed when destructors are called
  }

  cout << "The model has been loaded from: " << load_path;
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

double Model::GetCellProbability(pair<int, int> coordinate, double presence,
                                 int classification) {
  return (kCellLaplaceSmoother +
          image_grids_[classification].GetValue(coordinate)) /
         (2 * kCellLaplaceSmoother + label_counts_[classification]);
}

double Model::GetClassProbability(int classification) {
  return (kClassLaplaceSmoother + label_counts_[classification]) /
         (label_counts_.size() * kClassLaplaceSmoother + labels_.size());
}

void Model::ReadLabels(const string& label_path) {
  ifstream label_stream(label_path);

  string current_line;

  while (getline(label_stream, current_line)) {
    labels_.push_back(stoi(current_line));
  }

  CountLabels();
}

void Model::ReadImages(const string& image_path, size_t image_length) {
  ifstream image_stream(image_path);
  image_stream >> *this;
}

void Model::IncrementGridRow(const string& current_line, int current_label,
                             size_t image_row_index) {
  for (size_t col = 0; col < current_line.size(); ++col) {
    if (current_line[col] == kDarkChar) {
      image_grids_[current_label].IncrementValue(
          pair<int, int>(image_row_index - 1, col), kDarkValue);
    }

    else if (current_line[col] == kMediumChar) {
      image_grids_[current_label].IncrementValue(
          pair<int, int>(image_row_index - 1, col), kMediumValue);
    }
  }
}

void Model::CountLabels() {
  for (size_t label_index = 0; label_index < labels_.size(); ++label_index) {
    label_counts_[labels_[label_index]]++;
  }
  for (size_t classification_index = 0; classification_index < label_counts_.size(); ++classification_index) {
    image_grids_[classification_index] = ImageGrid(image_length_);
  }
}

}  // namespace naivebayes