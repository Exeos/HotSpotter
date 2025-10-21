#pragma once
#include "Widget.hpp"

namespace hot_spotter::gui::widget {
class ClassList final : public Widget {
public:
  ClassList(float startY) : startY(startY) {}
  void Render() override;

private:
  float startY;
  int width = 200;
};
} // namespace hot_spotter::gui::widget
