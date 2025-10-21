#include "Toolbar.hpp"

#include "../gui.hpp"
#include "imgui.h"

namespace hot_spotter::gui::widget {
void Toolbar::Render() {
  ImGui::SetNextWindowPos(ImVec2(0, 0));
  ImGui::SetNextWindowSize(ImVec2(static_cast<float>(display_w), height));

  ImGui::Begin("Toolbar", nullptr, staticWindowFlags);
  {
    ImGui::PushFont(fonts.bold, 0.0f);
    ImGui::Text("HotSpotter");
    ImGui::PopFont();

    ImGui::SameLine();

    ImGui::Button("export");
    ImGui::End();
  }
}

} // namespace hot_spotter::gui::widget
