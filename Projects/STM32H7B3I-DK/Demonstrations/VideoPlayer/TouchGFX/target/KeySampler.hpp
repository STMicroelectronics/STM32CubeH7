#ifndef KEYSAMPLER_HPP
#define KEYSAMPLER_HPP

#include <platform/driver/button/ButtonController.hpp>

namespace touchgfx
{
class KeySampler : public ButtonController
{
public:
    KeySampler()
    {
        init();
    }
    virtual ~KeySampler() {}

    /**
     * Initializes keySampler
     */
    virtual void init();

    /**
     * Sample external key events.
     * @param key Output parameter that will be set to the key value if a keypress was detected.
     *
     * @return True if a keypress was detected and the "key" parameter is set to a value.
     */
    virtual bool sample(uint8_t& key);

    static const uint8_t Key1 = 0x01;
};
}

#endif // KEYSAMPLER_HPP
