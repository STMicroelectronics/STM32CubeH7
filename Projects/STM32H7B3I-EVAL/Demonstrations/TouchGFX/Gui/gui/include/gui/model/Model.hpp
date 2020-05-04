#ifndef MODEL_HPP
#define MODEL_HPP

#include <touchgfx/Utils.hpp>

#include <gui/common/Defines.hpp>
#include <gui/model/Time.hpp>

class ModelListener;

/**
 * The Model class defines the data model in the model-view-presenter paradigm.
 * The Model is a singular object used across all presenters. The currently active
 * presenter will have a pointer to the Model through deriving from ModelListener.
 *
 * The Model will typically contain UI state information that must be kept alive
 * through screen transitions. It also usually provides the interface to the rest
 * of the system (the backend). As such, the Model can receive events and data from
 * the backend and inform the current presenter of such events through the modelListener
 * pointer, which is automatically configured to point to the current presenter.
 * Conversely, the current presenter can trigger events in the backend through the Model.
 */
class Model
{
public:
    Model();

    /**
     * Sets the modelListener to point to the currently active presenter. Called automatically
     * when switching screen.
     */
    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    /**
     * This function will be called automatically every frame. Can be used to e.g. sample hardware
     * peripherals or read events from the surrounding system and inject events to the GUI through
     * the ModelListener interface.
     */
    void tick();

    Time getCurrentTime() const
    {
        return currentTime;
    }

    bool getMCULoadActive() const
    {
        return mcuLoadActive;
    }
    void setMCULoadActive(bool active)
    {
        mcuLoadActive = active;
    }

    Defines::DemoID getSelectedDemoScreen()
    {
        return selectedDemoScreen;
    }
    void setSelectedDemoScreen(Defines::DemoID demoId)
    {
        selectedDemoScreen = demoId;
    }

    // Stores the menu type that was used to choose a certain demo screen. This is
    // used for returning to the same menu.
    // When toggling between menus the NO_MENU is used.
    Defines::MainMenuType getPreviousSelectedMainMenuType()
    {
        return previousSelectedMenuType;
    }
    void setPreviousSelectedMainMenuType(Defines::MainMenuType menu)
    {
        previousSelectedMenuType = menu;
    }

    void screenSaverMinorTick();
    void screenSaverMajorTick();

    int getPotentiometerValue();

    uint8_t getBrightnessValue();
    void setBrightnessValue(uint8_t value);

    void setSelectedClock(int clockIndex)
    {
        selectedClockIndex = clockIndex;
    }
    int getSelectedClock()
    {
        return selectedClockIndex;
    }

    void setTime(uint8_t hour, uint8_t minute, uint8_t second);

    uint16_t getDateYear()
    {
        return dateYear;
    }
    uint8_t getDateMonth()
    {
        return dateMonth;
    }
    uint8_t getDateDay()
    {
        return dateDay;
    }
    void setDate(uint16_t year, uint8_t month, uint8_t day);

    void setScanPotentiometer(bool scanEnabled)
    {
        scanPotentiometer = scanEnabled;
    }

protected:
    /**
     * Pointer to the currently active presenter.
     */
    ModelListener* modelListener;

    bool mcuLoadActive;
    Time currentTime;

    Defines::DemoID selectedDemoScreen;
    Defines::MainMenuType previousSelectedMenuType;

    bool scanPotentiometer;

    int tickCounter;

    int selectedClockIndex;
    uint8_t brightnessValue;
    uint8_t lastBrightnessValue;

    uint16_t dateYear;
    uint8_t  dateMonth;
    uint8_t  dateDay;
    uint32_t milliseconds;
};

#endif /* MODEL_HPP */
