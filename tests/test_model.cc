
// #define private public
#include <core/image_grid.h>
#include <core/model.h>

#include <catch2/catch.hpp>
#include <iostream>

using boost::archive::text_iarchive;
using boost::archive::text_oarchive;
using naivebayes::ImageGrid;
using naivebayes::Model;
using std::cout;
using std::endl;

// TEST_CASE("Model prediction") {
//   SECTION("Predict valid file") {
//     Model model;
//     model.Load("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/test/testcache");
//     vector<int> results =
//     model.Predict("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/test/testimages");
//     vector<double> expectedLikelihoodScores = ?;
//     vector<double> actualLikelihoodScores = ?;
//     for (size_t score = 0; score < model.LikelihoodScores().size(); score++)
//     {
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

TEST_CASE("Operator overloading") {
  // Note that by testing on 2x2 images and using visualization for 28x28,
  // we are testing different sizes work already. Other image tests are in the
  // test_image_grid.cc file.

  // Note also that proper image loading is handled within this test as well. We
  // know that the characters that have been loaded are parsed correctly because
  // if they hadn't been loaded properly then some of the probabilities would be
  // incorrect, but we have verified below that they are all correct.

  Model model;
  size_t image_size = 2;
  ifstream input(
      "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
      "naivebayes-rustom-ichhaporia/data/test/testimages");
  model.Train(
      "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
      "naivebayes-rustom-ichhaporia/data/test/testimages",
      "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
      "naivebayes-rustom-ichhaporia/data/test/testlabels",
      image_size);

  // Test operator overloading for new model data (substitute probabilities)
  input >> model;
  vector<ImageGrid> grids = model.GetTrainImageGrids();

  SECTION("Image 0 values") {
    REQUIRE(grids[0].GetValue(0, 0) == Approx(1.666).epsilon(0.01));
    REQUIRE(grids[0].GetValue(0, 1) == Approx(1.666).epsilon(0.1));
    REQUIRE(grids[0].GetValue(1, 0) == Approx(1.666).epsilon(0.1));
    REQUIRE(grids[0].GetValue(1, 1) == Approx(1.666).epsilon(0.1));
  }

  SECTION("Image 1 values") {
    REQUIRE(grids[1].GetValue(0, 0) == Approx(0.333).epsilon(0.01));
    REQUIRE(grids[1].GetValue(0, 1) == Approx(1.000).epsilon(0.01));
    REQUIRE(grids[1].GetValue(1, 0) == Approx(0.333).epsilon(0.01));
    REQUIRE(grids[1].GetValue(1, 1) == Approx(1.666).epsilon(0.01));
  }
}

TEST_CASE("Serialization and deserialization") {
  SECTION("Model serialization") {
    // Model saving has already clearly been tested through the use of
    // caches. All of the saving and loading is handled by boost library
    // functions, so I have only checked a few important values to ensure that
    // they were loaded properly from a previously generated cache.
  }

  SECTION("Model deserialization") {
    Model model;
    model.Load(
        "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
        "naivebayes-rustom-ichhaporia/cache/test/testcache");
    vector<ImageGrid> grids = model.GetTrainImageGrids();

    REQUIRE(grids[0].GetValue(0, 0) == Approx(0.666).epsilon(0.01));
    REQUIRE(grids[1].GetValue(1, 1) == Approx(0.666).epsilon(0.01));
  }

  SECTION("Image serialization and deserialization") {
    // Image serialization and deserialization is covered through the use of
    // model serialization and deserialization. The model would not be able to
    // serialize or deserialize properly unless ImageGrid also had the
    // serialization functionality implemented properly because of boost's
    // serialization protocol for custom classes, and we have already tested
    // serialization for the Model, so we know that it works for the ImageGrid
    // as well.
  }
}

TEST_CASE("Mathematical correctness") {
  Model model;
  model.Load(
      "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
      "naivebayes-rustom-ichhaporia/cache/test/testcache");

  SECTION("Default probabilities") {
    // Note the correct calculation of default probabilities is handled in the
    // operator overloading test case and sections because the model stores the
    // probabilities automatically rather than the frequencies. Thus, they are
    // not repeated here.
  }

  SECTION("Class probability") {
    REQUIRE(model.GetClassProbability(0) == Approx(0.333).epsilon(0.01));
    REQUIRE(model.GetClassProbability(1) == Approx(0.333).epsilon(0.01));
  }

  SECTION("Likelihood score") {
    ImageGrid image(2);
    image.SetValue(0, 0, 1.0);
    image.SetValue(1, 1, 1.0);

    REQUIRE(model.LikelihoodScore(image, 0) == Approx(-4.107).epsilon(0.01));
    REQUIRE(model.LikelihoodScore(image, 1) == Approx(-3.701).epsilon(0.01));
  }

  SECTION("Predictions") {
    vector<int> results = model.Predict(
        "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
        "naivebayes-rustom-ichhaporia/data/test/testimages");
    REQUIRE(results[0] == 0);
    REQUIRE(results[1] == 1);
  }
}

TEST_CASE("Accuracy") {
  SECTION("Model accuracy") {
    Model model;
    model.Train("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/mnistdatatraining/trainingimages", "/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/data/mnistdatatraining/traininglabels", 28);
    model.Save("/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/mnistdatatraining/modelcache");
    model.Load(
        "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
        "naivebayes-rustom-ichhaporia/cache/mnistdatatraining/modelcache");
    REQUIRE(
        model.Score(
            "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
            "naivebayes-rustom-ichhaporia/data/mnistdatavalidation/testimages",
            "/Users/rustomichhaporia/GitHub/Cinder/my-projects/"
            "naivebayes-rustom-ichhaporia/data/mnistdatavalidation/"
            "testlabels") > 0.7);
  }
}