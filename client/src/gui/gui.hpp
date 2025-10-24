#pragma once
#include <optional>

#include "GLFW/glfw3.h"
#include "imgui.h"
#include "jni.h"
#include "widgets/ClassList.hpp"
#include "widgets/ClassView.hpp"
#include "widgets/Toolbar.hpp"

struct AppFonts {
    ImFont *regular;
    ImFont *semiBold;
    ImFont *bold;
};

namespace hot_spotter::gui {
    inline GLFWwindow *window = nullptr;
    inline AppFonts fonts;

    inline ImGuiWindowFlags staticWindowFlags =
            ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

    inline int display_w = 0;
    inline int display_h = 0;

    inline float toolbarHeight = 0.0f;
    inline float classListWidth = 0.0f;

    inline widget::Toolbar *widget_Toolbar = new widget::Toolbar(30);
    inline widget::ClassList *widget_ClassList = new widget::ClassList();
    inline widget::ClassView *widget_ClassView = nullptr;

    bool init();

    void render();

    void cleanup();

    static void glfw_error_callback(int error, const char *description);
} // namespace hot_spotter::gui
