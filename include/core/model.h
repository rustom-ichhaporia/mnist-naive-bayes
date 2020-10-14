#pragma once
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "image_grid.h"

using boost::serialization::access;
using std::ifstream;
using std::map;
using std::ofstream;
using std::pair;
using std::stoi;
using std::string;
using std::vector;

namespace naivebayes {

class Model {
 public:
  void Train(const string& image_path, const string& label_path,
             size_t image_length);
  void Save(const string& save_path);
  void Load(const string& load_path);
  vector<int> Predict(const string& image_path);

  friend ifstream& operator>>(ifstream& input, Model& model) {
    string current_line;
    size_t image_row_index = 0;
    size_t current_label_index = 0;
    int current_label = model.train_labels_[current_label_index];

    while (getline(input, current_line)) {
      ++image_row_index;

      model.IncrementGridRow(current_line, current_label, image_row_index);

      if (image_row_index == model.image_length_) {
        ++current_label_index;
        current_label = model.train_labels_[current_label_index];
        image_row_index = 0;
      }
    }

    return input;
  }

  friend class access;
  // When the class Archive corresponds to an output archive, the
  // & operator is defined similar to <<.  Likewise, when the class Archive
  // is a type of input archive the & operator is defined similar to >>.
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& train_image_grids_;
    ar& train_labels_;
    ar& label_counts_;
    ar& image_length_;
  }

 private:
  const double kCellLaplaceSmoother = 1.0;
  const double kClassLaplaceSmoother = 1.0;
  const char kDarkChar = '#';
  const double kDarkValue = 1.0;
  const char kMediumChar = '+';
  const double kMediumValue = 0.5;

  double LikelihoodScore(const ImageGrid& image, int classification) const;

  double GetCellProbability(const pair<int, int>& coordinate, double presence,
                            int classification) const;
  double GetClassProbability(int classification) const;

  void ReadTrainLabels(const string& label_path);
  void ReadTrainImages(const string& image_path, size_t image_length);

  void ReadTestImages(const string& image_path);
  void ReadTestLabels(const string& label_path);

  void IncrementGridRow(const string& current_line, int current_label,
                        size_t image_row_index);
  void CountLabels();

  map<int, ImageGrid> train_image_grids_;
  vector<int> train_labels_;
  map<int, int> label_counts_;

  vector<ImageGrid> test_images_;

  size_t image_length_;
};

}  // namespace naivebayes