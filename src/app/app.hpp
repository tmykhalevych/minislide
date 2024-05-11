#pragma once

#include <prohibit_copy_move.hpp>

namespace app
{

class App : public cmn::ProhibitCopyMove
{
public:
    App();

    void start() const;

private:
    bool init_platform() const;
    bool init_firmware() const;

    void init();
};

}  // namespace app
