#pragma once
#include "Widget.hpp"

namespace hot_spotter::gui::widget {
class Toolbar final : public Widget {
public:
  Toolbar(float height) : height(height) {}
  void Render() override;

private:
  float height;
};
} // namespace hot_spotter::gui::widget
