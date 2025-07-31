// AI shit code
// TODO replace this in the future

#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <imgui.h>
#include "../Window.hpp" // Your base class

namespace hot_spotter::gui {

class FileTree : public Window {
public:
    FileTree(const std::string& name, const std::vector<std::string>& fileList)
        : name_(name), fileList_(fileList), built_(false), root_(true) {}

    bool init() override {
        // You could rebuild the tree here if needed
        return true;
    }
    void close() override {
        // Clean up resources if needed
    }
    void render() override {
        if (!built_) {
            buildTree();
            built_ = true;
        }
        drawTree(root_);
    }
    const std::string& getName() const override {
        return name_;
    }

private:
    struct FileNode {
        bool isDir;
        std::map<std::string, std::unique_ptr<FileNode>> children;
        FileNode(bool dir = false) : isDir(dir) {}
    };

    void buildTree() {
        for (const auto& path : fileList_) {
            insertPath(root_, path);
        }
    }

    void insertPath(FileNode& root, const std::string& path) {
        size_t start = 0, end;
        FileNode* node = &root;
        while ((end = path.find('/', start)) != std::string::npos) {
            std::string part = path.substr(start, end - start);
            start = end + 1;
            if (!part.empty()) {
                if (!node->children.count(part))
                    node->children[part] = std::make_unique<FileNode>(true);
                node = node->children[part].get();
            }
        }
        // Handle file
        std::string last = path.substr(start);
        if (!last.empty() && !node->children.count(last)) {
            node->children[last] = std::make_unique<FileNode>(false);
        }
    }

    void drawTree(const FileNode& node) const {
        for (const auto& [key, child] : node.children) {
            if (child->isDir) {
                if (ImGui::TreeNode(key.c_str())) {
                    drawTree(*child);
                    ImGui::TreePop();
                }
            } else {
                ImGui::BulletText("%s", key.c_str());
            }
        }
    }

    std::string name_;
    std::vector<std::string> fileList_;
    bool built_;
    FileNode root_;
};

} // namespace hot_spotter::gui