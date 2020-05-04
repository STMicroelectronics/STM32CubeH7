#ifndef FORECASTWHEELCONTAINER_HPP
#define FORECASTWHEELCONTAINER_HPP

#include <gui_generated/containers/forecastWheelContainerBase.hpp>

#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/containers/forecastBarContainer.hpp>
#include <gui/containers/currentWeatherDataContainer.hpp>


class forecastWheelContainer : public forecastWheelContainerBase
{
public:
    forecastWheelContainer();
    virtual ~forecastWheelContainer() {}

    virtual void initialize();

    void setLocation(char newLocation[]);
    void updateCurrentTemp(int temperature, bool metric);
    void updateForecast(struct weatherData *data, bool metric);
    void setDateAndTime(const char* currentTime, int32_t timzone, bool hour24);
    void updateCurrentWeatherIcon(weatherData *data);
    void updateCurrentWeatherInformation(weatherData *data, bool metric);

    void fadeWeatherElements(uint8_t endAlpha, uint16_t duration, uint16_t delay, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone);
    
    void convertToUppercase(char *word, int size);

    bool isAnimating();
    uint8_t getAlpha();

    void setAnimationEnddedAction(GenericCallback<>& callback);

protected:

    FadeAnimator<forecastBarContainer> forecast5days;
    FadeAnimator<currentWeatherDataContainer> currentWeatherData;

    void weatherElementsAnimationEndded(const FadeAnimator<Image> & src);
    Callback<forecastWheelContainer, const FadeAnimator< Image>& > weatherElementsAnimationEnddedCallback;

    GenericCallback<>* animationEnddedCallback;

    bool inFocus;
    int id;
};

#endif // FORECASTWHEELCONTAINER_HPP
