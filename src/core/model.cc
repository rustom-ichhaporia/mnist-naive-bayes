#include "core/model.h"
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <utility>

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::ifstream;

using std::map;
namespace naivebayes {


void Model::Train(const string& image_path, const string& label_path, size_t image_length) {
  image_length_ = image_length;

  ifstream image_stream(image_path);
  ifstream label_stream(label_path);

  // image_stream >> image_cells_;

  

  // label_stream >> labels_;

}

void Model::Save(const string& save_path) {

}

void Model::Load(const string& load_path) {
    
}



}  // namespace naivebayes