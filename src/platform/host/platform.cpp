#include <platform.hpp>

#include <iostream>

namespace pfm
{

bool init()
{
    std::cout << "HOST: platform init" << std::endl;
    return true;
}

}  // namespace pfm
