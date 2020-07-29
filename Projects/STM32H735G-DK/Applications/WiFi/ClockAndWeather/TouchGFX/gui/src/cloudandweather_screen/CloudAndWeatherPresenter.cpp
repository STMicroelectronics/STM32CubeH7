#include <gui/cloudandweather_screen/CloudAndWeatherView.hpp>
#include <gui/cloudandweather_screen/CloudAndWeatherPresenter.hpp>

#include "gui/common/enums.hpp"
#include <gui/common/structs.hpp>

#ifdef SIMULATOR
#include "time.h"
#include <chrono>
#endif // SIMULATOR

#include <ctype.h>

CloudAndWeatherPresenter::CloudAndWeatherPresenter(CloudAndWeatherView& v)
    : view(v)//, cityCounter(NUMBERS_OF_CITYS)
{
  cityCounter = model->getCitiesNbr();
}

void CloudAndWeatherPresenter::activate()
{
    switchCity();
}

void CloudAndWeatherPresenter::deactivate()
{

}

void CloudAndWeatherPresenter::updateWeatherView(struct weatherData *data)
{
    view.updateAllContainers();
}

void CloudAndWeatherPresenter::updateWiFiInformaion(struct wifiData data[], int numberOfPoints)
{
    view.updateWiFiInformaion(data, numberOfPoints);
}

void CloudAndWeatherPresenter::setSelectedWifi(Unicode::UnicodeChar *password, int id)
{
    model->setWifiPassword(password, id);
}

void CloudAndWeatherPresenter::updateWifiIcon(int strength)
{
    view.updateWifiIcon(strength);
}

void CloudAndWeatherPresenter::switchCity()
{
    cityCounter++;
//    if (cityCounter >= NUMBERS_OF_CITYS)
    if (cityCounter >= model->getCitiesNbr())
    {
        cityCounter = 0;
    }
    model->updateWeather(cityCounter);    
}

void CloudAndWeatherPresenter::setMetric(bool isMetric)
{
    model->setMetric(isMetric);
    model->updateWeather(cityCounter);
}

bool CloudAndWeatherPresenter::getMetric()
{
    return model->getMetric();
}

void CloudAndWeatherPresenter::setClockFormat(bool isHour24)
{
    model->setClockFormat(isHour24);
    model->updateWeather(cityCounter);
}

bool CloudAndWeatherPresenter::getClockFormat()
{
    return model->getClockFormat();    
}

void CloudAndWeatherPresenter::getWifiAccessPoints(wifiData &ap, int id)
{
    model->getWifiAccessPoints(ap, id);
}

void CloudAndWeatherPresenter::getWeatherData(weatherData *weather, int CityVal)
{
    model->getWeather(weather, CityVal);
}

void CloudAndWeatherPresenter::wifiConnection(bool succesful)
{
    view.wifiConnection(succesful);
}

int CloudAndWeatherPresenter::getCitiesNbr()
{
    return model->getCitiesNbr();    
}

void CloudAndWeatherPresenter::setInSettings(bool inSettings)
{
   model->setInSettings(inSettings);
}

void CloudAndWeatherPresenter::returnToMenuPressed()
{
    model->returnToMenuLanucher();
}
