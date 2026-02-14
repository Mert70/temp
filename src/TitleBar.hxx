#ifndef _IHA_BASIC_TITLEBAR_
#define _IHA_BASIC_TITLEBAR_

#include <imgui.h>
#include <Globals.hxx>
#include <Pages/Pages.hxx>

#include <string>

namespace basic
{
    class TitleBar
    {
    public:
        TitleBar() = default;
        TitleBar(Window *_window) { create(_window); }
        ~TitleBar() { destroy(); }

        bool create(Window *_window);
        void destroy();

        void draw();

    private:
        bool m_page_Video = false;
        bool m_page_Map = false;
        bool m_page_FlightData = false;
        bool m_page_Settings = false;
        bool m_page_Logs = false;
        bool m_page_DebugInfo = false;

        uint8_t *testFrame = nullptr;

        Video m_video;
        Video m_map;
        DebugInfo m_debugInfo;

        Window *m_window = nullptr;
        bool m_created = false;
        std::string m_title;
    };
}

#endif