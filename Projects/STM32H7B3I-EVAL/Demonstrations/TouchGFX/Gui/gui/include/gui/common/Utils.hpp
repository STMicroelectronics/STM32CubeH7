#ifndef GUI_UTILS_HPP
#define GUI_UTILS_HPP

#include <touchgfx/hal/HAL.hpp>

#ifdef SIMULATOR
#include <stdlib.h>
#endif

class Utils
{
public:
    static int randomNumberBetween(int lowest, int highest)
    {
#ifdef SIMULATOR
        return lowest + (highest - lowest) * rand() / RAND_MAX;
#else
        uint32_t random = (touchgfx::HAL::getInstance()->getCPUCycles() * HAL::getInstance()->getCPUCycles());
        return lowest + (random % (highest - lowest));
#endif
    }

    static int nextRand(int lowest, int highest)
    {
        static uint32_t seed = 0;
        seed = ((214013 * (uint32_t)(seed) + 2531011) >> 16);
        return lowest + (seed % (highest - lowest));
    }
};

#endif /* GUI_UTILS_HPP */
