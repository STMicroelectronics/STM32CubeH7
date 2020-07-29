#ifndef OVENTIMERCONTAINER_HPP
#define OVENTIMERCONTAINER_HPP

#include <gui_generated/containers/OvenTimerContainerBase.hpp>

class OvenTimerContainer : public OvenTimerContainerBase
{
public:
    OvenTimerContainer();
    virtual ~OvenTimerContainer() {}

    virtual void initialize();

    virtual void scrollWheelHoursUpdateItem(OvenTimerWheelElement& item, int16_t itemIndex);
    virtual void scrollWheelHoursUpdateCenterItem(OvenTimerWheelCenterElement& item, int16_t itemIndex);
    virtual void scrollWheelMinutesUpdateItem(OvenTimerWheelElement& item, int16_t itemIndex);
    virtual void scrollWheelMinutesUpdateCenterItem(OvenTimerWheelCenterElement& item, int16_t itemIndex);

    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

    void setWheelsValue(int16_t hour, int16_t minute);
    int16_t getSelectedHour();
    int16_t getSelectedMinute();

    void animateHourWheelToItem(int16_t item, int16_t steps);
    void animateMinuteWheelToItem(int16_t item, int16_t steps);

protected:
};

#endif // OVENTIMERCONTAINER_HPP
