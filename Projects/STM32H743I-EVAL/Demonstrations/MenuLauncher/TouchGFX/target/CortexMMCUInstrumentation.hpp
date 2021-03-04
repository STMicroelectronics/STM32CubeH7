#ifndef CORTEXMMCUINSTRUMENTATION_HPP
#define CORTEXMMCUINSTRUMENTATION_HPP

#include <platform/core/MCUInstrumentation.hpp>
#include <stdint.h>

namespace touchgfx
{
/**
 * @class CortexMMCUInstrumentation CortexMMCUInstrumentation.hpp platform/core/arm/cortex-m/CortexMMCUInstrumentation.hpp
 *
 * @brief Interface for instrumenting Cortex-M processors to measure MCU load via measured CPU
 *        cycles.
 *
 *        Interface for instrumenting Cortex-M processors to measure MCU load via measured CPU
 *        cycles.
 *
 * @sa MCUInstrumentation
 */
class CortexMMCUInstrumentation : public MCUInstrumentation
{
public:
    /**
     * @fn virtual void CortexMMCUInstrumentation::init();
     *
     * @brief Initialization.
     *
     *        Initialization.
     */
    virtual void init();

    /**
     * @fn virtual unsigned int CortexMMCUInstrumentation::getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency);
     *
     * @brief Gets elapsed microseconds basedon clockfrequency.
     *
     *        Gets elapsed microseconds basedon clockfrequency.
     *
     * @param start          Start time.
     * @param now            Current time.
     * @param clockfrequency Clock frequency of the system.
     *
     * @return Elapsed microseconds start and now.
     */
    virtual unsigned int getElapsedUS(unsigned int start, unsigned int now, unsigned int clockfrequency);

    /**
     * @fn virtual unsigned int CortexMMCUInstrumentation::getCPUCycles();
     *
     * @brief Gets CPU cycles from register.
     *
     *        Gets CPU cycles from register.
     *
     * @return CPU cycles.
     */
    virtual unsigned int getCPUCycles();

    /**
     * @fn virtual void CortexMMCUInstrumentation::setMCUActive(bool active);
     *
     * @brief Register if MCU is active by measuring cpu cycles.
     *
     *        Register if MCU is active by measuring cpu cycles. If user wishes to track
     *        MCU load, this method should be called whenever the OS Idle task is scheduled
     *        in or out. This method makes calls to a concrete implementation of GPIO
     *        functionality and a concrete implementation of cpu cycles.
     *
     * @param active If true, MCU is registered as being active, inactive otherwise.
     */
    virtual void setMCUActive(bool active);
};
} // namespace touchgfx

#endif // CORTEXMMCUINSTRUMENTATION_HPP
