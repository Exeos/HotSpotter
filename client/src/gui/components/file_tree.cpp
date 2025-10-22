#include "file_tree.hpp"

#include "imgui.h"
#include "../../logger/logger.inl"

namespace hot_spotter::gui::components {
  std::shared_ptr<FileNode> BuildFileTree(const std::vector<std::string> &paths) {
    auto root = std::make_shared<FileNode>();
    for (const auto &path: paths) {
      AddFileToTree(root, path);
    }
    return root;
  }

  void AddFileToTree(const std::shared_ptr<FileNode> &root,
                     const std::string &path) {
    auto current = root;
    size_t start = 0, end;
    while ((end = path.find('/', start)) != std::string::npos) {
      std::string part = path.substr(start, end - start);
      if (!current->children.contains(part))
        current->children[part] = std::make_shared<FileNode>(FileNode{part});
      current = current->children[part];
      start = end + 1;
    }
    // file
    std::string fileName = path.substr(start);
    current->children[fileName] =
        std::make_shared<FileNode>(FileNode{fileName, true});
  }

  void RenderFileNode(const std::shared_ptr<FileNode> &node, void (*selectCallback)(const std::string &)) {
    for (auto &[name, child]: node->children) {
      if (child->isFile) {
        if (ImGui::Selectable(name.c_str())) {
          selectCallback(name);
        }
      } else {
        if (ImGui::TreeNode(name.c_str())) {
          RenderFileNode(child, selectCallback);
          ImGui::TreePop();
        }
      }
    }
  }
} // namespace hot_spotter::gui::components
