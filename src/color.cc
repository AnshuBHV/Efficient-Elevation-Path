#include "color.hpp"

#include <stdexcept>

Color::Color(int r, int g, int b): red_(r), green_(g), blue_(b) {
  // need to ensure valid color has been constructed...
  if (red_ < kColorValueMin) {
    throw std::length_error("Red_ is less than permitted minimum value.");
  }
  if (red_ > kColorValueMax) {
    throw std::length_error("Red_ is greater than permitted maximum value.");
  }
  if (green_ < kColorValueMin) {
    throw std::length_error("Green_ is less than permitted minimum value.");
  }
  if (green_ > kColorValueMax) {
    throw std::length_error("Green_ is greater than permitted maximum value.");
  }
  if (blue_ < kColorValueMin) {
    throw std::length_error("Blue_ is less than permitted minimum value.");
  }
  if (blue_ > kColorValueMax) {
    throw std::length_error("Blue_ is greater than permitted maximum value.");
  }
}

// do not modify
bool operator==(const Color& rhs, const Color& lhs) {
  return (rhs.Red() == lhs.Red() && rhs.Green() == lhs.Green() &&
          rhs.Blue() == lhs.Blue());
}