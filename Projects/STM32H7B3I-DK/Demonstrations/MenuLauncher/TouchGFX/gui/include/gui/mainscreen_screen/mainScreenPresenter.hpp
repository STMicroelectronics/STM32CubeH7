#ifndef MAINSCREENPRESENTER_HPP
#define MAINSCREENPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class mainScreenView;

class mainScreenPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    mainScreenPresenter(mainScreenView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~mainScreenPresenter() {};

    void moduleSelected(int moduleIndex);

    virtual void setFirmwareVersion(const char* fwVersion);

private:
    mainScreenPresenter();

    mainScreenView& view;
};


#endif // MAINSCREENPRESENTER_HPP
