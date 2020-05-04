#ifndef FRONTENDAPPLICATION_HPP
#define FRONTENDAPPLICATION_HPP

#include <gui_generated/common/FrontendApplicationBase.hpp>

class FrontendHeap;

using namespace touchgfx;

class FrontendApplication : public FrontendApplicationBase
{
public:
    FrontendApplication(Model& m, FrontendHeap& heap);
    virtual ~FrontendApplication() { }

    virtual void handleTickEvent();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void resetScreenSaver();
private:
    static const int SCREEN_SAVER_TIMEOUT = 180000; // Milliseconds
    static const int MINOR_TICK_FREQ = 3000 / 17; // Ticks - so Milliseconds / frequency (ms per frame)
    static const int MAJOR_TICK_FREQ = 10000 / 17; // Ticks - so Milliseconds / frequency (ms per frame)

    int tickCounter;
    int screenSaverTick;
    Time lastClickTime;
};

#endif // FRONTENDAPPLICATION_HPP
