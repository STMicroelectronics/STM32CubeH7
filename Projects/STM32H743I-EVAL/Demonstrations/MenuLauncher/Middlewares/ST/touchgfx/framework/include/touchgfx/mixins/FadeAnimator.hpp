/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#ifndef FADEANIMATOR_HPP
#define FADEANIMATOR_HPP

#include <touchgfx/Application.hpp>
#include <touchgfx/Callback.hpp>
#include <touchgfx/EasingEquations.hpp>

namespace touchgfx
{
/**
 * @class FadeAnimator FadeAnimator.hpp touchgfx/mixins/FadeAnimator.hpp
 *
 * @brief A FadeAnimator makes the template class T able to animate an alpha fade.
 *
 *        A FadeAnimator makes the template class T able to animate an alpha fade from its
 *        current alpha value to a specified end alpha value. The alpha development can be
 *        described by supplying an EasingEquation. The FadeAnimator performs a callback when
 *        the animation has finished.
 *
 *        This mixin can be used on any Drawable that has a 'void setAlpha(uint8_t)' and a
 *        'uint8_t getAlpha()' method.
 *
 * @tparam T Specifies the type should have the fade animation capability.
 */
template<class T>
class FadeAnimator : public T
{
public:

    /**
     * @fn FadeAnimator::FadeAnimator()
     *
     * @brief Default constructor.
     *
     *        Default constructor. Creates and initialize the FadeAnimator.
     */
    FadeAnimator() :
        T(),
        fadeAnimationRunning(false),
        fadeAnimationCounter(0),
        fadeAnimationDelay(0),
        fadeAnimationEndedCallback(0)
    {
    }

    /**
     * @fn virtual FadeAnimator::~FadeAnimator()
     *
     * @brief Destructor.
     *
     *        Destructor. Destroys the FadeAnimator.
     */
    virtual ~FadeAnimator()
    {
    }

    /**
     * @fn void FadeAnimator::setFadeAnimationEndedAction(GenericCallback<const FadeAnimator<T>& >& callback)
     *
     * @brief Associates an action to be performed when the animation ends.
     *
     *        Associates an action to be performed when the animation ends.
     *
     * @param callback The callback to be executed. The callback will be given a reference to
     *                 the FadeAnimator.
     *
     * @see GenericCallback
     */
    void setFadeAnimationEndedAction(GenericCallback<const FadeAnimator<T>& >& callback)
    {
        fadeAnimationEndedCallback = &callback;
    }

    /**
     * @fn void FadeAnimator::clearFadeAnimationEndedAction()
     *
     * @brief Clears the fade animation ended action previously set by setFadeAnimationEndedAction.
     *
     *        Clears the fade animation ended action previously set by
     *        setFadeAnimationEndedAction.
     *
     * @see setFadeAnimationEndedAction
     */
    void clearFadeAnimationEndedAction()
    {
        fadeAnimationEndedCallback = 0;
    }

    /**
     * @fn virtual void FadeAnimator::setFadeAnimationDelay(uint16_t delay)
     *
     * @brief Sets a delay on animations done by the FadeAnimator.
     *
     *        Sets a delay on animations done by the FadeAnimator.
     *
     * @param delay The delay in ticks.
     */
    virtual void setFadeAnimationDelay(uint16_t delay)
    {
        fadeAnimationDelay = delay;
    }

    /**
     * @fn virtual uint16_t FadeAnimator::getFadeAnimationDelay() const
     *
     * @brief Gets the current animation delay.
     *
     *        Gets the current animation delay.
     *
     * @return The current animation delay.
     */
    virtual uint16_t getFadeAnimationDelay() const
    {
        return fadeAnimationDelay;
    }

    /**
     * @fn virtual bool FadeAnimator::isRunning() const
     *
     * @brief Gets whether or not the fade animation is running.
     *
     *        Gets whether or not the fade animation is running.
     *
     * @return true if the fade animation is running.
     * @deprecated use isFadeAnimationRunning()
     */
    virtual bool isRunning() const
    {
        return isFadeAnimationRunning();
    }

    /**
     * @fn virtual bool FadeAnimator::isFadeAnimationRunning() const
     *
     * @brief Gets whether or not the fade animation is running.
     *
     *        Gets whether or not the fade animation is running.
     *
     * @return true if the fade animation is running.
     */
    virtual bool isFadeAnimationRunning() const
    {
        return fadeAnimationRunning;
    }

    /**
     * @fn void FadeAnimator::startFadeAnimation(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone)
     *
     * @brief Starts the fade animation.
     *
     *        Starts the fade animation from the current alpha value to the specified end alpha
     *        value. The development of the alpha value during the animation is described by
     *        the supplied EasingEquation.
     *
     * @param endAlpha                 The alpha value of T at animation end.
     * @param duration                 The duration of the animation measured in ticks.
     * @param alphaProgressionEquation The equation that describes the development of the alpha
     *                                 value during the animation. Default =
     *                                 EasingEquations::linearEaseNone.
     */
    void startFadeAnimation(uint8_t endAlpha, uint16_t duration, EasingEquation alphaProgressionEquation = &EasingEquations::linearEaseNone)
    {
        if (!fadeAnimationRunning)
        {
            Application::getInstance()->registerTimerWidget(this);
        }

        fadeAnimationCounter = 0;
        fadeAnimationStartAlpha = T::getAlpha();
        fadeAnimationEndAlpha = endAlpha;
        fadeAnimationDuration = duration;
        fadeAnimationAlphaEquation = alphaProgressionEquation;

        fadeAnimationRunning = true;

        if (fadeAnimationDelay == 0 && fadeAnimationDuration == 0)
        {
            nextFadeAnimationStep(); // Set end alpha and shut down
        }
    }

    /**
     * @fn void FadeAnimator::cancelFadeAnimation()
     *
     * @brief Cancel fade animation.
     */
    void cancelFadeAnimation()
    {
        if (fadeAnimationRunning)
        {
            Application::getInstance()->unregisterTimerWidget(this);
            fadeAnimationRunning = false;
        }
    }

protected:

    /**
     * @fn virtual void FadeAnimator::handleTickEvent()
     *
     * @brief The tick handler that handles the actual animation steps.
     *
     *        The tick handler that handles the actual animation steps.
     */
    virtual void handleTickEvent()
    {
        T::handleTickEvent();

        nextFadeAnimationStep();
    }

    /**
     * @fn void FadeAnimator::nextFadeAnimationStep()
     *
     * @brief Execute next step in fade animation.
     *
     *        Execute next step in fade animation and stop the timer if necessary.
     */
    void nextFadeAnimationStep()
    {
        if (fadeAnimationRunning)
        {
            if (fadeAnimationCounter < fadeAnimationDelay)
            {
                // Just wait for the delay time to pass
                fadeAnimationCounter++;
            }
            else
            {
                if (fadeAnimationCounter <= (uint32_t)(fadeAnimationDelay + fadeAnimationDuration))
                {
                    // Adjust the used animationCounter for the startup delay
                    uint32_t actualAnimationCounter = fadeAnimationCounter - fadeAnimationDelay;

                    int16_t deltaAlpha = (int16_t)fadeAnimationAlphaEquation(actualAnimationCounter, 0, fadeAnimationEndAlpha - fadeAnimationStartAlpha, fadeAnimationDuration);

                    T::setAlpha(fadeAnimationStartAlpha + deltaAlpha);
                    T::invalidate();

                    fadeAnimationCounter++;
                }
                if (fadeAnimationCounter > (uint32_t)(fadeAnimationDelay + fadeAnimationDuration))
                {
                    // End of animation
                    fadeAnimationRunning = false;
                    fadeAnimationDuration = 0;
                    Application::getInstance()->unregisterTimerWidget(this);

                    if (fadeAnimationEndedCallback && fadeAnimationEndedCallback->isValid())
                    {
                        fadeAnimationEndedCallback->execute(*this);
                    }
                }
            }
        }
    }

protected:
    bool           fadeAnimationRunning;       ///< Boolean that is true if the animation is running.
    uint16_t       fadeAnimationCounter;       ///< Counter that is equal to the current step in the animation
    uint16_t       fadeAnimationDelay;         ///< A delay that is applied before animation start. Expressed in ticks.
    uint16_t       fadeAnimationDuration;      ///< The complete duration of the animation. Expressed in ticks.
    int16_t        fadeAnimationStartAlpha;    ///< The alpha value at the beginning of the animation.
    int16_t        fadeAnimationEndAlpha;      ///< The alpha value at the end of the animation.
    EasingEquation fadeAnimationAlphaEquation; ///< EasingEquation expressing the development of the alpha value during the animation.

    GenericCallback<const FadeAnimator<T>& >* fadeAnimationEndedCallback; ///< Animation ended Callback.
};
} //namespace touchgfx
#endif // FADEANIMATOR_HPP
