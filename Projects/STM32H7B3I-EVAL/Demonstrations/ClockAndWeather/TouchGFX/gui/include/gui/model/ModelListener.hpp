#ifndef MODELLISTENER_HPP
#define MODELLISTENER_HPP

#include <gui/model/Model.hpp>

class ModelListener
{
public:
    ModelListener() : model(0) {}
    
    virtual ~ModelListener() {}

    void bind(Model* m)
    {
        model = m;
    }

    virtual void updateWeatherView(struct weatherData *data) {}
    virtual void updateWifiIcon(int strength) {}
    virtual void updateWiFiInformaion(struct  wifiData data[], int numberOfPoints) {}
    virtual void wifiConnection(bool succesful) {}

protected:
    Model* model;
};

#endif // MODELLISTENER_HPP
