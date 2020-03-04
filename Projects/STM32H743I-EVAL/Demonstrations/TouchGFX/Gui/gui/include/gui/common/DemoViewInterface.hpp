
#ifndef DEMO_VIEW_INTERFACE_HPP
#define DEMO_VIEW_INTERFACE_HPP

#include <touchgfx/hal/Types.hpp>

using namespace touchgfx;

class DemoViewInterface
{
public:
    virtual ~DemoViewInterface() {}
    virtual void updateProcessorLoad(uint8_t mcuLoadInProcent) {}
};
 
#endif
