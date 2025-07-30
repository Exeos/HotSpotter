#include "MainWindow.hpp"
#include "imgui.h"
#include <GLFW/glfw3.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../hot_spotter.hpp"

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

            ImGui::Begin("Data");
            ImGui::Text("jvm_handle: %p", reinterpret_cast<intptr_t>(jvm));
            ImGui::Text("jni_env: %p", reinterpret_cast<intptr_t>(jniEnv));
            ImGui::Text("jvm_ti: %p", reinterpret_cast<intptr_t>(jvmTi));
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
