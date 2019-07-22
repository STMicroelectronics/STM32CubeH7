#ifndef HOMEUIWHEELLARGEELEMENTS_HPP
#define HOMEUIWHEELLARGEELEMENTS_HPP

#include <gui_generated/containers/HomeUIWheelLargeElementsBase.hpp>

class HomeUIWheelLargeElements : public HomeUIWheelLargeElementsBase
{
public:
    HomeUIWheelLargeElements();
    virtual ~HomeUIWheelLargeElements() {}

    virtual void initialize();

    void setRoomInfo(int16_t item);
    void updateCurrentItem(int16_t item);
    void setContent(bool devices);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:
    int16_t currentItem;
    bool showDevices;
};

#endif // HOMEUIWHEELLARGEELEMENTS_HPP
