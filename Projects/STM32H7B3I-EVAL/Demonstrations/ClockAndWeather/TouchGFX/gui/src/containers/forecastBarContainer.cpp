#include <gui/containers/forecastBarContainer.hpp>

forecastBarContainer::forecastBarContainer()
{

}

void forecastBarContainer::initialize()
{
    forecastBarContainerBase::initialize();
}

void forecastBarContainer::updateForecast(struct weatherData *data, bool metric)
{
    forecastDay1.update(data->icon[1], data->dates[1], data->temperature[1], data->wind[1], metric);
    forecastDay2.update(data->icon[2], data->dates[2], data->temperature[2], data->wind[2], metric);
    forecastDay3.update(data->icon[3], data->dates[3], data->temperature[3], data->wind[3], metric);
    forecastDay4.update(data->icon[4], data->dates[4], data->temperature[4], data->wind[4], metric);
    forecastDay5.update(data->icon[5], data->dates[5], data->temperature[5], data->wind[5], metric);
}

void forecastBarContainer::setAlpha(uint8_t alpha)
{
    forecastDay1.setAlpha(alpha);
    forecastDay2.setAlpha(alpha);
    forecastDay3.setAlpha(alpha);
    forecastDay4.setAlpha(alpha);
    forecastDay5.setAlpha(alpha);
}

uint8_t forecastBarContainer::getAlpha()
{
    return forecastDay1.getAlpha();
}
