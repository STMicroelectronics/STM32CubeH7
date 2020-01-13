#ifndef ANIMATEDIMAGE_2_HPP
#define ANIMATEDIMAGE_2_HPP

#include <touchgfx/widgets/AnimatedImage.hpp>

namespace touchgfx
{
class AnimatedImage2 : public AnimatedImage
{
public:
    AnimatedImage2() :
    bitmapInterval(1)
    {
    
    }

    // the new handleTickEvent() is only able to move in one direction, but is
    // to skip images resulting in a "faster" animation
    virtual void handleTickEvent();

    // sets the interval between the images used for the animation.
    // Interval of 1 is every images, 2 is every second and so forth
    void setBitmapInterval(uint8_t);

protected:
    uint8_t bitmapInterval; //The set interval

};

} // namespace touchgfx

#endif // ANIMATEDIMAGE_2_HPP
