#pragma once
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace hot_spotter::gui::components {
    struct FileNode {
        std::string name;
        bool isFile = false;
        std::map<std::string, std::shared_ptr<FileNode> > children;
    };

    std::shared_ptr<FileNode> BuildFileTree(const std::vector<std::string> &paths);

    void AddFileToTree(const std::shared_ptr<FileNode> &root,
                       const std::string &path);

    void RenderFileNode(const std::shared_ptr<FileNode> &node);
} // namespace hot_spotter::gui::components
