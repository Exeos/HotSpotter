#include "ClassList.hpp"

#include "../../globals.hpp"
#include "../../hooks/hooks.hpp"
#include "../components/file_tree.hpp"
#include "../gui.hpp"
#include "imgui.h"

#include <ranges>

#include "../../logger/logger.hpp"

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

    ImGui::BeginChild("Classes", ImVec2(width, 0), true); {
      ImGui::Text("Classes (%d):", static_cast<int>(classes.size()));

      RenderFileNode(classTree, OnClassSelected);
    }
    ImGui::EndChild();
  }

  void ClassList::OnClassSelected(const std::string &name) {
    selectedClass = std::make_pair(name, classes[name].first);
  }

  void ClassList::AddClassToTree(const std::string &name) {
    if (!classTree) {
      classTree = std::make_shared<components::FileNode>();
    }
    AddFileToTree(classTree, name);
  }
} // namespace hot_spotter::gui::widget
