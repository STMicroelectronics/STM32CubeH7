


#ifndef DEMO_PRESENTER_HPP
#define DEMO_PRESENTER_HPP

#include <mvp/Presenter.hpp>
#include <gui/common/DemoViewInterface.hpp>
#include <gui/common/FrontendApplication.hpp>
#include <gui/model/ModelListener.hpp>

using namespace touchgfx;

class DemoPresenter : public Presenter, public ModelListener
{
public:
    DemoPresenter(DemoViewInterface& view);

    void menuOptionSelected();
    virtual void mcuLoadUpdated(uint8_t mcuLoad);
    FrontendApplication* application();
private:
    DemoViewInterface& viewInterface;

};

#endif
