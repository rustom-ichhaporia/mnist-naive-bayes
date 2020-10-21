#include "core/model.h"

#include <math.h>

#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>
#include <numeric>

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
using std::max_element;
using std::accumulate;
using std::begin;
using std::end;

namespace naivebayes {

void Model::Train(const string& image_path, const string& label_path,
                  size_t image_length) {
  image_height_ = image_length;

  ReadTrainLabels(label_path); 
  ReadTrainImages(image_path, image_length);

  CalculateProbabilities();

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

  vector<int> test_labels;

  size_t test_image_count = test_images_.size();

  if (max_test_images_ < test_images_.size()) {
    cout << "Test images are capped due to time constraints." << endl;
    cout << "Only the first " << max_test_images_ << " images will be classified." << endl;
    test_image_count = max_test_images_;
  }

  for (size_t image_index = 0; image_index < test_image_count; ++image_index) {
    test_labels.push_back(Predict(test_images_.at(image_index)));
  }

  // Return the predicted classes
  return test_labels;
}

int Model::Predict(const ImageGrid& image) {
  // Get likelihood score for each class
  map<int, double> likelihoods;

  for (auto const &classification : train_labels_) {
    likelihoods[classification] = LikelihoodScore(image, classification);
  }

  map<int, double>::iterator best = max_element(
      likelihoods.begin(), likelihoods.end(),
      [](const pair<int, double>& a, const pair<int, double>& b) -> bool {
        return a.second < b.second;
      });

  cout << "The predicted class of the image was: " << best->first << endl;

  // Insert classification of best score
  return best->first;
}

double Model::Score(const string& image_path, const string& label_path) {
  vector<int> predictions = Predict(image_path);
  ReadTestLabels(label_path);

  double score = 0.0;

  for (size_t index = 0; index < predictions.size(); ++index) {
    if (test_labels_.at(index) == predictions.at(index)) {
      score += 1;
    }
  }
  
  score /= predictions.size();

  cout << "The score of the model on the test data was: " << score << endl;
  return score;
}

ifstream& operator>>(ifstream& input, Model& model) {
  string current_line;
  size_t image_row_index = 0;
  size_t current_label_index = 0;
  int current_label = model.train_labels_[current_label_index];

  while (getline(input, current_line)) {
    ++image_row_index;  // 1 indexed for convenience

    // Adds the values to the imagegrid
    model.IncrementGridRow(current_line, current_label, image_row_index);

    // Moves to the next image
    if (image_row_index == model.image_height_) {
      ++current_label_index;
      current_label = model.train_labels_[current_label_index];
      image_row_index = 0;
    }
  }

  return input;
}

vector<ImageGrid> Model::GetTrainImageGrids() const {
  return train_image_grids_;
}

double Model::LikelihoodScore(const ImageGrid& image,
                              int classification) const {
  double sum = 0.0;

  // For each coordinate, add the log of the cell probability
  for (size_t row = 0; row < image_height_; ++row) {
    for (size_t col = 0; col < image_height_; ++col) {
      if (image.GetValue(row, col) > 0.5) {
        sum += log(train_image_grids_.at(classification).GetValue(row, col));
      } else {
        sum += log(1 - train_image_grids_.at(classification).GetValue(row, col));
      }
    }
  }

  // Add the overall class probability
  sum += log(GetClassProbability(classification));
  return sum;
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
  train_image_grids_ = vector<ImageGrid>(label_counts_.size(), ImageGrid(image_height_));
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
        current.IncrementValue(image_row_index - 1, col,
                              kDarkValue);
      }

      else if (current_line[col] == kMediumChar) {
        current.IncrementValue(image_row_index - 1, col,
                              kMediumValue);
      }
    }
    if (image_row_index == image_height_) {
      test_images_.push_back(current); // Add the image to the vector of test images, then reset
      current = ImageGrid(image_height_);
      image_row_index = 0;
    }
  }
}

void Model::ReadTestLabels(const string& label_path) {
  ifstream label_stream(label_path);

  string current_line;

  // Read each line in by converting string to int
  while (getline(label_stream, current_line)) {
    test_labels_.push_back(stoi(current_line));
  }
}

void Model::IncrementGridRow(const string& current_line, int current_label,
                             size_t image_row_index) {
  // Check if the given pixel is dark or medium dark, and increment the model frequencies accordingly                             
  for (size_t col = 0; col < current_line.size(); ++col) {
    if (current_line[col] == kDarkChar) {
      train_image_grids_[current_label].IncrementValue(
          image_row_index - 1, col, kDarkValue);
    }

    else if (current_line[col] == kMediumChar) {
      train_image_grids_[current_label].IncrementValue(
          image_row_index - 1, col, kMediumValue);
    }
  }
}

void Model::CountLabels() {
  // Counts the frequencies of all the labels
  for (size_t label_index = 0; label_index < train_labels_.size();
       ++label_index) {
    label_counts_[train_labels_[label_index]]++;
  }
}

void Model::CalculateProbabilities() {
  for (int classification = 0; classification < label_counts_.size(); ++classification) {
    for (size_t row = 0; row < image_height_; ++row) {
      for (size_t col = 0; col < image_height_; ++col) {
        train_image_grids_[classification].SetValue(row, col,
         (kCellLaplaceSmoother +
            train_image_grids_[classification].GetValue(row, col)) /
           (2 * kCellLaplaceSmoother + label_counts_.at(classification)));
      }
    }
  }
}


}  // namespace naivebayes