#include "ClassView.hpp"

#include "imgui.h"

namespace hot_spotter::gui::widget {
    void ClassView::Init() {
        //  needs to call jni / jvmti functions to obtain information about the class
        // these functions should not be called in the render loop earch frame
    }

    void ClassView::Render() {
        ImGui::BeginChild("ClassView");
        ImGui::Text("hello world");
        ImGui::EndChild();
    }
}
