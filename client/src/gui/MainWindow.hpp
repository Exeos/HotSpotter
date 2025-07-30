#pragma once
#include "Window.hpp"
#include <memory>
#include <string>

#include "GLFW/glfw3.h"

namespace hot_spotter::gui {

    class MainWindow : public Window {
    public:
        MainWindow();

        bool init() override;
        void render() override;
        void close() override;
        const std::string& getName() const override;

    private:
        std::string name_;
        GLFWwindow* window;
    };

}
