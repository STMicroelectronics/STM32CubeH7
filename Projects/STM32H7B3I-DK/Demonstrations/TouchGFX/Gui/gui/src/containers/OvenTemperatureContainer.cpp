#include <gui/containers/OvenTemperatureContainer.hpp>
#include "BitmapDatabase.hpp"

OvenTemperatureContainer::OvenTemperatureContainer() :
    currentTemp(0),
    lastDragX(0),
    firstImage(BITMAP_TEMP_WHEEL_01_ID),
    lastImage(BITMAP_TEMP_WHEEL_08_ID),
    wheelClicked(false),
    scrollDuration(0),
    beginningValue(0),
    targetValue(0),
    animationCounter(0),
    animate(false),
    lastGesture(0),
    gestureSteps(0),
    gestureRight(false)
{
   
}

void OvenTemperatureContainer::initialize()
{
    OvenTemperatureContainerBase::initialize();
    currentImage = tempWheel.getBitmap();
}

void OvenTemperatureContainer::setAlpha(uint8_t alpha)
{
    tempWheel.setAlpha(alpha);
    ovenHeat.setAlpha(alpha);
    selectedTemp.setAlpha(alpha);
}

uint8_t OvenTemperatureContainer::getAlpha()
{
    return tempWheel.getAlpha();
}

void OvenTemperatureContainer::setCurrentTemp(uint16_t temp)
{
    currentTemp = temp;
    Unicode::snprintf(selectedTempBuffer, SELECTEDTEMP_SIZE, "%d", currentTemp);
    selectedTemp.invalidate();
}

uint16_t OvenTemperatureContainer::getCurrnetTemp()
{
    return currentTemp;
}

void OvenTemperatureContainer::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        if (tempWheel.getRect().intersect(Rect(evt.getX(), evt.getY(), 1, 1)))
        {
            wheelClicked = true;
        }
        if (animate)
        {
            animate = false;
            Application::getInstance()->unregisterTimerWidget(this);
            animationCounter = 0;
        }
    }
    else if (evt.getType() == ClickEvent::RELEASED)
    {
        wheelClicked = false;
    }
}

void OvenTemperatureContainer::handleDragEvent(const DragEvent& evt)
{
    int dragDistance = evt.getDeltaX() *  evt.getDeltaX() + evt.getDeltaY() *  evt.getDeltaY();

    int tempChange = 0;

    gestureRight = evt.getDeltaX() > 0 ? true : false;
    wheelClicked = evt.getDeltaX() == 0 ? false : true;

    if (wheelClicked)
    {
        lastDragX = lastDragX + (gestureRight ? 1 : -1) * squareRoot(dragDistance);
        if (lastDragX > 0)
        {
            if (lastDragX / WHEEL_STEPS > lastImage - firstImage)
            {
                currentImage = currentImage + (lastImage - firstImage);
                tempChange = lastImage - firstImage + 1;
            }
            else
            {
                currentImage = currentImage + (lastDragX / WHEEL_STEPS);
                tempChange = lastDragX / WHEEL_STEPS;
            }

            if (currentImage > lastImage)
            {
                currentImage = firstImage + (currentImage - lastImage - 1);
            }
        }
        else if (lastDragX < 0)
        {
            if (lastDragX / WHEEL_STEPS < -(lastImage - firstImage))
            {
                currentImage = currentImage - (lastImage - firstImage);
                tempChange = -(lastImage - firstImage + 1);
            }
            else
            {
                currentImage = currentImage + (lastDragX / WHEEL_STEPS);
                tempChange = lastDragX / WHEEL_STEPS;
            }

            if (currentImage < firstImage)
            {
                if (currentImage <= 0)
                {
                    currentImage = lastImage + (currentImage - firstImage + 1);
                }
                else
                {
                    currentImage = lastImage - (firstImage - currentImage - 1);
                }
            }
        }

        if (currentTemp + tempChange > 0 && currentTemp + tempChange < 300)
        {
            currentTemp = currentTemp + tempChange;
        }
        else if (currentTemp + tempChange <= 0)
        {
            currentTemp = 0;
        }
        else if (currentTemp + tempChange >= 300)
        {
            currentTemp = 300;
        }

        Unicode::snprintf(selectedTempBuffer, SELECTEDTEMP_SIZE, "%d", currentTemp);
        selectedTemp.invalidate();

        lastDragX = lastDragX % WHEEL_STEPS;
        tempWheel.setBitmap(Bitmap(currentImage));
        tempWheel.invalidate();
    }
}

void OvenTemperatureContainer::handleGestureEvent(const GestureEvent& evt)
{
    int16_t velocityAbsolute = abs(evt.getVelocity());

    if (velocityAbsolute < 1)
    {
        return;
    }

    velocityAbsolute = MAX(MIN(velocityAbsolute, 17), 5);

    scrollDuration = 7 * velocityAbsolute;
    targetValue = (gestureRight ? 1 : -1) * (velocityAbsolute - 4) * 72;

    beginningValue = 0;
    animate = true;
    animationCounter = 0;
    lastGesture = 0;
    gestureSteps = 0;
    Application::getInstance()->registerTimerWidget(this);
}

void OvenTemperatureContainer::handleTickEvent()
{
    if (animate)
    {
        animationCounter++;
        if (animationCounter <= scrollDuration)
        {
            int16_t calculatedValue = EasingEquations::cubicEaseOut(animationCounter, beginningValue, targetValue, scrollDuration);
            gestureWheel(calculatedValue - gestureSteps);
            gestureSteps = calculatedValue;
        }
        else
        {
            animate = false;
        }

        if (!animate)
        {
            Application::getInstance()->unregisterTimerWidget(this);
            animationCounter = 0;
        }
    }
}

void OvenTemperatureContainer::stopWheel()
{
    animate = false;
    Application::getInstance()->unregisterTimerWidget(this);
    animationCounter = 0;
}

void OvenTemperatureContainer::gestureWheel(int16_t moved)
{
    int tempChange = 0;

    lastGesture = lastGesture + moved;
    if (lastGesture > 0)
    {
        if (lastGesture / WHEEL_STEPS > lastImage - firstImage)
        {
            currentImage = currentImage + (lastImage - firstImage);

            tempChange = lastImage - firstImage + 1;
        }
        else
        {
            currentImage = currentImage + (lastGesture / WHEEL_STEPS);
            tempChange = lastGesture / WHEEL_STEPS;
        }

        if (currentImage > lastImage)
        {
            currentImage = firstImage + (currentImage - lastImage - 1);
        }
    }
    else if (lastGesture < 0)
    {
        if (lastGesture / WHEEL_STEPS < -(lastImage - firstImage))
        {
            currentImage = currentImage - (lastImage - firstImage);
            tempChange = -(lastImage - firstImage + 1);
        }
        else
        {
            currentImage = currentImage + (lastGesture / WHEEL_STEPS);
            tempChange = lastGesture / WHEEL_STEPS;
        }

        if (currentImage < firstImage)
        {
            if (currentImage <= 0)
            {
                currentImage = lastImage + (currentImage - firstImage + 1);
            }
            else
            {
                currentImage = lastImage - (firstImage - currentImage - 1);
            }
        }
    }

    if (currentTemp + tempChange > 0 && currentTemp + tempChange < 300)
    {
        currentTemp = currentTemp + tempChange;
    }
    else if (currentTemp + tempChange <= 0)
    {
        currentTemp = 0;
    }
    else if (currentTemp + tempChange >= 300)
    {
        currentTemp = 300;
    }

    Unicode::snprintf(selectedTempBuffer, SELECTEDTEMP_SIZE, "%d", currentTemp);
    selectedTemp.invalidate();

    lastGesture = lastGesture % WHEEL_STEPS;
    tempWheel.setBitmap(Bitmap(currentImage));
    tempWheel.invalidate();
}

int OvenTemperatureContainer::squareRoot(int x)
{
    if (x == 0 || x == 1)
    {
        return x;
    }

    int start = 1, end = x, ans = 0;
    while (start <= end)
    {
        int mean = (start + end) / 2;

        if (mean * mean == x)
        {
            return mean;
        }

        if (mean * mean < x)
        {
            start = mean + 1;
            ans = mean;
        }
        else
        {
            end = mean - 1;
        }
    }
    return ans;
}
