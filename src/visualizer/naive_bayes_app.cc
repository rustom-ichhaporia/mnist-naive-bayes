#include <visualizer/naive_bayes_app.h>
#include <core/image_grid.h>
#include <core/model.h>

namespace naivebayes {

namespace visualizer {

NaiveBayesApp::NaiveBayesApp()
    : sketchpad_(glm::vec2(kMargin, kMargin), kImageDimension,
                 kWindowSize - 2 * kMargin) {
  ci::app::setWindowSize((int) kWindowSize, (int) kWindowSize);

  model_.Load(model_path_);
}

void NaiveBayesApp::draw() {
  ci::Color8u background_color(255, 246, 148);  // light yellow
  ci::gl::clear(background_color);

  sketchpad_.Draw();

  ci::gl::drawStringCentered(
      "Press Delete to clear the sketchpad. Press Enter to make a prediction.",
      glm::vec2(kWindowSize / 2, kMargin / 2), ci::Color("black"));

  ci::gl::drawStringCentered(
      "Prediction: " + std::to_string(current_prediction_),
      glm::vec2(kWindowSize / 2, kWindowSize - kMargin / 2), ci::Color("blue"));
}

void NaiveBayesApp::mouseDown(ci::app::MouseEvent event) {
  sketchpad_.HandleBrush(event.getPos());
}

void NaiveBayesApp::mouseDrag(ci::app::MouseEvent event) {
  sketchpad_.HandleBrush(event.getPos());
}

void NaiveBayesApp::keyDown(ci::app::KeyEvent event) {
  switch (event.getCode()) {
    case ci::app::KeyEvent::KEY_RETURN: {
      // ask your classifier to classify the image that's currently drawn on the
      // sketchpad and update current_prediction_
      map<pair<size_t, size_t>, double> shades = sketchpad_.GetShades();

      ImageGrid image(kImageDimension);

      for (size_t row = 0; row < kImageDimension; ++row) {
        for (size_t col = 0; col < kImageDimension; ++col) {
          image.SetValue(pair<size_t, size_t>(row, col), shades.at(pair<size_t, size_t>(row, col)));
        }
      }

      current_prediction_ = model_.Predict(image);

      break;
    }
    case ci::app::KeyEvent::KEY_DELETE:
      sketchpad_.Clear();
      break;
  }
}

}  // namespace visualizer

}  // namespace naivebayes
