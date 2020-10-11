#pragma once
#include <boost/program_options.hpp>
#include <iostream>
#include <string>
#include <vector>

using boost::program_options::variables_map;
using std::string;
using std::vector;

namespace naivebayes {

class ArgHandler {
 public:
  void EvaluateArguments(const int& argc, char* argv[]);

 private:
  void ExecuteArguments(const variables_map& vmap);

  void ConflictingOptions(const variables_map& vm, const char* opt1,
                          const char* opt2);

  template <class Container>
  bool Contains(const Container& container,
                const typename Container::value_type& element);
};

}  // namespace naivebayes