#include <CortexMMCUInstrumentation.hpp>
#include <touchgfx/hal/HAL.hpp>

namespace touchgfx
{
void CortexMMCUInstrumentation::init()
{
    // See: http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.ddi0337e/CEGHJDCF.html
    //
    //      [24]  Read/write  TRCENA  This bit must be set to 1 to enable use of the trace and debug blocks:
    //                                    Data Watchpoint and Trace (DWT)
    //                                    Instrumentation Trace Macrocell (ITM)
    //                                    Embedded Trace Macrocell (ETM)
    //                                    Trace Port Interface Unit (TPIU).
    //                                    This enables control of power usage unless tracing is required. The application can enable this, for ITM use, or use by a debugger.

    // Enable Debug Exception and Monitor Control Register
    *((volatile unsigned int*)0xE000EDFC) |= 0x01000000;
    // Enable Lock Access Register
    *((volatile unsigned int*)0xE0001FB0) |= 0xC5ACCE55;
    // Enable Data Watchpoint and Trace Control Register
    *((volatile unsigned int*)0xE0001000) |= 1;
}

//Board specific clockfrequency
unsigned int CortexMMCUInstrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency)
{
    return ((now - start) + (clockfrequency / 2)) / clockfrequency;
}

unsigned int CortexMMCUInstrumentation::getCPUCycles()
{
    return *((volatile unsigned int*)0xE0001004);
}

void CortexMMCUInstrumentation::setMCUActive(bool active)
{
    if (active) //idle task sched out
    {
        uint32_t cc_temp = getCPUCycles() - cc_in;
        cc_consumed += cc_temp;
    }
    else //idle task sched in
    {
        cc_in = getCPUCycles();
    }
}
}
