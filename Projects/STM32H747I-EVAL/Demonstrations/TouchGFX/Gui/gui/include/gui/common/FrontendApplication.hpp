

#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <mvp/View.hpp>
#include <mvp/MVPApplication.hpp>
#include <gui/model/Model.hpp>

using namespace touchgfx;

class FrontendHeap;

/**
 * The FrontendApplication is the concrete implementation of an MVPApplication
 * for a specific application. Its primary job is to provide functions for
 * switching between screens (presenter/view pairs). By convention these functions
 * are usually called gotoXXScreen().
 *
 * Note that the base class Application is a singleton. Switching screen can be done
 * from anywhere (usually presenters) by e.g. the following:
 * static_cast<FrontendApplication*>(Application::getInstance())->gotoXXScreen();
 */
class FrontendApplication : public MVPApplication
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleTickEvent();

    void resetScreenSaver();

    /**
     * Request a transition to the "Template" screen. The actual switching will
     * be done at next tick (by gotoTemplateScreenImpl).
     *
     * @note The reason for not switching immediately is that the new presenter and view
     * reuse the same memory as the current ones, so the switch cannot occur instantaneously
     * since that would cause memory corruption at the point of calling this function.
     */

    void gotoMainMenuCarouselScreen();
    void gotoMainMenuAnimatingButtonsScreen();
    void gotoGame2048Screen();
    void gotoGame2DScreen();
    void gotoControlsScreen();
    void gotoAudioPlayerScreen();
    void gotoHomeAutomationScreen();
    void gotoSettingsScreen();
    void gotoVideoPlayerScreen();
    void gotoGraphicsEffectScreen();

private:
    static const int SCREEN_SAVER_TIMEOUT = 600; // 180000; // Milliseconds
    static const int MINOR_TICK_FREQ = 3000 / 17; // Ticks - so Milliseconds / frequency (ms per frame)
    static const int MAJOR_TICK_FREQ = 10000 / 17; // Ticks - so Milliseconds / frequency (ms per frame)

    Callback<FrontendApplication> transitionCallback;
    FrontendHeap& frontendHeap;
    Model& model;

    int screenSaverTick;
    
    void gotoMainMenuCarouselScreenImpl();
    void gotoMainMenuAnimatingButtonsScreenImpl();
    void gotoGame2048ScreenImpl();
    void gotoGame2DScreenImpl();
    void gotoControlsScreenImpl();
    void gotoAudioPlayerScreenImpl();
    void gotoHomeAutomationScreenImpl();
    void gotoSettingsScreenImpl();
    void gotoVideoPlayerScreenImpl();
    void gotoGraphicsEffectScreenImpl();
};

#endif /* FRONTENDAPPLICATION_HPP */
