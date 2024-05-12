#pragma once

#include <prohibit_copy_move.hpp>

namespace fw
{

class Firmware : public cmn::ProhibitCopyMove
{
public:
    Firmware();

    void start() const;

private:
    bool init_bsp() const;
    bool init_services() const;

    void init();
};

}  // namespace fw
