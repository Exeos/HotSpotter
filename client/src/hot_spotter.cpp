#include "hot_spotter.hpp"

#include <cstdint>

#include "attacher/Attacher.hpp"
#include "logger/logger.hpp"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <GLFW/glfw3.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

namespace hot_spotter {

    JavaVM* jvm = nullptr;
    JNIEnv* jniEnv = nullptr;
    jvmtiEnv* jvmTi = nullptr;

    void init() {
        if (!Logger::InitConsole()) {
            goto exit;
        }
        Logger::Log("Allocated Console");

        if (Attacher* attacher = createAttacher(); !attacher->attach(jvm, jniEnv, jvmTi)) {
            Logger::Log("Failed to attach to jvm.");
            goto exit;
        }

        Logger::LogFormat("jvm_handle: %p", reinterpret_cast<intptr_t>(jvm));
        Logger::LogFormat("jni_env: %p", reinterpret_cast<intptr_t>(jniEnv));
        Logger::LogFormat("jvm_ti: %p", reinterpret_cast<intptr_t>(jvmTi));

        if (!showWindow()) {
            Logger::Log("Failed to show imgui window");
        }

        exit:
        tidy();
    }

    bool showWindow() {
        // Setup GLFW
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return false;

        // GL 3.3 + GLSL 330
        const char* glsl_version = "#version 330";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Create window with OpenGL context
        GLFWwindow* window = glfwCreateWindow(800, 600, "HotSpotter", NULL, NULL);
        if (!window)
            return false;

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            // Your ImGui code here
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

        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();

        return true;
    }


    void tidy() {
        Logger::CloseConsole();
    }
}
