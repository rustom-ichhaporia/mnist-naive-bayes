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
namespace naivebayes {

void ArgHandler::EvaluateArguments(const int& argc, char* argv[]) {
  vector<string> train_paths;
  string model_save_path;
  string load_path;

  options_description desc("Allowed options");
  desc.add_options()
      ("help,h", "Prints the descriptions for the command line arguments.")
      ("train,t", value<vector<string>>(&train_paths)->multitoken(),
          "Trains the model. Takes two file paths,  first train images then train labels.")
      ("save,s", value(&model_save_path), 
          "Saves the model. Takes one file path, the location for the model cache to be saved.")
      ("load,l", value(&load_path), 
          "Loads the model from a given model cache file.");

  variables_map vmap;
  store(parse_command_line(argc, argv, desc), vmap);
  notify(vmap);

  ConflictingOptions(vmap, "train", "load");
  ConflictingOptions(vmap, "save", "load");

  if (vmap.count("help")) {
    cout << desc;
  } else {
    ExecuteArguments(vmap);
  }
}

void ArgHandler::ExecuteArguments(const variables_map& vmap) {
  if (vmap.count("train") && vmap.count("save")) {

  } else if (vmap.count("load")) {
    
  } 
}


void ArgHandler::ConflictingOptions(const variables_map& vm, const char* opt1, const char* opt2) {
  if (vm.count(opt1) && !vm[opt1].defaulted() && vm.count(opt2) && !vm[opt2].defaulted())
    throw logic_error(string("Conflicting options '") + opt1 + "' and '" + opt2 + "'.");
}

template <class Container> 
const bool Contains(const Container& container, const typename Container::value_type& element) 
{
  return std::find(container.begin(), container.end(), element) != container.end();
}

}  // namespace naivebayes