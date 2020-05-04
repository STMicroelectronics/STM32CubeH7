#ifndef BUMP_MAP_PRESENTER_HPP
#define BUMP_MAP_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <gui/common/DemoPresenter.hpp>

using namespace touchgfx;

class BumpMapView;


class BumpMapPresenter : public DemoPresenter
{
public:
    BumpMapPresenter(BumpMapView& v);

    virtual void activate();
    virtual void deactivate();

    virtual ~BumpMapPresenter() {};
    virtual void oneSecond();
    virtual void screenSaverMinorTick();
    
private:
    BumpMapPresenter();
    BumpMapView& view;

    int16_t counter;
};

#endif 
