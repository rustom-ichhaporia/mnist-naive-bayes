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
   * @brief Predicts the classification of a given ImageGrid.
   *
   * @param image  the ImageGrid to classify
   * @return int the predicted class
   */
  int Predict(const ImageGrid& image);

  /**
   * @brief Calculates the score of accurate classifications for a given test
   * image set.
   *
   * @param image_path the string path to the test images
   * @param label_path the string path to the lable images
   * @return double the ratio of accurate classifications to total images
   */
  double Score(const string& image_path, const string& label_path);

  /**
   * @brief The operator overload of the stream extraction operator.
   *
   * @param input the input file stream
   * @param model a reference to the model to read into
   * @return ifstream& the given input file stream
   */
  friend ifstream& operator>>(ifstream& input, Model& model);

  /**
   * @brief Gets the Train Image Grids from the model for diagnostic purposes.
   *
   * @return vector<ImageGrid> the training probabilities
   */
  vector<ImageGrid> GetTrainImageGrids() const;

  /**
   * @brief Calculates the likelihood score of a given image being a given
   * class.
   *
   * @param image the ImageGrid object to classify
   * @param classification the test classification
   * @return double the logged likelihood score
   */
  double LikelihoodScore(const ImageGrid& image, int classification) const;

  /**
   * @brief Calculate the probability of an image being a certain class.
   *
   * @param classification the class to test
   * @return double the probability of the image being a part of the given class
   */
  double GetClassProbability(int classification) const;

  /**
   * @brief A helper class used by Boost to serialize the model. Must be placed
   * in header.
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
  static constexpr double kCellLaplaceSmoother = 1.0;
  // The k value for laplace smoothing on the class probabilities
  static constexpr double kClassLaplaceSmoother = 1.0;
  // The character for full darkness (1)
  static constexpr char kDarkChar = '#';  
  // The darkness value for the given character
  static constexpr double kDarkValue = 1.0;
  // The character for half darkness (0.5)
  static constexpr char kMediumChar = '+';
  // The darkness value for the given character
  static constexpr double kMediumValue = 0.5;

  /**
   * @brief Reads the training images from a file.
   *
   * @param image_path the path to the trianing images
   * @param image_height the height of the training images in pixels
   */
  void ReadTrainImages(const string& image_path, size_t image_height);

  /**
   * @brief Reads the training labels from a file.
   *
   * @param label_path the path to the training labels
   */
  void ReadTrainLabels(const string& label_path);

  /**
   * @brief Reads the test images from a file.
   *
   * @param image_path the path to the test images
   */
  void ReadTestImages(const string& image_path);

  /**
   * @brief Reads the test labels from a file.
   *
   * @param label_path the path to the test labels
   */
  void ReadTestLabels(const string& label_path);

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

  /**
   * @brief Calculates the probabilities of being shaded for each classification
   * and each cell when training the model.
   *
   */
  void CalculateProbabilities();

  void PrintConfusionMatrix();

  // An ImageGrid containing the frequencies of each pixel being shaded in the
  // training images
  vector<ImageGrid> train_image_grids_;
  // A vector of the labels of the training images
  vector<int> train_labels_;
  // A map of the counts of the labels based on each classification
  map<int, int> label_counts_;
  // A vector of the test images that are read in for classification
  vector<ImageGrid> test_images_;
  // A vector of test labels that are to be compared to test images
  vector<int> test_labels_;
  // The height of the images in pixels for the given model
  size_t image_height_;
  // The limit of the number of images that can be scored due to time
  size_t max_test_images_ = 100;
  // A confusion matrix for testing
  vector<vector<int>> confusion_matrix_;
};

}  // namespace naivebayes