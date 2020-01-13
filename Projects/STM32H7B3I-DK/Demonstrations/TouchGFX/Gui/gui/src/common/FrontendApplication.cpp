#include <gui/common/FrontendApplication.hpp>

FrontendApplication::FrontendApplication(Model& m, FrontendHeap& heap)
    : FrontendApplicationBase(m, heap)
{
}

void FrontendApplication::handleClickEvent(const ClickEvent& evt)
{
    FrontendApplicationBase::handleClickEvent(evt);

    // A click event has been registered so reset last click time
    model.resetAutoDemo();
}

void FrontendApplication::handleDragEvent(const DragEvent& evt)
{
    FrontendApplicationBase::handleDragEvent(evt);

    // A drag event has been registered so reset last click time
    model.resetAutoDemo();
}
