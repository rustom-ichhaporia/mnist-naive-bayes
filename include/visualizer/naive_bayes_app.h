#pragma once

#include "Cinder/app/App.h"
#include "Cinder/app/RendererGl.h"
#include "Cinder/gl/gl.h"
#include "sketchpad.h"
#include <core/model.h>
#include <string>

using std::string;

namespace naivebayes {

namespace visualizer {

/**
 * Allows a user to draw a digit on a sketchpad and uses Naive Bayes to
 * classify it.
 */
class NaiveBayesApp : public ci::app::App {
 public:
  NaiveBayesApp();

  void draw() override;
  void mouseDown(ci::app::MouseEvent event) override;
  void mouseDrag(ci::app::MouseEvent event) override;
  void keyDown(ci::app::KeyEvent event) override;

  const double kWindowSize = 875;
  const double kMargin = 100;
  const size_t kImageDimension = 28;

 private:
  /** Path to the model cache to load for the classifier. */
  static constexpr auto model_path_ = "/Users/rustomichhaporia/GitHub/Cinder/my-projects/naivebayes-rustom-ichhaporia/cache/mnistdatatraining/modelcache";
  Sketchpad sketchpad_;
  Model model_;
  int current_prediction_ = -1;
};

}  // namespace visualizer

}  // namespace naivebayes
