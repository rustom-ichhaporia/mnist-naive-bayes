#include <core/arg_handler.h>
#include <core/model.h>

#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>

using boost::program_options::notify;
using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using std::cout;
using std::endl;
using std::invalid_argument;
using std::logic_error;
using std::string;
using naivebayes::ArgHandler;
using std::ifstream;
using std::getline;
using naivebayes::Model;

int main(int argc, char* argv[]) {  

  // Model model;
  // model.Load("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/mnistdatatraining/modelcache");

  // double accuracy = model.Score("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/mnistdatavalidation/testimages", "/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/mnistdatavalidation/testlabels");
  
  // cout << accuracy << endl;

  ArgHandler arg_handler;

  // ArgHandler evaluates arguments for legality and then executes them
  arg_handler.EvaluateArguments(argc, argv);

  return 0;
}
