#include <gui/main_screen/MainView.hpp>
#include <touchgfx/EasingEquations.hpp>
#include "texts/TextKeysAndLanguages.hpp"
#include <touchgfx/hal/HAL.hpp>

static const float PI = 3.14159265358979323846f;
static const float PI_HALF = PI / 2;
static const float PI_QUART = PI / 4;
static const float angleFactor = 1.6f;
static const float speedFactor = 0.1f;

MainView::MainView() :
    moveAnimationEndedCallback(this, &MainView::moveAnimationEndedHandler),
    radius_x(165),
    radius_y(40),
    angle(0),
    originX(140),
    originY(0),
    mininalSize(0.5f),
    scalingFactor(1 / ((float)radius_y * 2)),
    offset(radius_y - originY),
    stepSizes(0),
    steps(0),
    stepCounter(0),
    moveDistance(0),
    startValue(0),
    closetsIcon(0),
    animate(false),
    autoAnimate(false),
    animateBounce(false),
    scaleStart(false),
    scaleCounter(0),
    tickCounter(0),
    isDragged(false),
    backIcon(3),
    lastIconChanged(0),
    moveRight(false),
    demoInfo(false),
    centerIcon(EEMBC),
    selectedIcon(0)
{
    iconsAngle[0] = 0;
    iconsAngle[1] = 0;
    iconsAngle[2] = 0;
    iconsAngle[3] = 0;

    HEADER_FADE_DURATION    = 10;
    ICONS_MOVE_DURATION     = 50;
    DESCRIPTION_Y_1         = -23;
    DESCRIPTION_Y_2 = -100;
    DESCRIPTION_Y_3 = 0;
}

void MainView::setupScreen()
{
    HAL::getInstance()->setFrameRateCompensation(true);
  
    MainViewBase::setupScreen();

    double newX, newY;        

    for (int i = 0; i < 4; i++)
    {
        iconsAngle[i] = angle + PI_HALF * i;

        newX = originX + cosf(iconsAngle[i])*radius_x;
        newY = originY + sinf(iconsAngle[i])*radius_y;

        icons[i].setXY((int)newX, (int)newY);
        icons[i].setScale((icons[i].getY() + offset) * scalingFactor);

        int newAlpha = 255 - icons[i].getY() * 7;
        if (newAlpha > 255)
        {
            newAlpha = 255;
        }
        else if (newAlpha < 0)
        {
            newAlpha = 0;
        }
        icons[i].setSideAlpha(newAlpha);        

        icons[i].setBitmap(i % NUMBER_OF_DEMOS);
        icons[i].setMoveAnimationEndedAction(moveAnimationEndedCallback);
    }

    for (int i = 0; i < 4; i++)
    {
        newX = originX + 20 + cosf(iconsAngle[i])*radius_x;
        newY = originY + 170 + sinf(iconsAngle[i])*radius_y;
        iconsShadows[i].setXY((int)newX, (int)newY);

        int subfact = 4 * (210 - iconsShadows[i].getY());
        if (subfact >= 255)
        {
            subfact = 255;
        }
        iconsShadows[i].setAlpha(255 - subfact);

        add(iconsShadows[i]);
        add(icons[i]);
    }

    centerIcon = EEMBC;
}

void MainView::tearDownScreen()
{
    MainViewBase::tearDownScreen();
}

void MainView::handleClickEvent(const ClickEvent& evt)
{
    if (demoInfo)
    {
        if (startTextureMapper.getRect().intersect(evt.getX(), evt.getY()))
        {
            switch (evt.getType())
            {
            case ClickEvent::PRESSED:
                playPressed = true;
                break;
            case ClickEvent::RELEASED:
                if (playPressed)
                {
                    presenter->startDemo(centerIcon);
                }
            default:
                break;
            }
        }
        for (int i = 0; i < 4; i++)
        {
            if (icons[i].getRect().intersect(evt.getX(), evt.getY()) && icons[i].getY() == 100 && evt.getType() == ClickEvent::RELEASED)
            {
                exitDemo();
            }
        }
    }
    else
    {
        if (evt.getType() == ClickEvent::PRESSED)
        {
            animate = false;
            isDragged = false;

            float b = evt.getX() - originX;
            float tempY = evt.getY() > 150 ? (float)evt.getY() : 150.f;
            float a = tempY - originY;
            float c = sqrtf(a * a + b * b);

            if (evt.getX() >= originX)
            {
                lastAngle = asinf(a / c) * angleFactor;
            }
            else if (evt.getX() < originX)
            {
                lastAngle = (PI_HALF + (PI_HALF - asinf(a / c))) * angleFactor;
            }

            angle = fmodf(angle, 2 * PI);

            for (int i = 0; i < 4; i++)
            {
                iconsAngle[i] = angle + PI_HALF * i;
                if (iconsAngle[i] > 2 * PI)
                {
                    iconsAngle[i] = iconsAngle[i] - 2 * PI;
                }
                else if (iconsAngle[i] < 0)
                {
                    iconsAngle[i] = iconsAngle[i] + 2 * PI;
                }
            }
        }
        else
        {
            if (!animate && (isDragged || stepCounter < steps))
            {
                animateToClosesItem();
            }
            else if (!isDragged && !animate)
            {
                for (int i = 0; i < 4; i++)
                {
                    if (icons[i].getRect().intersect(evt.getX(), evt.getY()))
                    {
                        if (iconsAngle[i] > (2 * PI - 0.25f) || iconsAngle[i] < 0.25f)
                        {
                            header.startFadeAnimation(0, HEADER_FADE_DURATION, EasingEquations::cubicEaseOut);
                            startValue = angle;
                            moveDistance = PI_HALF;
                            stepCounter = 0;
                            steps = 40;
                            animate = true;
                        }
                        else if (iconsAngle[i] > (PI_HALF - 0.25f) && iconsAngle[i] < (PI_HALF + 0.25f))
                        {
                            enterDemo();
                        }
                        else if (iconsAngle[i] > (PI - 0.25f) && iconsAngle[i] < (PI + 0.25f))
                        {
                            header.startFadeAnimation(0, HEADER_FADE_DURATION, EasingEquations::cubicEaseOut);
                            startValue = angle;
                            moveDistance = -PI_HALF;
                            stepCounter = 0;
                            steps = 40;
                            animate = true;
                        }
                    }
                }
            }
        }
    }
    MainViewBase::handleClickEvent(evt);
}

void MainView::handleDragEvent(const DragEvent& evt)
{
    if (demoInfo)
    {
        if (!startTextureMapper.getRect().intersect(evt.getNewX(), evt.getNewY()))
        {
            playPressed = false;
        }
    }
    else
    {
        if (!isDragged)
        {
            header.startFadeAnimation(0, HEADER_FADE_DURATION, EasingEquations::cubicEaseOut);
        }

        isDragged = true;

        float tmpAngle=0;

        float b = evt.getNewX() - originX;

        float tempY = evt.getNewY() > 150 ? (float)evt.getNewY() : 150.f;
        float a = tempY - originY;
        float c = sqrtf(a * a + b * b);

        if (evt.getNewX() >= originX)
        {
            tmpAngle = asinf(a / c) * angleFactor;
        }
        else if (evt.getNewX() < originX)
        {
            tmpAngle = (PI_HALF + (PI_HALF - asinf(a / c))) * angleFactor;
        }

        moveIcons(angle - (lastAngle - tmpAngle));
        lastAngle = tmpAngle;
    }
}

void MainView::handleGestureEvent(const GestureEvent& evt)
{
    if (demoInfo)
    {
    }
    else
    {
        if (evt.getType() == GestureEvent::SWIPE_HORIZONTAL)
        {
            if (!isDragged)
            {
                header.startFadeAnimation(0, HEADER_FADE_DURATION, EasingEquations::cubicEaseOut);
            }

            float velocityAbsolute = (float)abs((double)evt.getVelocity());

            steps = floorf(velocityAbsolute * 2);
            stepCounter = 0;
            startValue = angle;
            moveDistance = ((evt.getVelocity() > 0) ? -1 : 1) * (velocityAbsolute * speedFactor);
            moveDistance = moveDistance - fmodf(moveDistance, PI_HALF);

            for (int i = 0; i < 4; i++)
            {
                while (iconsAngle[i] > 2 * PI || iconsAngle[i] < 0)
                {
                    if (iconsAngle[i] > 2 * PI)
                    {
                        iconsAngle[i] = iconsAngle[i] - 2 * PI;
                    }
                    else
                    {
                        iconsAngle[i] = iconsAngle[i] + 2 * PI;
                    }
                }
                if (abs(abs(iconsAngle[i]) - PI_HALF) < abs(abs(iconsAngle[closetsIcon]) - PI_HALF))
                {
                    closetsIcon = i;
                }
            }

            moveDistance += PI_HALF - fmodf(angle, PI_HALF);
            animate = true;
        }
    }
}

void MainView::handleTickEvent()
{
    if (animate)
    {
        float test = floatCubicEaseOut(stepCounter, startValue, moveDistance, steps);
        moveIcons(test);
        stepCounter++;
        if (stepCounter > steps)
        {
            animate = false;
            header.setTypedText(TypedText(static_cast<TypedTextId>((static_cast<int>(T_HEADEROSCILLOSCOPE) + static_cast<int>(centerIcon)))));
            header.startFadeAnimation(255, HEADER_FADE_DURATION, EasingEquations::cubicEaseOut);
        }
    }
    else if (animateBounce)
    {
        float test = 0.25f * stepCounter / steps + 1;
        float newScale = floatBackEaseInIut(stepCounter, 1, 0.25f, steps) - test;
        icons[selectedIcon].setScale(1 + newScale);
        icons[selectedIcon].invalidate();
        stepCounter++;
        if (stepCounter > steps)
        {
            icons[selectedIcon].setScale(1);
            animateBounce = false;
            
            icons[selectedIcon].setMoveAnimationDelay(0);
            icons[selectedIcon].startMoveAnimation(icons[selectedIcon].getX(), 100, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

            iconsShadows[selectedIcon].setMoveAnimationDelay(0);
            iconsShadows[selectedIcon].startMoveAnimation(iconsShadows[selectedIcon].getX(), 270, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

            descriptionContianer.setY(DESCRIPTION_Y_2);
            descriptionContianer.setMoveAnimationDelay(0);
            descriptionContianer.startMoveAnimation(0, DESCRIPTION_Y_3, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

            switch (centerIcon)
            {
            case OSCILLOSCOPE:
                description1.setTypedText(TypedText(T_DESCRIPTIONOSCILLOSCOPE1));
                description2.setTypedText(TypedText(T_DESCRIPTIONOSCILLOSCOPE2));
                break;
            case EEMBC:
                description1.setTypedText(TypedText(T_DESCRIPTIONEEMBC1));
                description2.setTypedText(TypedText(T_DESCRIPTIONEEMBC2));
                break;
            case TouchGFX:
                description1.setTypedText(TypedText(T_DESCRIPTIONTOUCHGFX1));
                description2.setTypedText(TypedText(T_DESCRIPTIONTOUCHGFX2));
                break;
            case NUMBER_OF_DEMOS:
            default:
                break;
            }

            header.setAlpha(0);
            description1.setAlpha(0);
            description2.setAlpha(0);
            exitButton.setAlpha(0);     

            exitButton.setTouchable(true);

            header.startFadeAnimation(255, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);
            description1.startFadeAnimation(255, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);
            description2.startFadeAnimation(255, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);
            exitButton.startFadeAnimation(255, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);

            scaleCounter = ICONS_MOVE_DURATION - 10;
            startValue = 0;
            moveDistance = 1;
            steps = 10;
            stepCounter = 0;
            scaleStart = true;
            startTextureMapper.setVisible(true);
        }
    }
    else if (autoAnimate)
    {
      tickCounter++;

      const ClickEvent press_left_icon_evt( ClickEvent::PRESSED , 100, 100 );
      const ClickEvent release_left_icon_evt( ClickEvent::RELEASED, 100, 100 );
      const ClickEvent press_center_icon_evt ( ClickEvent::PRESSED , 240, 136 );
      const ClickEvent release_center_icon_evt ( ClickEvent::RELEASED, 240, 136 );

      if(demoInfo && (tickCounter <= ICONS_MOVE_DURATION))
      {
        if(tickCounter == 1)
        {
          handleClickEvent(press_center_icon_evt);
        }
        else if(tickCounter == 2)
        {
          handleClickEvent(release_center_icon_evt);
        }
        else
        {
          /* Wait until demoInfo is switched to false */
          tickCounter--;
        }
      }
      else if(tickCounter == (1 * ICONS_MOVE_DURATION))
      {
        handleClickEvent(press_left_icon_evt);
      }
      else if(tickCounter == ((1 * ICONS_MOVE_DURATION) + 1))
      {
        handleClickEvent(release_left_icon_evt);
      }
      else if(tickCounter == (2 * ICONS_MOVE_DURATION))
      {
        handleClickEvent(press_center_icon_evt);
      }
      else if(tickCounter == ((2 * ICONS_MOVE_DURATION) + 1))
      {
        handleClickEvent(release_center_icon_evt);
      }
      else if(tickCounter == (5 * ICONS_MOVE_DURATION))
      {
        handleClickEvent(press_center_icon_evt);
      }
      else if(tickCounter == ((5 * ICONS_MOVE_DURATION) + 1))
      {
        handleClickEvent(release_center_icon_evt);
      }
      else if(tickCounter >= (8 * ICONS_MOVE_DURATION))
      {
        tickCounter = 0;
      }
    }
    if (scaleStart)
    {
        if (scaleCounter > 0)
        {
            scaleCounter--;
        }
        else
        {
            startTextureMapper.invalidate();
            float test = floatCubicEaseOut(stepCounter, startValue, moveDistance, steps);
            startTextureMapper.setScale(test);            
            stepCounter++;
            if (stepCounter > steps)
            {
                scaleStart = false;
                if (test == 0)
                {
                    startTextureMapper.setVisible(false);
                }
            }
            startTextureMapper.invalidate();
        }
    }
}

void MainView::enterDemo()
{
    if (animate == false)
    {
        demoInfo = true;

        exitButton.setTouchable(true);
        enterButton.setMoveAnimationDelay(0);
        enterButton.startMoveAnimation(enterButton.getX(), HAL::DISPLAY_HEIGHT, ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);
        for (int i = 0; i < 4; i++)
        {
            if (iconsAngle[i] > (2 * PI - 0.25f) || iconsAngle[i] < 0.25f)
            {
                icons[i].setMoveAnimationDelay(0);
                icons[i].startMoveAnimation(HAL::DISPLAY_WIDTH, icons[i].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);

                iconsShadows[i].setMoveAnimationDelay(0);
                iconsShadows[i].startMoveAnimation(HAL::DISPLAY_WIDTH, iconsShadows[i].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);
            }
            else if (iconsAngle[i] > (PI_HALF - 0.25f) && iconsAngle[i] < (PI_HALF + 0.25f))
            {
                stepCounter = 0;
                steps = floorf(ICONS_MOVE_DURATION / 2.f);
                animateBounce = true;
                selectedIcon = i;
                header.startFadeAnimation(0, HEADER_FADE_DURATION, EasingEquations::cubicEaseOut);                
            }
            else if (iconsAngle[i] > (PI - 0.25f) && iconsAngle[i] < (PI + 0.25f))
            {
                icons[i].setMoveAnimationDelay(0);
                icons[i].startMoveAnimation(-icons[i].getWidth(), icons[i].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);

                iconsShadows[i].setMoveAnimationDelay(0);
                iconsShadows[i].startMoveAnimation(-iconsShadows[i].getWidth(), iconsShadows[i].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);
            }            
        }
    }
}

void MainView::exitDemo()
{
    icons[selectedIcon].setMoveAnimationDelay(ICONS_MOVE_DURATION / 2 - 20);
    icons[selectedIcon].startMoveAnimation(icons[selectedIcon].getX(), radius_y, ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

    iconsShadows[selectedIcon].setMoveAnimationDelay(ICONS_MOVE_DURATION / 2 - 20);
    iconsShadows[selectedIcon].startMoveAnimation(iconsShadows[selectedIcon].getX(), radius_y + 170, ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

    enterButton.setMoveAnimationDelay(ICONS_MOVE_DURATION / 2);
    enterButton.startMoveAnimation(enterButton.getX(), 242, ICONS_MOVE_DURATION - 20, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

    descriptionContianer.setMoveAnimationDelay(0);
    descriptionContianer.startMoveAnimation(0, DESCRIPTION_Y_1, ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut, EasingEquations::cubicEaseOut);

    exitButton.setTouchable(false);

    header.startFadeAnimation(0, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);
    description1.startFadeAnimation(0, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);
    description2.startFadeAnimation(0, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);
    exitButton.startFadeAnimation(0, ICONS_MOVE_DURATION - 10, EasingEquations::cubicEaseOut);

    int sidesIcon = (3 - (-selectedIcon)) % 4;
    iconsShadows[sidesIcon].setMoveAnimationDelay(ICONS_MOVE_DURATION / 2);
    iconsShadows[sidesIcon].startMoveAnimation((int)(originX + 20 + cosf(iconsAngle[sidesIcon])*radius_x), iconsShadows[sidesIcon].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);

    icons[sidesIcon].setMoveAnimationDelay(ICONS_MOVE_DURATION / 2);
    icons[sidesIcon].startMoveAnimation((int)(originX + cosf(iconsAngle[sidesIcon])*radius_x), icons[sidesIcon].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);

    sidesIcon = (selectedIcon + 1) % 4;
    iconsShadows[sidesIcon].setMoveAnimationDelay(ICONS_MOVE_DURATION / 2);
    iconsShadows[sidesIcon].startMoveAnimation((int)(originX + 20 + cosf(iconsAngle[sidesIcon])*radius_x), iconsShadows[sidesIcon].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);

    icons[sidesIcon].setMoveAnimationDelay(ICONS_MOVE_DURATION / 2);
    icons[sidesIcon].startMoveAnimation((int)(originX + cosf(iconsAngle[sidesIcon])*radius_x), icons[sidesIcon].getY(), ICONS_MOVE_DURATION, EasingEquations::cubicEaseOut);

    scaleCounter = 0;
    startValue = 1;
    moveDistance = -1;
    steps = 10;
    stepCounter = 0;
    scaleStart = true;
}

void MainView::animateToClosesItem()
{
    closetsIcon = 0;
    float distance = 10.f; //Number large enough to never be closets

    for (int i = 0; i < 4; i++)
    {
        while (iconsAngle[i] > 2 * PI || iconsAngle[i] < 0)
        {
            if (iconsAngle[i] > 2 * PI)
            {
                iconsAngle[i] = iconsAngle[i] - 2 * PI;
            }
            else
            {
                iconsAngle[i] = iconsAngle[i] + 2 * PI;
            }
        }
        if (abs(abs(iconsAngle[i]) - PI_HALF) < abs(abs(distance) - PI_HALF))
        {
            closetsIcon = i;
            distance = iconsAngle[i];
        }
    }
    angle = iconsAngle[0];    
    stepCounter = 0;
    startValue = angle;
    moveDistance = PI_HALF - iconsAngle[closetsIcon];
    steps = MAX(abs(floorf(moveDistance * 40)), 2);
    animate = true;
}

void MainView::moveIcons(float newAngle)
{
    double newX, newY;

    moveRight = angle > newAngle ? true : false;

    float fmodNewAngle = fmodf(newAngle, PI_HALF);
    float fmodAngle = fmodf(angle, PI_HALF);
    
    if (newAngle > angle && fmodNewAngle < fmodAngle)
    {
        fmodNewAngle = fmodNewAngle + PI_HALF;
    }
    else if (newAngle < angle && fmodNewAngle > fmodAngle)
    {
        fmodNewAngle = fmodNewAngle - PI_HALF;
    }

    if ((fmodAngle < PI_QUART && fmodNewAngle >= PI_QUART) || (fmodAngle < (-PI_QUART) && fmodNewAngle >= (-PI_QUART) && moveRight == false))
    {   
        centerIcon = (DEMOS)((NUMBER_OF_DEMOS - 1 - (-centerIcon)) % NUMBER_OF_DEMOS);
    }
    else if ((fmodAngle > PI_QUART && fmodNewAngle <= PI_QUART ) || (fmodAngle > (-PI_QUART) && fmodNewAngle <= (-PI_QUART) && moveRight == true ))
    {
        centerIcon = (DEMOS)((centerIcon + 1) % NUMBER_OF_DEMOS);
    }

    angle = newAngle;

    for (int i = 0; i < 4; i++)
    {
        icons[i].invalidate();

        iconsAngle[i] = angle + PI_HALF * i;

        newX = originX + cosf(iconsAngle[i])*radius_x;
        newY = originY + sinf(iconsAngle[i])*radius_y;

        icons[i].setXY((int)newX, (int)newY);
        icons[i].setScale((icons[i].getY() + offset) * scalingFactor);

        float circAngle = fmodf(iconsAngle[i], 2 * PI);

        if ((circAngle >= -PI_QUART && circAngle < PI_QUART) ||( circAngle >= 2 * PI - PI_QUART || circAngle < - 2 * PI + PI_QUART))
        {            
            icons[i].setVisible(true);
            iconsShadows[i].setVisible(true);
            icons[i].setBitmap((NUMBER_OF_DEMOS - 1 - (-centerIcon)) % NUMBER_OF_DEMOS);
        }
        else if ((circAngle >= PI_QUART && circAngle <= PI_HALF + PI_QUART) || (circAngle <= -PI - PI_QUART && circAngle >= -2 * PI + PI_QUART))
        {
            icons[i].setVisible(true);
            iconsShadows[i].setVisible(true);
            icons[i].setBitmap(centerIcon);
        }
        else if ((circAngle > PI_HALF + PI_QUART && circAngle <= PI + PI_QUART) || (circAngle <=  - PI + PI_QUART && circAngle > - PI - PI_QUART))
            {
            icons[i].setVisible(true);
            iconsShadows[i].setVisible(true);
                    icons[i].setBitmap((centerIcon + 1) % NUMBER_OF_DEMOS);                   
                }
                else
                {
            icons[i].setVisible(false);
            iconsShadows[i].setVisible(false);
        }

        int newAlpha;
        if (icons[i].getY() > 20 && icons[i].getY() < 39)
        {
            newAlpha = 255 - (icons[i].getY() - 20) * 20;
            if (newAlpha > 255)
            {
                newAlpha = 255;
            }
            else if (newAlpha < 0)
            {
                newAlpha = 0;
            }
        }
        else if (icons[i].getY() >= 39)
        {
            newAlpha = 0;
        }
        else
        {
            newAlpha = 255;
        }
        
        icons[i].setSideAlpha(newAlpha);
        icons[i].invalidate();
    }

    reorderIcons();

    for (int i = 0; i < 4; i++)
    {
        newX = originX + 20 + cosf(iconsAngle[i])*radius_x;
        newY = originY + 170 + sinf(iconsAngle[i])*radius_y;

        iconsShadows[i].invalidate();
        iconsShadows[i].setXY((int)newX, (int)newY);
        
        int subfact = 4 * (210 - iconsShadows[i].getY());
        if (subfact >= 255)
        {
            subfact = 255;
        }
        
        iconsShadows[i].setAlpha(255 - subfact);
        iconsShadows[i].invalidate();
    }
}

float MainView::floatCubicEaseOut(float t, float b, float c, float d)
{
    t /= d;
    t--;
    return c * (t*t*t + 1) + b;
}

float MainView::floatBackEaseInIut(float t, float b, float c, float d)
{
    float s = 1.70158f;
    
    if ((t /= d / 2) < 1)
    {
        s = s * (1.525f);
        return c / 2 * (t*t*((s + 1)*t - s)) + b;
    }

    float postFix = t -= 2;
    
    s *= (1.525f);    
    return c / 2 * ((postFix)*t*((s + 1)*t + s) + 2) + b;
}

void MainView::reorderIcons()
{
    float tmp = fmodf(angle, 2 * PI);

    for (int i = 0; i < 4; i++)
    {
        remove(icons[i]);
    }

    remove(gradient);
    remove(enterButton);

    if (moveRight)
    {
        if (tmp >= 0)
        {
            if (tmp <= PI_HALF)
            {
                backIcon = 2;
            }
            else if (tmp > PI_HALF && tmp <= PI)
            {
                backIcon = 1;
            }
            else if (tmp > PI && tmp <= PI + PI_HALF)
            {
                backIcon = 0;
            }
            else
            {
                backIcon = 3;
            }
        }    
        else
        {
            if (tmp >= -PI_HALF)
            {
                backIcon = 3;
            }
            else if (tmp < -PI_HALF && tmp >= -PI)
            {
                backIcon = 0;
            }
            else if (tmp < -PI && tmp >= -PI - PI_HALF)
            {
                backIcon = 1;
            }
            else
            {
                backIcon = 2;
            }
        }

        switch (backIcon)
        {
        case 0:
            add(icons[0]);
            add(icons[1]);
            add(icons[2]);
            add(icons[3]);
            break;
        case 1:
            add(icons[1]);
            add(icons[2]);
            add(icons[3]);
            add(icons[0]);
            break;
        case 2:
            add(icons[2]);
            add(icons[3]);
            add(icons[0]);
            add(icons[1]);
            break;
        case 3:
            add(icons[3]);
            add(icons[0]);
            add(icons[1]);
            add(icons[2]);
            break;
        }
    }
    else
    {
        if (tmp >= 0)
        {

            if (tmp <= PI_HALF)
            {
                backIcon = 3;
            }
            else if (tmp > PI_HALF && tmp <= PI)
            {
                backIcon = 2;
            }
            else if (tmp > PI && tmp <= PI + PI_HALF)
            {
                backIcon = 1;
            }
            else
            {
                backIcon = 0;
            }
        }
        else
        {
            if (tmp >= -PI_HALF)
            {
                backIcon = 0;
            }
            else if (tmp < -PI_HALF && tmp >= -PI)
            {
                backIcon = 1;
            }
            else if (tmp < -PI && tmp >= -PI - PI_HALF)
            {
                backIcon = 2;
            }
            else
            {
                backIcon = 3;
            }
        }

        switch (backIcon)
        {
        case 0:
            add(icons[0]);
            add(icons[3]);
            add(icons[2]);
            add(icons[1]);
            break;
        case 1:
            add(icons[1]);
            add(icons[0]);
            add(icons[3]);
            add(icons[2]);
            break;
        case 2:
            add(icons[2]);
            add(icons[1]);
            add(icons[0]);
            add(icons[3]);
            break;
        case 3:
            add(icons[3]);
            add(icons[2]);
            add(icons[1]);
            add(icons[0]);
            break;
        }
    }   

    add(gradient);
    add(enterButton);
}

void MainView::moveAnimationEndedHandler(const MoveAnimator<iconContainer>& src)
{
    if (src.getY() == 40)
    {
        header.setFadeAnimationDelay(0);
        header.startFadeAnimation(255, HEADER_FADE_DURATION, EasingEquations::cubicEaseOut);
    }
    else if (src.getX() == -25)
    {
        demoInfo = false;
    }
}
