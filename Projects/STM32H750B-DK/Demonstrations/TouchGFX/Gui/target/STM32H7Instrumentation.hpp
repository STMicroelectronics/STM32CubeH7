


#ifndef STM32H7INSTRUMENTATION_HPP
#define STM32H7INSTRUMENTATION_HPP

#include <platform/core/MCUInstrumentation.hpp>
#include <stdint.h>

namespace touchgfx
{
class STM32H7Instrumentation: public MCUInstrumentation
{
public:
    virtual void init();
    virtual unsigned int getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency);
    virtual unsigned int getCPUCycles();
    virtual void setMCUActive(bool active);
private:
    uint32_t m_sysclkRatio;
};

}

#endif // STM32H7INSTRUMENTATION_HPP
