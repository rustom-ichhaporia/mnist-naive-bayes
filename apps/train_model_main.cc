#include <core/arg_handler.h>
#include <core/file_handler.h>

#include <boost/program_options.hpp>
#include <iostream>
#include <sstream>

using boost::program_options::notify;
using boost::program_options::options_description;
using boost::program_options::value;
using boost::program_options::variables_map;
using std::cerr;
using std::cout;
using std::clog;
using std::endl;
using std::invalid_argument;
using std::logic_error;
using std::string;
using naivebayes::ArgHandler;
using std::ifstream;
using std::getline;

int main(int argc, char* argv[]) {  
  ArgHandler arg_handler;

  arg_handler.EvaluateArguments(argc, argv);

  return 0;
}
