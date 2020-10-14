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
  /**
   * @brief Trains the model using given images and labels, as well as a
   * specified image.
   *
   * @param image_path the path to the images
   * @param label_path the path to the labels
   * @param image_height the height of the images in pixels
   */
  void Train(const string& image_path, const string& label_path,
             size_t image_height);

  /**
   * @brief Saves the model information in a cache.
   *
   * @param save_path the path to save the model cache
   */
  void Save(const string& save_path);

  /**
   * @brief Loads the model from a given cache file.
   *
   * @param load_path the path to the cache file
   */
  void Load(const string& load_path);

  /**
   * @brief Predicts the classifications of a set of test images.
   *
   * @param image_path the path to the test images
   * @return vector<int> the classifications of the images in order
   */
  vector<int> Predict(const string& image_path);

  /**
   * @brief The operator overload of the stream extraction operator.
   *
   * @param input the input file stream
   * @param model a reference to the model to read into
   * @return ifstream& the given input file stream
   */
  friend ifstream& operator>>(ifstream& input, Model& model) {
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

  /**
   * @brief A helper class used by Boost to serialize the model.
   *
   */
  friend class access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& train_image_grids_;
    ar& train_labels_;
    ar& label_counts_;
    ar& image_height_;
  }

 private:
  // The k value for laplace smoothing on cells
  const double kCellLaplaceSmoother = 1.0;
  // The k value for laplace smoothing on the class probabilities
  const double kClassLaplaceSmoother = 1.0;
  const char kDarkChar = '#'; // The character for full darkness (1)
  const double kDarkValue = 1.0; // The darkness value for the given character
  const char kMediumChar = '+'; // The character for half darkness (0.5)
  const double kMediumValue = 0.5; // The darkness value for the given character

  /**
   * @brief Calculates the likelihood score of a given image being a given class. 
   * 
   * @param image the ImageGrid object to classify
   * @param classification the test classification
   * @return double the logged likelihood score
   */
  double LikelihoodScore(const ImageGrid& image, int classification) const;

  /**
   * @brief Calculate the probability of a certain cell being shaded in a given classification. 
   * 
   * @param coordinate the coordinate of the cell
   * @param presence the shading level of the cell
   * @param classification the classification to test
   * @return double the probability that the given cell is shaded
   */
  double GetCellProbability(const pair<int, int>& coordinate, double presence,
                            int classification) const;

  /**
   * @brief Calculate the probability of an image being a certain class. 
   * 
   * @param classification the class to test
   * @return double the probability of the image being a part of the given class
   */
  double GetClassProbability(int classification) const;

  /**
   * @brief Reads the training labels from a file. 
   * 
   * @param label_path the path to the training labels
   */
  void ReadTrainLabels(const string& label_path);

  /**
   * @brief Reads the training images from a file. 
   * 
   * @param image_path the path to the trianing images
   * @param image_height the height of the training images in pixels
   */
  void ReadTrainImages(const string& image_path, size_t image_height);

  /**
   * @brief Reads the test images from a file. 
   * 
   * @param image_path the path to the test images
   */
  void ReadTestImages(const string& image_path);

  /**
   * @brief Increments the row of a grid when reading the training images. 
   * 
   * @param current_line the current line of the grid
   * @param current_label the current label of the image
   * @param image_row_index the row of the image
   */
  void IncrementGridRow(const string& current_line, int current_label,
                        size_t image_row_index);

  /**
   * @brief Counts the number of labels in each category for a member variable. 
   * 
   */
  void CountLabels();

  // An ImageGrid containing the frequencies of each pixel being shaded in the training images
  map<int, ImageGrid> train_image_grids_;
  // A vector of the labels of the training images
  vector<int> train_labels_;
  // A map of the counts of the labels based on each classification
  map<int, int> label_counts_;
  // A vector of the test images that are read in for classification
  vector<ImageGrid> test_images_;
  // The height of the images in pixels for the given model
  size_t image_height_;
};

}  // namespace naivebayes