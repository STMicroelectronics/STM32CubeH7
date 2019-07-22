

#ifndef CLOCK_SELECTOR_CONTAINER
#define CLOCK_SELECTOR_CONTAINER

#include <touchgfx/widgets/Image.hpp>
#include <gui/settings_screen/AnalogClock.hpp>
#include <BitmapDatabase.hpp>
#include <touchgfx/mixins/ClickListener.hpp>

using namespace touchgfx;

class ClockSelectorContainer : public Container
{
public:
    static const int NUMBER_OF_CLOCK_SELECTIONS = 3;

    ClockSelectorContainer();
    virtual ~ClockSelectorContainer();

    void updateTime(uint8_t hour, uint8_t minute, uint8_t second);
    void initializeTime(uint8_t hour, uint8_t minute, uint8_t second);

    AnalogClock* getClock(int index);
    void setSelectedClock(int clockIndex);
    int getSelectedClock()
    {
        return selectedClockIndex;
    }

    void setSelectedClockChangedCallback(GenericCallback< const ClockSelectorContainer&, int >& callback)
    {
        selectedClockChangedCallback = &callback;
    }

private:
    ClickListener<AnalogClock> clockSelectors[NUMBER_OF_CLOCK_SELECTIONS];
    Image clockSelectedIndicator;

    Callback<ClockSelectorContainer, const AnalogClock&, const ClickEvent&> clockSelectedCallback;
    void clockSelectedHandler(const AnalogClock& c, const ClickEvent& evt);
    GenericCallback<const ClockSelectorContainer&, int >* selectedClockChangedCallback;

    int selectedClockIndex;
    bool pressed; ///< Is the clock pressed or released? True if pressed.
};

#endif /* CLOCK_SELECTOR_CONTAINER */
