#pragma once
#include "Window.hpp"
#include <memory>
#include <optional>
#include <string>

#include "jni.h"
#include "GLFW/glfw3.h"

namespace hot_spotter::gui {
    class MainWindow : public Window {
    public:
        MainWindow();

        bool init() override;

        void render() override;

        void close() override;

        const std::string &getName() const override;

        void renderToolbar();

        void renderFileTree();

        void renderInfoPanel();

        void renderClassDetails();

        void renderClassMethods(jint methodCount, jmethodID *classMethods);

        void renderMethodCard(jmethodID methodId, int index);

        std::optional<std::pair<std::string, jclass> > selectedClass;

    private:
        std::string name_;
        GLFWwindow *window;
    };
}
