#include <gui/cloudandweather_screen/CloudAndWeatherView.hpp>
#include "BitmapDatabase.hpp"

#include <gui/common/enums.hpp>
#include <gui/common/structs.hpp>
#include <gui/common/ClockAndDate.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CloudAndWeatherView::CloudAndWeatherView() :
    wifiAccessPointSelectedCallback(this, &CloudAndWeatherView::wifiAccessPointSelected),
    backdgroundAnimationEnddedCallback(this, &CloudAndWeatherView::backdgroundAnimationEndded),
    weatherAnimationEnddedCallback(this, &CloudAndWeatherView::weatherAnimationEndded),
    keyboardDonePressedCallback(this, &CloudAndWeatherView::keyboardDonePressed),
    centerId(1),
    waitForBGChangeCounter(WAIT_FOR_BG_ANIMATION),
    fadeAnimationCanceld(false),
    connectionIsEstablised(false)
{
}

void CloudAndWeatherView::setupScreen()
{
    CloudAndWeatherViewBase::setupScreen();
    settingsMenu.setAction(wifiAccessPointSelectedCallback);
    wifiMenu.setAction(wifiAccessPointSelectedCallback);

    gradientBackground.setFadeAnimationEndedAction(backdgroundAnimationEnddedCallback);
    forecast1.setAnimationEnddedAction(weatherAnimationEnddedCallback);

    remove(rightForecast);
    remove(centerForecast);
    remove(leftForecast);    
    
    swipeContainerForecast.add(rightForecast);
    swipeContainerForecast.add(centerForecast);
    swipeContainerForecast.add(leftForecast);
    
    swipeContainerForecast.setXY(0, 0);
    swipeContainerForecast.setSwipeCutoff(200);
    swipeContainerForecast.setEndSwipeElasticWidth(0);
    swipeContainerForecast.setSelectedPage(1);
    swipeContainerForecast.setNewCenterAction(forecastScrollContainerNewCenterCallback);   
    swipeContainerForecast.setNewCenterCanceledAction(forecastContainerNewCenterCanceledCallback);

    updateForecastContainer(forecast2, centerId);
    
    swipeContainerForecast.setTouchable(false);
    add(swipeContainerForecast);
    

    wifiKeyboard.setPosition(0, 20, 800, 460);
    wifiKeyboard.setKeyboardDoneReturnPressedAction(keyboardDonePressedCallback);
    wifiKeyboard.setVisible(false);
    add(wifiKeyboard);

    weatherData weather;
    presenter->getWeatherData(&weather, centerId);
    updateBackground(&weather);
    gradientBackground2.setVisible(false);

    remove(settingsMenu);
    remove(wifiMenu);
    remove(wifiIcon);
    remove(settingsButton);
    remove(closeButton);
    add(settingsMenu);
    add(wifiMenu);
    add(wifiIcon);
    add(settingsButton);
    add(closeButton);
}

void CloudAndWeatherView::tearDownScreen()
{
    CloudAndWeatherViewBase::tearDownScreen();
}

void CloudAndWeatherView::updateWiFiInformaion(struct  wifiData data[], uint16_t numberOfPoints)
{
    wifiMenu.updateWifiList(data, numberOfPoints);
    settingsMenu.updateWifiList(data, numberOfPoints);
}

void CloudAndWeatherView::updateWifiIcon(int8_t strength)
{
    switch (strength)
    {
    case WIFI_STRENGTH::NONE:
        wifiIcon.setBitmap(Bitmap(BITMAP_NO_WIFI_ID));
        break;

    case WIFI_STRENGTH::MINIMAL:
        wifiIcon.setBitmap(Bitmap(BITMAP_WIFI_DOT_ID));
        break;

    case WIFI_STRENGTH::LOW:
        wifiIcon.setBitmap(Bitmap(BITMAP_WIFI_LOW_ID));
        break;

    case WIFI_STRENGTH::MEDIUM:
        wifiIcon.setBitmap(Bitmap(BITMAP_WIFI_MID_ID));
        break;

    case WIFI_STRENGTH::HIGH:
        wifiIcon.setBitmap(Bitmap(BITMAP_WIFI_FULL_ID));
        break;
    }
    wifiIcon.invalidate();
}
    
void CloudAndWeatherView::updateClockFormat(bool value)
{
    presenter->setClockFormat(value);
}

void CloudAndWeatherView::updateTemperatureUnit(bool value)
{
    presenter->setMetric(value);
}

void CloudAndWeatherView::settingsPressed()
{
    connectionIsEstablised = false;

    swipeContainerForecast.setVisible(false);
    wifiMenu.setVisible(false);
    wifiKeyboard.setVisible(false);
    wifiConnecting.setVisible(false);

    wifiMenu.invalidate();
    wifiKeyboard.invalidate();
    wifiConnecting.invalidate();
    swipeContainerForecast.invalidate();

    wifiKeyboard.clearBuffer();
    presenter->setInSettings(true);
}

void CloudAndWeatherView::closeButtonPressed()
{
    connectionIsEstablised = false;
	
	if(swipeContainerForecast.isVisible())
    {
      presenter->returnToMenuPressed();
    }
    settingsMenu.setVisible(false);
    wifiMenu.setVisible(false);
    wifiKeyboard.setVisible(false);
    wifiConnecting.setVisible(false);

    settingsMenu.invalidate();
    wifiMenu.invalidate();
    wifiKeyboard.invalidate();
    wifiConnecting.invalidate();

    swipeContainerForecast.setVisible(true);
    swipeContainerForecast.invalidate();

    wifiKeyboard.clearBuffer();
	presenter->setInSettings(false);
}

void CloudAndWeatherView::wifiAccessPointSelected(int id)
{
    wifiData selectedAP;
    presenter->getWifiAccessPoints(selectedAP, id);
    selectedWiFiID = id;

    settingsMenu.setVisible(false);
    wifiMenu.setVisible(false);

    settingsMenu.invalidate();
    wifiMenu.invalidate();
    
    presenter->setInSettings(false);

    if (selectedAP.encrypted)
    {
        wifiKeyboard.setVisible(true);
        wifiKeyboard.invalidate();
    }
    else
    {
        keyboardDonePressed(0);
    }
}


void CloudAndWeatherView::updateAllContainers()
{
    rightForecast.removeAll();
    centerForecast.removeAll();
    leftForecast.removeAll();

    centerForecast.add(forecast2);

    updateForecastContainer(forecast2, centerId);

    swipeContainerForecast.invalidate();
}

void CloudAndWeatherView::updateForecastContainer(forecastWheelContainer &forecast, uint16_t centerId)
{
    weatherData weather;
    
    presenter->getWeatherData(&weather, centerId);
    forecast.setLocation(weather.city);
    forecast.updateCurrentTemp(weather.temperature[0], presenter->getMetric());
    forecast.updateForecast(&weather, presenter->getMetric());
    forecast.setDateAndTime(weather.dates[0], weather.timezone, presenter->getClockFormat());
    forecast.updateCurrentWeatherIcon(&weather);
    forecast.updateCurrentWeatherInformation(&weather, presenter->getMetric());
    updateBackground(&weather);
}

void CloudAndWeatherView::updateBackground(weatherData *data)
{
    int hour, minute;
    char temp[3];

    memcpy(temp, &data->dates[0][11], 2);
    temp[2] = '\0';
    hour = atoi(temp);

    memcpy(temp, &data->dates[0][14], 2);
    temp[2] = '\0';
    minute = atoi(temp);

    uint16_t timezoneCompentation = 0;
    int hourBuffer;

    if ((minute + 30) > 60)
    {
        timezoneCompentation = 1;
    }

    int hourWithTimezone = hour + (data->timezone / 60) / 60 + timezoneCompentation;

    if (hourWithTimezone < 24 && hourWithTimezone >= 0)
    {
        hourBuffer = hourWithTimezone;
    }
    else if (hourWithTimezone >= 24)
    {
        hourBuffer = hourWithTimezone - 24;
    }
    else
    {
        hourBuffer = hourWithTimezone + 24;
    }

    if ((data->icon[0][2]) == 'n')
    {
        gradientBackground.setBitmap(Bitmap(BITMAP_NIGHT_ID));
    }
    else if (hourBuffer < 10)
    {
        gradientBackground.setBitmap(Bitmap(BITMAP_SUNRISE_ID));
    }
    else if (hourBuffer < 16)
    {
        gradientBackground.setBitmap(Bitmap(BITMAP_MIDDAY_ID));
    }
    else
    {
        gradientBackground.setBitmap(Bitmap(BITMAP_AFTERNOON_ID));
    }

    gradientBackground.setWidth(HAL::DISPLAY_WIDTH);
    gradientBackground.setHeight(HAL::DISPLAY_HEIGHT);
}

void CloudAndWeatherView::forecastContainerNewCenterCanceled()
{
    /*
    forecast1.fadeWeatherElements(255, 30, 0);
    forecast2.fadeWeatherElements(255, 30, 0);
    forecast3.fadeWeatherElements(255, 30, 0);
    */
}

void CloudAndWeatherView::keyboardDonePressed(Unicode::UnicodeChar buffer[])
{
    presenter->setSelectedWifi(buffer, selectedWiFiID);
    wifiConnecting.resetContainer();

    wifiKeyboard.clearBuffer();

    wifiKeyboard.setVisible(false);
    wifiConnecting.setVisible(true);
    
    settingsButton.setVisible(false);
    closeButton.setVisible(false);
    settingsButton.invalidate();
    closeButton.invalidate();
   /**/ 
   
    wifiKeyboard.invalidate();
    wifiConnecting.invalidate();
}

void CloudAndWeatherView::wifiRetryButtonPressed()
{
    wifiKeyboard.setVisible(true);
    wifiKeyboard.invalidate();
    
    settingsButton.setVisible(true);
    closeButton.setVisible(true);
    settingsButton.invalidate();
    closeButton.invalidate();
}

void CloudAndWeatherView::wifiConnection(bool succesful)
{
    if (wifiConnecting.isVisible())
    {
        if (succesful)
        {
            wifiData selectedAP;
            presenter->getWifiAccessPoints(selectedAP, selectedWiFiID);            
            wifiConnecting.connectionSuccesfull(selectedAP.signalStrength);
            connectionCnt = 0;
            connectionIsEstablised = true;
			
            closeButtonPressed();
            /*added by GHA*/
            settingsButton.setVisible(true);
            closeButton.setVisible(true);
            settingsButton.invalidate();
            closeButton.invalidate();
        }
        else
        {
            updateWifiIcon(WIFI_STRENGTH::NONE);
            wifiConnecting.connectionUnsuccesfull();
        }
    }
}

void CloudAndWeatherView::handleClickEvent(const ClickEvent& evt)
{
    if (gradientBackground.isRunning())
    {
        gradientBackground.cancelFadeAnimation();
        fadeAnimationCanceld = true;

        gradientBackground.setAlpha(255);
        gradientBackground.invalidate();
        gradientBackground2.setAlpha(0);
        gradientBackground2.invalidate();
    }
    if (gradientBackground2.isRunning())
    {
        gradientBackground2.cancelFadeAnimation();
        fadeAnimationCanceld = true;
    }
    CloudAndWeatherViewBase::handleClickEvent(evt);
}

void CloudAndWeatherView::backdgroundAnimationEndded(const FadeAnimator<TiledImage>& src)
{
    forecast1.fadeWeatherElements(255, 30, 0);
    forecast2.fadeWeatherElements(255, 30, 0);
    forecast3.fadeWeatherElements(255, 30, 0);

    gradientBackground2.setAlpha(255);
    gradientBackground2.setVisible(false);
    gradientBackground2.invalidate();
}

void CloudAndWeatherView::weatherAnimationEndded()
{

}

void CloudAndWeatherView::handleTickEvent()
{
    if (waitForBGChangeCounter > 0)
    {
        waitForBGChangeCounter--;
        if (waitForBGChangeCounter == 0)
        {
            weatherData weather;

            if (fadeAnimationCanceld)
            {
                gradientBackground.setAlpha(255);
                gradientBackground2.setVisible(false);
                presenter->getWeatherData(&weather, centerId);
                updateBackground(&weather);

                fadeAnimationCanceld = false;
                gradientBackground.invalidate();
            }
            else
            {
                gradientBackground2.setBitmap(Bitmap(gradientBackground.getBitmap()));
                gradientBackground2.setWidth(HAL::DISPLAY_WIDTH);
                gradientBackground2.setHeight(HAL::DISPLAY_HEIGHT);

                presenter->getWeatherData(&weather, centerId);
                updateBackground(&weather);

                if (gradientBackground2.getBitmap() != gradientBackground.getBitmap())
                {
                    gradientBackground2.setAlpha(255);
                    gradientBackground2.setVisible(true);
                    gradientBackground.setAlpha(0);

                    gradientBackground2.startFadeAnimation(0, 40);
                    gradientBackground.setFadeAnimationDelay(20);
                    gradientBackground.startFadeAnimation(255, 40);
                }
            }
        }
    }
    if (wifiConnecting.isVisible())
    {
        wifiConnecting.tickEvent();
        if (connectionIsEstablised)
        {
            if (connectionCnt > 170)
            {
                wifiConnecting.setVisible(false);
                connectionCnt = 0;
                connectionIsEstablised = false;
                closeButtonPressed();
            }
            connectionCnt++;
        }
    }
}
