#include <core/arg_handler.h>
#include <core/model.h>

#include <algorithm>
#include <boost/program_options.hpp>
#include <iostream>
#include <string>

using boost::program_options::notify;
using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using std::cerr;
using std::clog;
using std::cout;
using std::endl;
using std::exit;
using std::invalid_argument;
using std::logic_error;
using std::string;
using naivebayes::Model;

namespace naivebayes {

void ArgHandler::EvaluateArguments(const int& argc, char* argv[]) {
  options_description description = GetOptionsDescription();

  // Parse the command line input variables and update the local variables
  variables_map vmap;
  store(parse_command_line(argc, argv, description), vmap); 
  notify(vmap);

  // Raise errors if the user attemtps to load while also training or saving
  ConflictingOptions(vmap, "train", "load");
  ConflictingOptions(vmap, "save", "load");

  if (vmap.count("help")) { // Display option information if requested
    cout << description;
  } else {
    ExecuteArguments(vmap); // Executes the arguments that were passed and validated
  }
}

void ArgHandler::ExecuteArguments(const variables_map& vmap) {
  Model model;
  if (vmap.count("train") && vmap.count("save")) {
    // Trains the model and then saves it with the given paths
    model.Train(train_paths_[0], train_paths_[1], image_height_);
    model.Save(save_path_);
  } else if (vmap.count("load")) {
    // Loads the model from a given cache file
    model.Load(load_path_);
  } 
}

void ArgHandler::ConflictingOptions(const variables_map& vmap, const char* opt1, const char* opt2) {
  // Specifies the conflicting options if they both exist in the variable map
  if (vmap.count(opt1) && !vmap[opt1].defaulted() && vmap.count(opt2) && !vmap[opt2].defaulted())
    throw logic_error(string("Conflicting options: \"") + opt1 + "\" and \"" + opt2 + "\".");
}

options_description ArgHandler::GetOptionsDescription() {
  options_description description("Allowed options"); // Create an object with matching options
  description.add_options()
      ("help,h", "Prints the descriptions for the command line arguments.")
      ("train,t", value<vector<string>>(&train_paths_)->multitoken(),
          "Trains the model. Takes two file paths,  first train images then train labels.")
      ("save,s", value(&save_path_), 
          "Saves the model. Takes one file path, the location for the model cache to be saved.")
      ("load,l", value(&load_path_), 
          "Loads the model from a given model cache file.")
      ("image-height,i", value(&image_height_), 
          "Sets the height of each image in the training file.");
  return description;
}
}  // namespace naivebayes