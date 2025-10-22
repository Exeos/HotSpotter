#include "Toolbar.hpp"

#include "../gui.hpp"
#include "imgui.h"

namespace hot_spotter::gui::widget {
void Toolbar::Render() {
  ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));

  ImGui::BeginChild("Toolbar", ImVec2(0.0f, height), true, staticWindowFlags);
  {
    ImGui::PushFont(fonts.bold, 0.0f);
    ImGui::Text("HotSpotter");
    ImGui::PopFont();

    ImGui::SameLine();
    ImGui::Button("export");

    toolbarHeight = ImGui::GetCursorPosY();
  }
  ImGui::EndChild();
}

} // namespace hot_spotter::gui::widget
