#pragma once
#include "../components/file_tree.hpp"
#include "Widget.hpp"

namespace hot_spotter::gui::widget {
  class ClassList final : public Widget {
  public:
    explicit ClassList() = default;

    void Init() override;

    void Render() override;

    static void OnClassSelected(const std::string &name);

    void AddClassToTree(const std::string &name);

  private:
    float width = 200.0f;
    std::shared_ptr<components::FileNode> classTree;
  };
} // namespace hot_spotter::gui::widget
