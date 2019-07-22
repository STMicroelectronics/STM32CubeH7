#ifndef GRAPHICS_EFFECT_VIEW_HPP
#define GRAPHICS_EFFECT_VIEW_HPP

#include <gui/common/DemoView.hpp>
#include <gui/graphics_effect_screen/GraphicsEffectPresenter.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/containers/Slider.hpp>

class GraphicsEffectView : public DemoView<GraphicsEffectPresenter>
{
public:
    GraphicsEffectView();
    virtual ~GraphicsEffectView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void handleGestureEvent(const GestureEvent& evt);

    void changeSlider1Value(uint16_t val);
    void changeSlider2Value(uint16_t val);

    void screenClick(ClickEvent::ClickEventType type, int x, int y);


protected:
    Image background;
    Slider slider1;
    Slider slider2;

    void sliderValueChangedCallbackHandler(const touchgfx::Slider& src, int value);
    touchgfx::Callback<GraphicsEffectView, const touchgfx::Slider&, int> sliderValueChangedCallback;

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

    TextAreaWithOneWildcard fpsInfo;
    Unicode::UnicodeChar fpsInfoBuf[FPS_TITLE_LENGTH];

    virtual void sliderValueUpdated(int value);
    virtual void slider2ValueUpdated(int value);

};

#endif // GRAPHICS_EFFECT_VIEW_HPP
