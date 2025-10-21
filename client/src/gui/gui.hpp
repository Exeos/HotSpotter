#pragma once
#include "GLFW/glfw3.h"
#include "imgui.h"

namespace hot_spotter::gui {
struct AppFonts {
  ImFont *regular;
  ImFont *semiBold;
  ImFont *bold;
};
extern AppFonts fonts;
extern GLFWwindow *window;

bool init();

void render();

void cleanup();

static void glfw_error_callback(int error, const char *description);
} // namespace hot_spotter::gui
