#include <core/model.h>

#include <catch2/catch.hpp>

using naivebayes::Model;
using std::ifstream;

TEST_CASE("Model saving and loading") {
  Model model;
  size_t image_size = 2;
  model.Train("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/test/testimages", 
  "/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/test/testlabels", image_size);
  model.Save("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/test/testcache");
}

TEST_CASE("Model prediction") {
  SECTION("Predict valid file") {
    Model model;
    model.Load("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/test/testcache");
    vector<int> results = model.Predict("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/test/testimages");
    REQUIRE(results.size() == 3);
    REQUIRE(results[0] == 0);
    REQUIRE(results[1] == 1);
    REQUIRE(results[2] == 2);
  }
}