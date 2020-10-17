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
  image_height_ = image_length;

  ReadTrainLabels(label_path); 
  ReadTrainImages(image_path, image_length);

  // Once images and labels have been loaded, then model is ready for prediction
  cout << "The model has been trained using images from: " << image_path
       << "\n And labels from: " << label_path << endl;
}

void Model::Save(const string& save_path) {
  // Create output stream
  ofstream output(save_path);
  text_oarchive archive(output);
  archive << *this; // Serialize the model into the archive using Boost
  cout << "The model has been cached to: " << save_path << endl;
}

void Model::Load(const string& load_path) {
  ifstream ifs(load_path);
  text_iarchive ia(ifs);
  ia >> *this;   // Read the model from the serialized file using Boost

  cout << "The model has been loaded from: " << load_path << endl;
}

vector<int> Model::Predict(const string& image_path) {
  ReadTestImages(image_path);

  map<int, double> likelihoods;
  vector<int> test_labels;

  for (ImageGrid image : test_images_) {
    // Get likelihood score for each class
    for (auto classification : train_labels_) {
      likelihoods[classification] = LikelihoodScore(image, classification);
    }

    test_labels.push_back(likelihoods.rbegin()->first); // Insert the index of highest
    likelihoods.clear();
  }

  // Return index of largest likelihood score
  return test_labels;
}

double Model::LikelihoodScore(const ImageGrid& image,
                              int classification) const {
  double sum = 0.0;

  // For each coordinate, add the log of the cell probability
  for (auto coordinate : train_image_grids_.at(classification).GetGrid()) {
    sum += log(GetCellProbability(
        coordinate.first, image.GetValue(coordinate.first), classification));
  }

  // Add the overall class probability
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

void Model::ReadTrainLabels(const string& label_path) {
  ifstream label_stream(label_path);

  string current_line;

  // Read each line in by converting string to int
  while (getline(label_stream, current_line)) {
    train_labels_.push_back(stoi(current_line));
  }

  CountLabels(); // Count the frequency of each label
}

void Model::ReadTrainImages(const string& image_path, size_t image_length) {
  ifstream image_stream(image_path);
  image_stream >> *this; // Reads the training images using operator overloading
}

void Model::ReadTestImages(const string& image_path) {
  // The following method is nearly identical to the operator overloaded method, but
  // uses different variables. This is poor object design, but necessitated by the requirements
  // of the assignment. 
  ifstream image_stream(image_path);

  string current_line;
  size_t image_row_index = 0;

  ImageGrid current(image_height_);

  while (getline(image_stream, current_line)) {
    image_row_index++;
    for (size_t col = 0; col < current_line.size(); ++col) {
      if (current_line[col] == kDarkChar) {
        current.IncrementValue(pair<int, int>(image_row_index - 1, col),
                              kDarkValue);
      }

      else if (current_line[col] == kMediumChar) {
        current.IncrementValue(pair<int, int>(image_row_index - 1, col),
                              kMediumValue);
      }
    }
    if (image_row_index == image_height_) {
      test_images_.push_back(current); // Add the image to the vector of test images, then reset
      current = ImageGrid(image_height_);
    }
  }
}

void Model::IncrementGridRow(const string& current_line, int current_label,
                             size_t image_row_index) {
  // Check if the given pixel is dark or medium dark, and increment the model frequencies accordingly                             
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
  // Counts the frequencies of all the labels
  for (size_t label_index = 0; label_index < train_labels_.size();
       ++label_index) {
    label_counts_[train_labels_[label_index]]++;
  }
  for (size_t classification_index = 0;
       classification_index < label_counts_.size(); ++classification_index) {
    train_image_grids_[classification_index] = ImageGrid(image_height_);
  }
}

}  // namespace naivebayes