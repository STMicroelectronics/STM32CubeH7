#ifndef RECIPESELECTEDSCREEN_VIEW_HPP
#define RECIPESELECTEDSCREEN_VIEW_HPP

#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui_generated/recipeselectedscreen_screen/RecipeSelectedScreenViewBase.hpp>
#include <gui/recipeselectedscreen_screen/RecipeSelectedScreenPresenter.hpp>
#include <gui/containers/FpsMcuLoad.hpp>
#include <widgets/MJPEGReader.hpp>

class RecipeSelectedScreenView : public RecipeSelectedScreenViewBase
{
public:
    RecipeSelectedScreenView();
    virtual ~RecipeSelectedScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleGestureEvent(const GestureEvent& evt);

    void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();

    void screenClick(ClickEvent::ClickEventType type, int16_t x, int16_t y);
    void screenDrag(DragEvent::DragEventType type, int16_t oldX, int16_t oldY, int16_t newX, int16_t newY);
    void screenGesture(GestureEvent::GestureType type, int16_t v, int16_t x_coord, int16_t y_coord);

protected:
    void updateVideoPlayButton(bool movingUp);
    virtual void playClicked();

    FadeAnimator<MJPEGReader> mjpgWidget;
    bool playing;
    BitmapId videoBitmap;
    BitmapId temporaryBufferBitmap;

private:
    Callback<RecipeSelectedScreenView> movieEndedCallback;

    void onMovieEnded();
    FpsMcuLoad fps_mcu;

    uint16_t autoDemoCounter;
    bool autoDemoScroll;
    int16_t scrollSpeed;
};

#endif // RECIPESELECTEDSCREEN_VIEW_HPP
