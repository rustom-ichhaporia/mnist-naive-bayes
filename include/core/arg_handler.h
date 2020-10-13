#pragma once
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

using boost::program_options::options_description;
using boost::program_options::variables_map;
using std::string;
using std::vector;

namespace naivebayes {

class ArgHandler {
 public:
  /**
   * @brief Evaluates the command line arguments that are passed to the program,
   *        throwing an error for invalid arguments and executing valid ones.
   *
   * @param argc the number of arguments passed
   * @param argv the array of arguments
   */
  void EvaluateArguments(const int& argc, char* argv[]);

 private:
 /**
  * @brief Executes the train, save, and load arguments after being validated. 
  * 
  * @param vmap the variables_map object containing informaiton about the arguments
  */
  void ExecuteArguments(const variables_map& vmap);

  /**
   * @brief Get the OptionsDescription object for the program options
   * 
   * @return options_description object with documentation about options
   */
  options_description GetOptionsDescription();

  /**
   * @brief Checks if two conflicting options exist in the user input and raises an exception if so. 
   * 
   * @param vm the variables_map object containing information about the user input
   * @param opt1 the first option to check
   * @param opt2 the second option to check
   */
  void ConflictingOptions(const variables_map& vm, const char* opt1, const char* opt2);

  vector<string> train_paths_;  // The paths to training images and labels
  string save_path_;            // The path to save the model cache
  string load_path_;  // The path to load a previously trained model cache
  string predict_path_;
  size_t image_height_; // The height in rows of the images passed in through a file
};

}  // namespace naivebayes