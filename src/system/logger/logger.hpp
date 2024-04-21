#include <FreeRTOS.h>

#include <source_location>

class Logger
{
public:
    static void assert(auto expr, auto loc = std::source_location::current())
    {
        if (expr) return;
        vAssertCalled(loc.file_name(), loc.line());
    }
};
