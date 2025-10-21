#include "file_tree.hpp"

#include "imgui.h"

namespace hot_spotter::gui::components {
    std::shared_ptr<FileNode> BuildFileTree(const std::vector<std::string> &paths) {
        auto root = std::make_shared<FileNode>();
        for (const auto &path: paths) {
            auto current = root;
            size_t start = 0, end;
            while ((end = path.find('/', start)) != std::string::npos) {
                std::string part = path.substr(start, end - start);
                if (!current->children.count(part))
                    current->children[part] = std::make_shared<FileNode>(FileNode{part});
                current = current->children[part];
                start = end + 1;
            }
            // file
            std::string fileName = path.substr(start);
            current->children[fileName] = std::make_shared<FileNode>(FileNode{fileName, true});
        }
        return root;
    }

    void RenderFileNode(const std::shared_ptr<FileNode> &node) {
        for (auto &[name, child]: node->children) {
            if (child->isFile) {
                ImGui::BulletText("%s", name.c_str());
            } else {
                if (ImGui::TreeNode(name.c_str())) {
                    RenderFileNode(child);
                    ImGui::TreePop();
                }
            }
        }
    }
}

