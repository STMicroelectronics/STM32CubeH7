#ifndef GRAPHICS_EFFECTPRESENTER_HPP
#define GRAPHICS_EFFECTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class graphics_effectView;

class graphics_effectPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    graphics_effectPresenter(graphics_effectView& v);

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

    virtual ~graphics_effectPresenter() {};

    void gotoMenuSelected();

private:
    graphics_effectPresenter();

    graphics_effectView& view;
};


#endif // GRAPHICS_EFFECTPRESENTER_HPP
