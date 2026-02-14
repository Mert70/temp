#include <Pages/DebugInfo.hxx>

namespace basic
{
    void DebugInfo::draw()
    {
        ImGui::Begin("Debug Info");
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }
}