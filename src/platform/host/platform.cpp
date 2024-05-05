#include <platform.hpp>

#include <iostream>

namespace platform
{

bool init()
{
    std::cout << "HOST: platform init" << std::endl;
    return true;
}

}  // namespace platform
