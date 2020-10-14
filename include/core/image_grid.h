#pragma once
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <iostream>
#include <map>
#include <string>
#include <utility>
#include <vector>

using boost::serialization::access;
using std::map;
using std::pair;
using std::string;
using std::vector;

namespace naivebayes {

class ImageGrid {
 public:
  /**
   * @brief Empty constructor for ImageGrid
   *
   */
  ImageGrid();

  /**
   * @brief Constructor for ImageGrid using image size
   *
   * @param image_length the the height of the image
   */
  ImageGrid(size_t image_length);

  /**
   * @brief Get the value at a certain pixel
   *
   * @param coodinate the coordinate of the pixel whose value to get
   * @return double representing the count or probability value at that pixel
   */
  double GetValue(const pair<int, int>& coodinate) const;

  /**
   * @brief Set the value at a certain pixel
   *
   * @param coodinate the coordinate of the pixel whose value to set
   * @param probability the probability or count to set at that pixel
   */
  void SetValue(const pair<int, int>& coodinate, double probability);

  /**
   * @brief Increments the value of a certain pixel by a given amount
   *
   * @param coordinate the coordinate of the pixel to increment
   * @param increment the double increment value (can be negative)
   */
  void IncrementValue(const pair<int, int>& coordinate, double increment);

  /**
   * @brief Gets the grid of pixels and values
   *
   * @return a map with key <int, int> pair and value double
   */
  map<pair<int, int>, double> GetGrid() const;

  /**
   * @brief A helper method used by Boost to serialize the ImageGrid
   *
   */
  friend class access;
  template <class Archive>
  void serialize(Archive& ar, const unsigned int version) {
    ar& shade_presence_;
  }

 private:
  map<pair<int, int>, double>
      shade_presence_;  // The presence of shade or probability at a certain
                        // pixel or location
  size_t image_length_; // The height of the image
};
}  // namespace naivebayes