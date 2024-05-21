#pragma once

namespace cmn
{

/// @brief Helper class to prohibit derived class copy operations
class ProhibitCopy
{
public:
    ProhibitCopy(const ProhibitCopy&) = delete;
    ProhibitCopy& operator=(const ProhibitCopy&) = delete;

    ~ProhibitCopy() = default;

protected:
    ProhibitCopy() = default;
};

/// @brief Helper class to prohibit derived class move operations
class ProhibitMove
{
public:
    ProhibitMove(ProhibitMove&&) = delete;
    ProhibitMove& operator=(ProhibitMove&&) = delete;

    ~ProhibitMove() = default;

protected:
    ProhibitMove() = default;
};

/// @brief Helper class to prohibit derived class copy and move operations
class ProhibitCopyMove : public ProhibitCopy, public ProhibitMove
{
public:
    ~ProhibitCopyMove() = default;

protected:
    ProhibitCopyMove() = default;
};

}  // namespace cmn