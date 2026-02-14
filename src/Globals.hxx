#ifndef _IHA_BASIC_GLOBALS_
#define _IHA_BASIC_GLOBALS_

#include <atomic>
#include <string>

namespace basic
{
    extern std::atomic<bool> connected;
    extern std::atomic<int> last_response_time;
    extern std::atomic<int> MaxPing;   
    static inline const char *app_version = "1.0.0";
}

#endif