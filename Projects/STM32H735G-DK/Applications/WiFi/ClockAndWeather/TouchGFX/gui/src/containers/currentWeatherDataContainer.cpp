#include <gui/containers/currentWeatherDataContainer.hpp>

currentWeatherDataContainer::currentWeatherDataContainer()
{

}

void currentWeatherDataContainer::initialize()
{
    currentWeatherDataContainerBase::initialize();
}

void currentWeatherDataContainer::updateCurrentWeatherInformation(weatherData *data, bool metric)
{
    if (metric)
    {
        Unicode::snprintf(windSpeedValueBuffer, WINDSPEEDVALUE_SIZE, "%d m/s", data->wind[0]);
    }
    else
    {
        Unicode::snprintf(windSpeedValueBuffer, WINDSPEEDVALUE_SIZE, "%d mph", data->wind[0]);
    }
    windSpeedValue.invalidate();

    if (data->windDegree < 23)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "North");
    }
    else if (data->windDegree < 67)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "Northeast");
    }
    else if (data->windDegree < 112)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "East");
    }
    else if (data->windDegree < 160)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "Southeast");
    }
    else if (data->windDegree < 202)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "South");
    }
    else if (data->windDegree < 247)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "Southwest");
    }
    else if (data->windDegree < 292)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "West");
    }
    else if (data->windDegree < 337)
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "Northwest");
    }
    else
    {
        Unicode::snprintf(windDirectionValueBuffer, WINDDIRECTIONVALUE_SIZE, "North");
    }
    windDirectionValue.invalidate();

    Unicode::snprintf(pressureValBuffer, PRESSUREVAL_SIZE, "%d", data->pressure);
    pressureVal.invalidate();

    Unicode::snprintf(humidityValBuffer, HUMIDITYVAL_SIZE, "%d", data->humidity);
    humidityVal.invalidate();

    if (data->rain == 0)
    {
        Unicode::snprintf(rainValBuffer, RAINVAL_SIZE, "0");
    }
    else
    {
        Unicode::snprintfFloat(rainValBuffer, RAINVAL_SIZE, "%f", (float)data->rain);
    }
    rainVal.invalidate();
}

void currentWeatherDataContainer::setAlpha(uint8_t alpha)
{
    windSpeed.setAlpha(alpha);
    windDirection.setAlpha(alpha);
    pressure.setAlpha(alpha);
    humidity.setAlpha(alpha);
    rain.setAlpha(alpha);
    windSpeedValue.setAlpha(alpha);
    windDirectionValue.setAlpha(alpha);
    pressureVal.setAlpha(alpha);
    humidityVal.setAlpha(alpha);
    rainVal.setAlpha(alpha);
    rainIcon.setAlpha(alpha);
    windSpeedIcon.setAlpha(alpha);
    pressureIcon.setAlpha(alpha);
}

uint8_t currentWeatherDataContainer::getAlpha()
{
    return windSpeedIcon.getAlpha();
}
