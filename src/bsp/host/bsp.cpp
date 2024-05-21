#include <bsp.hpp>

#include <fcntl.h>
#include <iostream>
#include <unistd.h>

namespace bsp
{

bool init()
{
    // setup non-blocking input for getchar()
    int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);

    std::cout << "HOST: bsp init" << std::endl;
    return true;
}

}  // namespace bsp
