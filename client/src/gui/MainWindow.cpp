#include "MainWindow.hpp"

#include <ranges>

#include "imgui.h"
#include <GLFW/glfw3.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../globals.hpp"
#include "components/FileTree.hpp"

namespace hot_spotter::gui {

    MainWindow::MainWindow() : name_("HotSpotter") {
        // children_.emplace_back(std::make_unique<DemoWindow>());
    }

    bool MainWindow::init() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return false;

        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window with OpenGL context
        window = glfwCreateWindow(800, 600, "HotSpotter", NULL, NULL);
        if (!window)
            return false;

        glfwMakeContextCurrent(window);
        // glfwSwapInterval(1); // vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        return true;
    }

    void MainWindow::render() {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;

            // Set next window position and size to cover the entire native window
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);

            ImGui::Begin("Main", nullptr, window_flags);

            ImGui::BeginChild("Classes", ImVec2(300, 0), true);

            std::vector<std::string> fileList;
            for (const auto &key: classes | std::views::keys) {
                fileList.push_back(key);
            }

            auto fileTree = std::make_unique<FileTree>("classes", fileList);
            fileTree->render();

            ImGui::EndChild();

            ImGui::SameLine();

            ImGui::BeginChild("Info", ImVec2(0, 0), false);
            ImGui::Text("jvm_handle: %p", reinterpret_cast<intptr_t>(jvm));
            ImGui::Text("jni_env: %p", reinterpret_cast<intptr_t>(jniEnv));
            ImGui::Text("jvm_ti: %p", reinterpret_cast<intptr_t>(jvmTi));
            ImGui::EndChild();

            ImGui::End();

            // Rendering
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(0.1f, 0.12f, 0.15f, 1.00f);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
        }
    }

    void MainWindow::close() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    const std::string& MainWindow::getName() const { return name_; }

}
