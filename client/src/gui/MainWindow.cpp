#include "MainWindow.hpp"

#include <ranges>

#include "imgui.h"
#include <GLFW/glfw3.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "../globals.hpp"
#include "../utils/io_utils.hpp"
#include "../utils/java_utils.hpp"
#include "components/FileTree.hpp"

namespace hot_spotter::gui {
    MainWindow::MainWindow() : Window(nullptr), name_("HotSpotter") {
    }

    bool MainWindow::init() {
        glfwSetErrorCallback(glfw_error_callback);
        if (!glfwInit())
            return false;

        const char *glsl_version = "#version 330";
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
        ImGuiIO &io = ImGui::GetIO();
        (void) io;
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        return true;
    }

    void MainWindow::render() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings;
            ImGui::SetNextWindowPos(ImVec2(0, 0));
            ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
            ImGui::Begin("Main", nullptr, window_flags);

            renderToolbar();

            // Render file tree
            renderFileTree();
            ImGui::SameLine();

            // Render info panel
            renderInfoPanel();
            ImGui::End(); // Main

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

    void MainWindow::renderToolbar() {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.2f, 0.25f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.35f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15f, 0.15f, 0.2f, 1.0f));
        ImGui::BeginChild("##toolbar", ImVec2(0, ImGui::GetFrameHeight() + 10),
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        if (ImGui::Button("Export", ImVec2(100, 0))) {
            utils::io::exportClasses();
        }

        // Optionally add a tooltip when hovering over the button
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Export all intercepted classes to disk.");
        }

        ImGui::EndChild(); // toolbar
        ImGui::Spacing();
        ImGui::PopStyleColor(3);
    }

    void MainWindow::renderFileTree() {
        ImGui::BeginChild("Classes", ImVec2(300, 0), true);
        std::vector<std::string> fileList;
        for (const auto &key: classes | std::views::keys) {
            fileList.push_back(key);
        }
        auto fileTree = std::make_unique<FileTree>(this, "classes", fileList);
        fileTree->render();
        ImGui::EndChild(); // Classes
    }

    void MainWindow::renderInfoPanel() {
        ImGui::BeginChild("Info", ImVec2(0, 0), false);
        ImGui::Text("jvm_handle: %p", reinterpret_cast<intptr_t>(jvm));
        ImGui::Text("jni_env: %p", reinterpret_cast<intptr_t>(jniEnv));
        ImGui::Text("jvm_ti: %p", reinterpret_cast<intptr_t>(jvmTi));
        ImGui::Spacing();
        ImGui::Spacing();
        if (selectedClass.has_value()) {
            renderClassDetails();
        }
        ImGui::EndChild(); // Info
    }

    void MainWindow::renderClassDetails() {
        ImGui::BeginChild("Class", ImVec2(0, 0), false);
        ImGui::Text("Class: %s", selectedClass.value().first.c_str());
        jclass selected = selectedClass.value().second;
        jint methodCount;
        jmethodID *classMethods;
        if (jvmTi->GetClassMethods(selected, &methodCount, &classMethods) != JVMTI_ERROR_NONE) {
            ImGui::Text("Failed to obtained methods from class");
        } else {
            renderClassMethods(methodCount, classMethods);
            jvmTi->Deallocate(reinterpret_cast<unsigned char *>(classMethods));
        }
        ImGui::EndChild(); // Class
    }

    void MainWindow::renderClassMethods(jint methodCount, jmethodID *classMethods) {
        ImGui::BeginChild("ClassMethods");
        ImGui::Text("Methods (%d):", methodCount);
        for (int i = 0; i < methodCount; i++) {
            renderMethodCard(classMethods[i], i);
        }
        ImGui::EndChild(); // ClassMethods
    }

    void MainWindow::renderMethodCard(jmethodID methodId, int index) {
        char *methodName = nullptr;
        char *methodSignature = nullptr;
        char *methodGenericSignature = nullptr;
        jint methodModifiers = -1;

        jvmtiError error = jvmTi->GetMethodName(methodId, &methodName, &methodSignature, &methodGenericSignature);
        if (error != JVMTI_ERROR_NONE) {
            ImGui::Text("Failed to get method information. Error %d", error);
            return;
        }

        error = jvmTi->GetMethodModifiers(methodId, &methodModifiers);
        if (error != JVMTI_ERROR_NONE) {
            methodModifiers = -1;
            logger::LogFormat("Failed to get method modifiers. Error: %d", error);
        }

        std::vector<std::string> methodModifiersVector = utils::java::parseJavaModifiers(methodModifiers);

        char titleBuffer[512];
        if (methodSignature) {
            snprintf(titleBuffer, sizeof(titleBuffer), "%s%s", methodName, methodSignature);
        } else {
            snprintf(titleBuffer, sizeof(titleBuffer), "%s", methodName);
        }

        ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(0.15f, 0.15f, 0.15f, 1.0f));
        if (ImGui::CollapsingHeader(titleBuffer)) {
            ImGui::Indent(10.0f);
            if (methodSignature) {
                ImGui::Text("Signature: %s", methodSignature);
            }
            if (methodGenericSignature) {
                ImGui::Text("Signature Generic: %s", methodGenericSignature);
            }

            if (methodModifiers != -1) {
                char modifiersHeaderId[32];
                snprintf(modifiersHeaderId, sizeof(modifiersHeaderId), "Modifiers##Mod%d", index);

                if (ImGui::CollapsingHeader(modifiersHeaderId)) {
                    ImGui::Indent(10.0f);

                    for (const auto &i: methodModifiersVector) {
                        ImGui::Text(i.c_str());
                    }

                    ImGui::Unindent(10.0f);
                }
            }

            ImGui::Unindent(10.0f);
        }
        ImGui::PopStyleColor();

        // Clean up
        if (methodName) {
            jvmTi->Deallocate(reinterpret_cast<unsigned char *>(methodName));
        }
        if (methodSignature) {
            jvmTi->Deallocate(reinterpret_cast<unsigned char *>(methodSignature));
        }
        if (methodGenericSignature) {
            jvmTi->Deallocate(reinterpret_cast<unsigned char *>(methodGenericSignature));
        }
    }

    void MainWindow::close() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
    }

    const std::string &MainWindow::getName() const {
        return name_;
    }
}
