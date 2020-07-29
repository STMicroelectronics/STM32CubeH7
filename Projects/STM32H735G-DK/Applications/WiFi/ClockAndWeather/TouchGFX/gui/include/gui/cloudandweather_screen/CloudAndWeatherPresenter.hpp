#ifndef CLOUDANDWEATHERPRESENTER_HPP
#define CLOUDANDWEATHERPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

//#include "json_parsers.h"

using namespace touchgfx;

class CloudAndWeatherView;

class CloudAndWeatherPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CloudAndWeatherPresenter(CloudAndWeatherView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~CloudAndWeatherPresenter() {};

    virtual void updateWeatherView(struct weatherData *data);

    virtual void updateWiFiInformaion(struct  wifiData data[], int numberOfPoints);

    virtual void updateWifiIcon(int strength);

    void switchCity();

    void setMetric(bool isMetric);
    bool getMetric();

    void setClockFormat(bool isHour24);
    bool getClockFormat();

    void getWifiAccessPoints(wifiData &ap, int id);
    void setSelectedWifi(Unicode::UnicodeChar *password, int id);

    void getWeatherData(weatherData *weather, int CityVal);

    virtual void wifiConnection(bool succesful);
    
    int getCitiesNbr();
    
    void setInSettings(bool inSettings);

    void returnToMenuPressed();

private:
    CloudAndWeatherPresenter();

    CloudAndWeatherView& view;

    int cityCounter;
};


#endif // CLOUDANDWEATHERPRESENTER_HPP
