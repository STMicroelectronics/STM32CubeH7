#ifndef CURRENTWEATHERDATACONTAINER_HPP
#define CURRENTWEATHERDATACONTAINER_HPP

#include <gui_generated/containers/currentWeatherDataContainerBase.hpp>

class currentWeatherDataContainer : public currentWeatherDataContainerBase
{
public:
    currentWeatherDataContainer();
    virtual ~currentWeatherDataContainer() {}

    virtual void initialize();

    void updateCurrentWeatherInformation(weatherData *data, bool metric);

    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:
};

#endif // CURRENTWEATHERDATACONTAINER_HPP
