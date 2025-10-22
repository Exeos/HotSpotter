#include "ClassList.hpp"

#include "../../globals.hpp"
#include "../../hooks/hooks.hpp"
#include "../components/file_tree.hpp"
#include "../gui.hpp"
#include "imgui.h"

#include <ranges>

namespace hot_spotter::gui::widget {
  void ClassList::Init() {
    std::vector<std::string> classFiles;
    classFiles.reserve(classes.size());
    for (const auto &key: classes | std::views::keys) {
      classFiles.push_back(key);
    }
    classTree = components::BuildFileTree(classFiles);
  }

  void ClassList::Render() {
    // Drain any classes intercepted by the JVMTI hook and add them to the tree.
    auto pending = hooks::DrainPendingClasses();
    for (const auto &name: pending) {
      AddClassToTree(name);
    }

    ImGui::SetNextWindowPos(ImVec2(0, toolbarHeight));
    ImGui::SetNextWindowSize(
      ImVec2(width, static_cast<float>(display_h) - toolbarHeight),
      ImGuiCond_Once);
    ImGui::SetNextWindowSizeConstraints(
      ImVec2(width, static_cast<float>(display_h)),
      ImVec2(static_cast<float>(display_w), static_cast<float>(display_h)));

    ImGui::Begin("Classes", nullptr,
                 ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar); {
      RenderFileNode(classTree);
    }
    ImGui::End();
  }

  void ClassList::AddClassToTree(const std::string &name) {
    if (!classTree) {
      classTree = std::make_shared<components::FileNode>();
    }
    AddFileToTree(classTree, name);
  }
} // namespace hot_spotter::gui::widget
