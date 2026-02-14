#include <TitleBar.hxx>
#include <iostream>

namespace basic
{
    bool TitleBar::create(Window *_window)
    {
        destroy();
        m_window = _window;


        if (!m_video.create(_window, "Camera", 800, 600))
        {
            std::cerr << "Couldn't created the video page!\n";
            exit(EXIT_FAILURE);
        }
        m_video.setPosition(50, 50);
        m_video.updateFrame(testFrame, 1280 * 720, 1280);

        if (!m_map.create(_window, "Map", 600, 600))
        {
            std::cerr << "Couldn't created the Map page!\n";
            exit(EXIT_FAILURE);
        }
        m_created = true;
        return true;
    }

    void TitleBar::destroy()
    {
        delete[] testFrame;
        m_created = false;
        m_title.clear();
    }

    void TitleBar::draw()
    {
        if (!m_created)
            return;
        int ping = last_response_time.load();
        float t = static_cast<float>(ping) / static_cast<float>(MaxPing.load());
        if (t < 0.0f)
            t = 0.0f;
        if (t > 1.0f)
            t = 1.0f;
        ImVec4 color = ImVec4(t, 1.0f - t, 0.2f, 1.0f);

        ImGui::BeginMainMenuBar();
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0.4f, 0.5f, 0.7f, 1.0f));
        if (ImGui::Button("Home"))
        {
            m_page_Video = true;
            m_page_Map = true;
            m_page_FlightData = true;
            m_page_Settings = false;
            m_page_Logs = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Video"))
        {
            m_page_Video = !m_page_Video;
        }
        ImGui::SameLine();
        if (ImGui::Button("Map"))
        {
            m_page_Map = !m_page_Map;
        }
        ImGui::SameLine();
        if (ImGui::Button("Flight Data"))
        {
            m_page_FlightData = !m_page_FlightData;
        }
        ImGui::SameLine();
        if (ImGui::Button("Settings"))
        {
            m_page_Settings = !m_page_Settings;
        }
        ImGui::SameLine();
        if (ImGui::Button("Logs"))
        {
            m_page_Logs = !m_page_Logs;
        }
        ImGui::SameLine();
        if (ImGui::Button("Debug Info"))
        {
            m_page_DebugInfo = !m_page_DebugInfo;
        }

        ImGui::SameLine(ImGui::GetIO().DisplaySize.x - 290);
        if (connected.load())
            ImGui::TextColored(ImVec4(0.2f, 1, 0.2, 1), "Connected");
        else
            ImGui::TextColored(ImVec4(1, 0.2, 0.2, 1), "Disconnected");

        ImGui::SameLine();

        ImGui::TextColored(color, "Response Time: %d ms", last_response_time.load());

        ImGui::SameLine();
        ImGui::Text("v%s", app_version);

        ImGui::PopStyleColor();
        ImGui::EndMainMenuBar();

        if (m_page_Video)
        {
            m_video.draw();
        }
        if (m_page_Map)
        {
            m_map.draw();
        }
        if (m_page_FlightData)
        {
            // m_flightData.draw();
        }
        if (m_page_Settings)
        {
            // m_settings.draw();
        }
        if (m_page_Logs)
        {
            // m_logs.draw();
        }
        if (m_page_DebugInfo)
        {
            m_debugInfo.draw();
        }
    }

}
