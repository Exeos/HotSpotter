#include "ClassList.hpp"

#include "../../globals.hpp"
#include "../components/file_tree.hpp"
#include "../gui.hpp"
#include "imgui.h"

#include <ranges>
#include <string>
#include <vector>

namespace hot_spotter::gui::widget {
void ClassList::Render() {
  ImGui::SetNextWindowPos(ImVec2(0, startY));
  ImGui::SetNextWindowSize(
      ImVec2(width, static_cast<float>(display_h) - startY), ImGuiCond_Once);
  ImGui::SetNextWindowSizeConstraints(
      ImVec2(width, static_cast<float>(display_h)),
      ImVec2(static_cast<float>(display_w), static_cast<float>(display_h)));

  ImGui::Begin("Classes", nullptr,
               ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
  {
    std::vector<std::string> classFiles;
    classFiles.reserve(classes.size());
    for (const auto &key : classes | std::views::keys) {
      classFiles.push_back(key);
    }
    auto tree = components::BuildFileTree(classFiles);

    RenderFileNode(tree);
    ImGui::End();
  }
}

} // namespace hot_spotter::gui::widget
