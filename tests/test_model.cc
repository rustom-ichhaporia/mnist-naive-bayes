
// #define private public
#include <core/model.h>
#include <iostream>
#include <catch2/catch.hpp>

using naivebayes::Model;
using std::cout;
using std::endl;

TEST_CASE("Model saving and loading") {
  Model model;
  size_t image_size = 2;
  model.Train("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/test/testimages", 
  "/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/test/testlabels", image_size);
  model.Save("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/test/testcache");
}

// TEST_CASE("Model prediction") {
//   SECTION("Predict valid file") {
//     Model model;
//     model.Load("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/test/testcache");
//     vector<int> results = model.Predict("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/test/testimages");
//     vector<double> expectedLikelihoodScores = ?;
//     vector<double> actualLikelihoodScores = ?;
//     for (size_t score = 0; score < model.LikelihoodScores().size(); score++) {
//       REQUIRE(expectedLikelihoodScores[score] == actualLikelihoodScores);
//     }

//     REQUIRE(results.size() == 3);
//     REQUIRE(results[0] == 0);
//     REQUIRE(results[1] == 1);
//     REQUIRE(results[2] == 2);
//   }

//   TEST_CASE("Predict invalid file") {
//     Model model;
//     REQUIRE_THROWS(model.load("something"));
//   }
// }

TEST_CASE("Images") {
  SECTION("") {

  }
}

TEST_CASE("Mathematical correctness") {
  SECTION("Predictions") {

  }
}

TEST_CASE("Accuracy") {
  Model model;
  model.Load("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/mnistdatatraining/modelcache");

  double accuracy = model.Score("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/mnistdatavalidation/testimages", "/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/mnistdatavalidation/testlabels");
  
  cout << accuracy << endl;
  REQUIRE(accuracy >= 0.5);
}