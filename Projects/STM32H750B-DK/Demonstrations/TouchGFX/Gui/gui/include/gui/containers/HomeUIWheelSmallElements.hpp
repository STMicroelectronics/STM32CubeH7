#ifndef HOMEUIWHEELSMALLELEMENTS_HPP
#define HOMEUIWHEELSMALLELEMENTS_HPP

#include <gui_generated/containers/HomeUIWheelSmallElementsBase.hpp>

class HomeUIWheelSmallElements : public HomeUIWheelSmallElementsBase
{
public:
    HomeUIWheelSmallElements();
    virtual ~HomeUIWheelSmallElements() {}

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

#endif // HOMEUIWHEELSMALLELEMENTS_HPP
