#include "core/model.h"

#include <math.h>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using boost::archive::text_iarchive;
using boost::archive::text_oarchive;
using std::count;
using std::cout;
using std::endl;
using std::ifstream;
using std::map;
using std::ofstream;
using std::pair;
using std::string;
using std::vector;

namespace naivebayes {

void Model::Train(const string& image_path, const string& label_path,
                  size_t image_length) {
  image_length_ = image_length;

  ReadLabels(label_path);
  ReadTrainImages(image_path, image_length);

  // Once images and labels have been loaded, then model is ready for prediction
  cout << "The model has been trained using images from: " << image_path
       << "\n And labels from: " << label_path << endl;
}

void Model::Save(const string& save_path) {
  // Create output stream
  ofstream output(save_path);
  text_oarchive archive(output);
  archive << *this;
  cout << "The model has been cached to: " << save_path << endl;
}

void Model::Load(const string& load_path) {
  ifstream ifs(load_path);
  text_iarchive ia(ifs);
  ia >> *this;

  cout << "The model has been loaded from: " << load_path << endl;
}

int Model::Predict(const string& image_path) {
  ImageGrid image = ReadTestImage(image_path);

  map<int, double> likelihoods;

  // Get likelihood score for each class
  for (auto classification : train_labels_) {
    likelihoods[classification] = LikelihoodScore(image, classification);
  }

  // Return index of largest likelihood score
  return likelihoods.rbegin()->first;
}

double Model::LikelihoodScore(const ImageGrid& image,
                              int classification) const {
  double sum = 0.0;
  for (auto coordinate : train_image_grids_.at(classification).GetGrid()) {
    sum += log(GetCellProbability(
        coordinate.first, image.GetValue(coordinate.first), classification));
  }
  sum += log(GetClassProbability(classification));
  return 0;
}

double Model::GetCellProbability(const pair<int, int>& coordinate,
                                 double presence, int classification) const {
  return (kCellLaplaceSmoother +
          train_image_grids_.at(classification).GetValue(coordinate)) /
         (2 * kCellLaplaceSmoother + label_counts_.at(classification));
}

double Model::GetClassProbability(int classification) const {
  return (kClassLaplaceSmoother + label_counts_.at(classification)) /
         (label_counts_.size() * kClassLaplaceSmoother + train_labels_.size());
}

void Model::ReadLabels(const string& label_path) {
  ifstream label_stream(label_path);

  string current_line;

  while (getline(label_stream, current_line)) {
    train_labels_.push_back(stoi(current_line));
  }

  CountLabels();
}

void Model::ReadTrainImages(const string& image_path, size_t image_length) {
  ifstream image_stream(image_path);
  image_stream >> *this;
}

ImageGrid Model::ReadTestImage(const string& image_path) {
  ifstream image_stream(image_path);

  string current_line;
  size_t image_row_index = 0;


  ImageGrid result;


  while (getline(image_stream, current_line)) {
    image_row_index++;
    for (size_t col = 0; col < current_line.size(); ++col) {
      if (current_line[col] == kDarkChar) {
        result.IncrementValue(pair<int, int>(image_row_index - 1, col),
                              kDarkValue);
      }

      else if (current_line[col] == kMediumChar) {
        result.IncrementValue(pair<int, int>(image_row_index - 1, col),
                              kMediumValue);
      }
    }
  }
  return result;
}

void Model::IncrementGridRow(const string& current_line, int current_label,
                             size_t image_row_index) {
  for (size_t col = 0; col < current_line.size(); ++col) {
    if (current_line[col] == kDarkChar) {
      train_image_grids_[current_label].IncrementValue(
          pair<int, int>(image_row_index - 1, col), kDarkValue);
    }

    else if (current_line[col] == kMediumChar) {
      train_image_grids_[current_label].IncrementValue(
          pair<int, int>(image_row_index - 1, col), kMediumValue);
    }
  }
}

void Model::CountLabels() {
  for (size_t label_index = 0; label_index < train_labels_.size();
       ++label_index) {
    label_counts_[train_labels_[label_index]]++;
  }
  for (size_t classification_index = 0;
       classification_index < label_counts_.size(); ++classification_index) {
    train_image_grids_[classification_index] = ImageGrid(image_length_);
  }
}

}  // namespace naivebayes