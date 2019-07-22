#ifndef ABOUTSCREEN_PRESENTER_HPP
#define ABOUTSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

#ifndef SIMULATOR
#include "storage.h"
#endif

#include <widgets/FileInput.hpp>
#include <string.h>


using namespace touchgfx;

class AboutScreenView;

class AboutScreenPresenter : public Presenter, public ModelListener
{
public:
    AboutScreenPresenter(AboutScreenView& v);

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

    virtual ~AboutScreenPresenter() {};
    
    // Function called fom the model via the modelListener to update the MCU load 
    virtual void mcuLoadUpdated(uint8_t mcuLoad);   

    // Called fom the model via the modelListener to perform the Auto Demo
    virtual void autoDemoTick();
    
    // Called fom the model via the modelListener to start the Auto Demo
    virtual void autoDemoStart();

    // Handles the state of the Auto Demo to keep track of it
    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();

private:
    AboutScreenPresenter();

    AboutScreenView& view;
};

#endif // ABOUTSCREEN_PRESENTER_HPP
