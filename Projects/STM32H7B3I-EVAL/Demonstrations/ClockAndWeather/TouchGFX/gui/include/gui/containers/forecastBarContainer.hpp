#ifndef FORECASTBARCONTAINER_HPP
#define FORECASTBARCONTAINER_HPP

#include <gui_generated/containers/forecastBarContainerBase.hpp>

class forecastBarContainer : public forecastBarContainerBase
{
public:
    forecastBarContainer();
    virtual ~forecastBarContainer() {}

    virtual void initialize();

    void updateForecast(struct weatherData *data, bool metric);
    void setAlpha(uint8_t alpha);
    uint8_t getAlpha();

protected:
};

#endif // FORECASTBARCONTAINER_HPP
