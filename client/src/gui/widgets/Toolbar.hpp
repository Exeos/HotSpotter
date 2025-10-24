#pragma once
#include "Widget.hpp"

namespace hot_spotter::gui::widget {
class Toolbar final : public Widget {
public:
  explicit Toolbar(const float height) : height(height) {}
  void Render() override;

private:
  float height;
};
} // namespace hot_spotter::gui::widget
