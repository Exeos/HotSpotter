#pragma once

namespace hot_spotter::gui::widget {
class Widget {
public:
  virtual ~Widget() = default;

  virtual void Init() {}
  virtual void Render() = 0;
};
} // namespace hot_spotter::gui::widget
