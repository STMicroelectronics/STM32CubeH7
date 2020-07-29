#include <gui/common/FrontendApplication.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap),
      model(m),
      screenSaverTick(0)
{

}

void FrontendApplication::handleTickEvent()
{
    model.tick();
    FrontendApplicationBase::handleTickEvent();

    // Screen saver functionality:
    if (screenSaverTick >= model.GetautoDemoTicks())
    {
        if (!model.GetautoAnimate())
        {
            model.SetautoAnimate(true);
        }
        screenSaverTick = 0;
    }
    else
    {
        screenSaverTick++;
    }
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{
    FrontendApplicationBase::handleClickEvent(evt);

    // A click event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{
    FrontendApplicationBase::handleDragEvent(evt);

    // A drag event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::handleGestureEvent(const GestureEvent& evt)
{
    FrontendApplicationBase::handleGestureEvent(evt);

    // A drag event has been registered so reset last click time
    resetScreenSaver();
}

void FrontendApplication::resetScreenSaver()
{
    screenSaverTick = 0;
    model.SetautoAnimate(false);
}
