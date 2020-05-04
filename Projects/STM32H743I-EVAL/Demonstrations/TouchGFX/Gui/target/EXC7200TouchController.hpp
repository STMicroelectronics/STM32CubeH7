
#ifndef EXC7200TOUCHCONTROLLER_HPP
#define EXC7200TOUCHCONTROLLER_HPP

#include <platform/driver/touch/TouchController.hpp>

class EXC7200TouchController : public touchgfx::TouchController
{
public:
    EXC7200TouchController() {}
    virtual ~EXC7200TouchController() {}
    virtual void init();
    virtual bool sampleTouch(int32_t& x, int32_t& y);
};

#endif // EXC7200TOUCHCONTROLLER_HPP
