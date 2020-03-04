#ifndef CLOUDANDWEATHERVIEW_HPP
#define CLOUDANDWEATHERVIEW_HPP

#include <gui_generated/cloudandweather_screen/CloudAndWeatherViewBase.hpp>
#include <gui/cloudandweather_screen/CloudAndWeatherPresenter.hpp>
#include <gui/common/CustomKeyboard.hpp>

#include "SwipeContainerWithCallback.hpp"

class CloudAndWeatherView : public CloudAndWeatherViewBase
{
public:
    CloudAndWeatherView();
    virtual ~CloudAndWeatherView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void updateWifiIcon(int8_t strength);

    void updateWiFiInformaion(struct wifiData data[], uint16_t numberOfPoints);

    void updateBackground(weatherData *data);

    virtual void updateClockFormat(bool value);
    virtual void updateTemperatureUnit(bool value);
    virtual void settingsPressed();
    virtual void closeButtonPressed();
    virtual void wifiRetryButtonPressed();

    void updateForecastContainer(forecastWheelContainer &forecast, uint16_t centerId);
    void updateAllContainers();
    void wifiConnection(bool succesful);

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleTickEvent();

protected:

    static const int WAIT_FOR_BG_ANIMATION = 50;

    SwipeContainerWithCallback swipeContainerForecast;
    CustomKeyboard wifiKeyboard;

    void wifiAccessPointSelected(int id);
    Callback<CloudAndWeatherView, int> wifiAccessPointSelectedCallback;

    void forecastScrollContainerNewCenter(bool movedRight);
    Callback<CloudAndWeatherView, bool> forecastScrollContainerNewCenterCallback;

    void forecastContainerNewCenterCanceled();
    Callback<CloudAndWeatherView> forecastContainerNewCenterCanceledCallback;

    void backdgroundAnimationEndded(const FadeAnimator<TiledImage> & src);
    Callback<CloudAndWeatherView, const FadeAnimator< TiledImage>& > backdgroundAnimationEnddedCallback;

    void weatherAnimationEndded();
    Callback<CloudAndWeatherView> weatherAnimationEnddedCallback;

    void keyboardDonePressed(Unicode::UnicodeChar buffer[]);
    Callback<CloudAndWeatherView, Unicode::UnicodeChar[]> keyboardDonePressedCallback;

    int centerId;
    int waitForBGChangeCounter;
    bool fadeAnimationCanceld;
    int selectedWiFiID;
    bool connectionIsEstablised;
    int connectionCnt;
};


#endif // CLOUDANDWEATHERVIEW_HPP
