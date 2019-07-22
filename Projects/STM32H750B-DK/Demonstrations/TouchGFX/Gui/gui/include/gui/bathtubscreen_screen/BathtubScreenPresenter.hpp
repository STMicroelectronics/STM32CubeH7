#ifndef BATHTUBSCREEN_PRESENTER_HPP
#define BATHTUBSCREEN_PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class BathtubScreenView;

class BathtubScreenPresenter : public Presenter, public ModelListener
{
public:
    BathtubScreenPresenter(BathtubScreenView& v);

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

    virtual ~BathtubScreenPresenter() {};

    // The set and get functions are used to update the states and the values
    // concrening the bathtub.
    void setBathtubOn(bool on);
    bool getBathtubOn();
    void setTempCelcius(bool celcius);
    bool getTempCelcius();
    void setJetOn(bool on);
    bool getJetOn();
    void setLightOn(bool on);
    bool getLightOn();

    void setTempValue(uint16_t temp);
    uint16_t getTempValue();
    void setJetLevel(uint16_t level);
    uint16_t getJetLevel();
    void setColorCoordinates(uint32_t x, uint32_t y);
    void getColorCoordinates(uint32_t& x, uint32_t& y);

    virtual void mcuLoadUpdated(uint8_t load);

    virtual void autoDemoTick();
    virtual void autoDemoStart();

    void setAutoDemoState(Model::AutoDemoStates state);
    Model::AutoDemoStates getAutoDemoState();

private:
    BathtubScreenPresenter();

    BathtubScreenView& view;
};

#endif // BATHTUBSCREEN_PRESENTER_HPP
