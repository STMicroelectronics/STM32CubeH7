#ifndef MAINPRESENTER_HPP
#define MAINPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class mainView;

class mainPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    mainPresenter(mainView& v);

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

    virtual ~mainPresenter() {};

    void moduleSelected(int moduleIndex);

    virtual void setFirmwareVersion(const char* fwVersion);

private:
    mainPresenter();

    mainView& view;
};


#endif // MAINPRESENTER_HPP
