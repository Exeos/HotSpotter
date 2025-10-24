#include "Toolbar.hpp"

#include "../gui.hpp"
#include "imgui.h"
#include "../../../external/tinyfiledialogs/tinyfiledialogs.h"
#include "../../logger/logger.hpp"
#include "../../utils/io_utils.hpp"

namespace hot_spotter::gui::widget {
  void Toolbar::Render() {
    ImGui::SetCursorPos(ImVec2(0.0f, 0.0f));

    ImGui::BeginChild("Toolbar", ImVec2(0.0f, height), true, staticWindowFlags); {
      bool exportSuccess = false;

      ImGui::PushFont(fonts.bold, 0.0f);
      ImGui::Text("HotSpotter");
      ImGui::PopFont();

      ImGui::SameLine();
      if (ImGui::Button("export")) {
        exportSuccess = utils::io::exportClasses();
        ImGui::OpenPopup("ExportMessage");
      }

      if (ImGui::BeginPopupModal("ExportMessage", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text(exportSuccess ? "Exported Classes." : "Failed to Export ALL Classes.");
        ImGui::Separator();

        if (ImGui::Button("OK", ImVec2(120, 0))) {
          ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
      }

      toolbarHeight = ImGui::GetCursorPosY();
    }
    ImGui::EndChild();
  }
} // namespace hot_spotter::gui::widget
