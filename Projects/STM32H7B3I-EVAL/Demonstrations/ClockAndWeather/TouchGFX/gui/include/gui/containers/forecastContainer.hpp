#ifndef FORECASTCONTAINER_HPP
#define FORECASTCONTAINER_HPP

#include <gui_generated/containers/forecastContainerBase.hpp>

class forecastContainer : public forecastContainerBase
{
public:
    forecastContainer();
    virtual ~forecastContainer() {}

    virtual void initialize();

    void update(char *icon, char *weekday, int temperature, int windspeed, bool metric);

    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:

    void updateIcon(char *icon);
    void updateWeekday(char *weekday);
    void updateTemperature(int temperature);
    void updateWindspeed(int speed, bool metric);
};

#endif // FORECASTCONTAINER_HPP
