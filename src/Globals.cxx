#include <atomic>
#include <string>

namespace basic
{
    std::atomic<bool> connected = false;
    std::atomic<int> last_response_time = 0;
    std::atomic<int> MaxPing = 200;
}