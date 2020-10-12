#pragma once
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <tuple>
#include <utility>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/map.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include "image_grid.h"

using std::ifstream;
using std::ofstream;
using std::map;
using std::pair;
using std::string;
using std::tuple;
using std::vector;
using std::stoi;
using boost::serialization::access;

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

    friend class access;
    // When the class Archive corresponds to an output archive, the
    // & operator is defined similar to <<.  Likewise, when the class Archive
    // is a type of input archive the & operator is defined similar to >>.
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& image_grids_;
    ar& labels_;
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



  double LikelihoodScores();

  // vector<double> GetCellProbabilities(double presence);




  double GetCellProbability(pair<int, int> coordinate, double presence, int classification);
  double GetClassProbability(int classification);

  void ReadLabels(const string& label_path);
  void ReadImages(const string& image_path, size_t image_length);

  void IncrementGridRow(const string& current_line, int current_label, size_t image_row_index);
  void CountLabels();

  map<int, ImageGrid> image_grids_;
  vector<int> labels_;
  map<int, int> label_counts_;
  size_t image_length_;

  // pair<int, int> image_dimensions_;
};

}  // namespace naivebayes