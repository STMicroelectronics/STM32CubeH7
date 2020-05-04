#ifndef GRAPHICS_EFFECTVIEW_HPP
#define GRAPHICS_EFFECTVIEW_HPP

#include <gui_generated/graphics_effect_screen/graphics_effectViewBase.hpp>
#include <gui/graphics_effect_screen/graphics_effectPresenter.hpp>

#include <touchgfx/widgets/TextureMapper.hpp>

class graphics_effectView : public graphics_effectViewBase
{
public:
    graphics_effectView();
    virtual ~graphics_effectView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void slider1ValueUpdated(int value);
    virtual void slider2ValueUpdated(int value);

    void handleTickEvent();
    void handleGestureEvent(const GestureEvent& evt);

    void changeSlider1Value(uint16_t val);
    void changeSlider2Value(uint16_t val);

protected:

    int tickCounter;

    static const int NUMBER_OF_TM = 4;
    TextureMapper textureMapperImage3D[NUMBER_OF_TM];

    float xAngle3D;
    float yAngle3D;
    float zAngle3D;

    float deltaXangle3D;
    float deltaYangle3D;
    float deltaZangle3D;

    int gestureYTick;
    float gestureY;
    int gestureZTick;
    float gestureZ;

    float zAngle2D;

    float deltaZangle2D;

    int cameraYDelta;

    enum
    {
        FPS_TITLE_LENGTH = 4
    };
};

#endif // GRAPHICS_EFFECTVIEW_HPP
