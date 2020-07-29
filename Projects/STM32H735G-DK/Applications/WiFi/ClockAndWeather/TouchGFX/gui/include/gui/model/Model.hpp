#ifndef MODEL_HPP
#define MODEL_HPP

#include <gui/common/structs.hpp>

class ModelListener;

class Model
{
public:
    Model();

    void bind(ModelListener* listener)
    {
        modelListener = listener;
    }

    void tick();
    void updateWeather(int CityVal);
    void updateWifi();

    void setMetric(bool isMetric);
    bool getMetric();

    void setClockFormat(bool isHour24);
    bool getClockFormat();

    void getWifiAccessPoints(wifiData &ap, int id);
    void setWifiPassword(touchgfx::Unicode::UnicodeChar *password, int id);

    void getWeather(weatherData *weather, int CityVal);

    void wifiConnection(bool succesful) {}

    void setInSettings(bool inSettings);
    
    void setIsWifiConected(bool isWifiConected);
    bool getIsWifiConected();
    
    int  getCitiesNbr(void);
    
    char* getCityName(int CityVal);
    
    void updateWifiSignalStrength(void);
    
    void returnToMenuLanucher();

protected:
    ModelListener* modelListener;

    bool metric;
    bool hour24;
    int tickCnt;

    struct wifiData wifiAccessPoints[20];
    int numberOfWifiAccesPoints;
    bool getWifiData;
    selectedWifi currentConnectedWifi;

    bool connectingToWifi;
    bool isWifiConected;
    bool inSettings;
    weatherData cityWeather_C[6];
    weatherData cityWeather_F[6];

};

#endif // MODEL_HPP
