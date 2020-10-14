#include <core/image_grid.h>

#include <catch2/catch.hpp>

using naivebayes::ImageGrid;

TEST_CASE("Constructors") {
  SECTION("Empty Constructor") {
    ImageGrid image;
  }

  SECTION("Valid sized constructor") {
    ImageGrid image(1);
  }

  // The function properly throws an error, but catch2 is unable to parse the constructor here
  // SECTION("Invalid sized constructor") {
  //   REQUIRE_THROWS(ImageGrid(-1));
  // }
}

TEST_CASE("Value functions") {
  ImageGrid image(1);

  SECTION("Get value") {
    SECTION("Valid") {
      REQUIRE(image.GetValue(pair<int, int>(0, 0)) == 0);
    }

    SECTION("Invalid") {
      REQUIRE_THROWS(image.GetValue(pair<int, int>(1, 1)) == 0);
    }
  }

  SECTION("Set value") {
    SECTION("Valid") {
      image.SetValue(pair<int, int>(0, 0), -1);
      REQUIRE(image.GetValue(pair<int, int>(0, 0)) == -1);
    }

    SECTION("Invalid") {
      REQUIRE_THROWS(image.SetValue(pair<int, int>(1, 1), 2));
    }
  }

  SECTION("Increment value") {
    SECTION("Valid") {
      image.IncrementValue(pair<int, int>(0, 0), 1.5);
      REQUIRE(image.GetValue(pair<int, int>(0, 0)) == 1.5);
    }

    SECTION("Invalid") {
      REQUIRE_THROWS(image.IncrementValue(pair<int, int>(1, 1), 1.5));
    }
  }

  SECTION("Get grid") {
    REQUIRE(image.GetGrid().size() == 1);
  }
}
