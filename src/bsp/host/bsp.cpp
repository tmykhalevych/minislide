#include <bsp.hpp>

#include <iostream>

namespace bsp
{

bool init()
{
    std::cout << "HOST: bsp init" << std::endl;
    return true;
}

}  // namespace bsp
