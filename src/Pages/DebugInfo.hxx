#ifndef _IHA_BASIC_PAGES_DEBUG_
#define _IHA_BASIC_PAGES_DEBUG_

#include <imgui.h>
#include <Globals.hxx>

namespace basic
{
    class DebugInfo
    {
    public:
        DebugInfo() = default;
        ~DebugInfo() = default;

        void draw();
    };
}

#endif