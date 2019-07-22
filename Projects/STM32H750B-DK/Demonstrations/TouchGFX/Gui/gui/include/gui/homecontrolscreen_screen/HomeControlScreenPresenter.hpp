#ifndef HOMECONTROLSCREEN_PRESENTER_HPP
#define HOMECONTROLSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class HomeControlScreenView;

class HomeControlScreenPresenter : public Presenter, public ModelListener
{
public:
    HomeControlScreenPresenter(HomeControlScreenView& v);

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

    virtual ~HomeControlScreenPresenter() {};

    bool getLightState();
    bool getRecipeState();
    bool getOvenState();
    bool getFanState();
    uint16_t getFanSpeed();
    bool getBathtubOn();
    bool getTempCelcius();
    bool getJetOn();
    bool getLightOn();
    uint16_t getTempValue();
    uint16_t getJetLevel();
    uint16_t getSelectedOvenMode();
    int16_t getOvenTimerHour();
    int16_t getOvenTimerMinute();
    uint16_t getOvenTemperature();
    uint16_t getWashingProgram();
    uint16_t getWashingTime();

    bool getBathroomHomeUI();

    bool getKitchenUIEntered();
    bool getBathroomUIEntered();

    void setFromRecipeScreen(bool recipeScreen);
    bool getFromRecipeScreen();

    virtual void mcuLoadUpdated(uint8_t mcuLoad);

    virtual void autoDemoTick();
    virtual void autoDemoStart();

    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();

private:
    HomeControlScreenPresenter();

    HomeControlScreenView& view;
};


#endif // HOMECONTROLSCREEN_PRESENTER_HPP
