#include "gui.hpp"

#include <ranges>

#include "../../external/fonts/roboto_bold.h"
#include "../../external/fonts/roboto_regular.h"
#include "../../external/fonts/roboto_semi_bold.h"
#include "../globals.hpp"
#include "../logger/logger.inl"
#include "GLFW/glfw3.h"
#include "components/file_tree.hpp"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"

namespace hot_spotter::gui {
GLFWwindow *window = nullptr;
AppFonts fonts;

bool init() {
  glfwSetErrorCallback(glfw_error_callback);
  if (!glfwInit()) {
    return false;
  }

#if defined(__APPLE__)
  // GL 3.2 + GLSL 150
  const char *glsl_version = "#version 150";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#else
  // GL 3.0 + GLSL 130
  const char *glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

  // float main_scale =
  //     ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());
  float main_scale = 1.0f;

  window = glfwCreateWindow(static_cast<int>(800 * main_scale),
                            static_cast<int>(600 * main_scale), "HotSpotter",
                            nullptr, nullptr);

  if (window == nullptr) {
    return false;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsClassic();

  // Setup scaling
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(main_scale);
  style.FontScaleDpi = main_scale;

  // Setup Platform/Renderer backends
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);

  ImFont *regular = io.Fonts->AddFontFromMemoryTTF(
      Roboto_Regular_ttf, Roboto_Regular_ttf_len, 16.0f);
  ImFont *semiBold = io.Fonts->AddFontFromMemoryTTF(
      Roboto_SemiBold_ttf, Roboto_SemiBold_ttf_len, 16.0f);
  ImFont *bold = io.Fonts->AddFontFromMemoryTTF(Roboto_Bold_ttf,
                                                Roboto_Bold_ttf_len, 16.0f);

  if (!regular || !semiBold || !bold) {
    return false;
  }

  fonts.regular = regular;
  fonts.semiBold = semiBold;
  fonts.bold = bold;

  return true;
}

void render() {
  ImGuiWindowFlags mainWindowFlags =
      ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
      ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
      ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBringToFrontOnFocus;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
      ImGui_ImplGlfw_Sleep(10);
      continue;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);

    ImGui::NewFrame();
    {
      ImGui::PushFont(fonts.regular, 0.0f);
      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(
          ImVec2(static_cast<float>(display_w), static_cast<float>(display_h)));

      ImGui::Begin("HotSpotter", nullptr, mainWindowFlags);

      ImGui::SetNextWindowPos(ImVec2(0, 0));
      ImGui::SetNextWindowSize(ImVec2(200, static_cast<float>(display_h)),
                               ImGuiCond_Once);
      ImGui::SetNextWindowSizeConstraints(
          ImVec2(200, static_cast<float>(display_h)),
          ImVec2(static_cast<float>(display_w), static_cast<float>(display_h)));

      ImGui::Begin("Classes", nullptr,
                   ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
      {
        std::vector<std::string> classFiles;
        classFiles.reserve(classes.size());
        for (const auto &key : classes | std::views::keys) {
          classFiles.push_back(key);
        }
        auto tree = components::BuildFileTree(classFiles);

        RenderFileNode(tree);
        ImGui::End();
      }

      ImGui::End();
      ImGui::PopFont();
    }

    // Rendering
    ImGui::Render();
    glViewport(0, 0, display_w, display_h);
    glClearColor(1, 1, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
  }
}

void cleanup() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void glfw_error_callback(int error, const char *description) {
  logger::LogFormat("GLFW Error %d: %s\n", error, description);
}
} // namespace hot_spotter::gui
