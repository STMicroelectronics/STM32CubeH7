#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <gui_generated/main_screen/MainViewBase.hpp>
#include <gui/main_screen/MainPresenter.hpp>
#include "gui/containers/shadowContainer.hpp"
#include "gui/containers/iconContainer.hpp"
#include "gui/common/enums.hpp"

class MainView : public MainViewBase
{
public:
    MainView();
    virtual ~MainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    virtual void handleGestureEvent(const GestureEvent& evt);

    virtual void handleTickEvent();

    virtual void enterDemo();
    virtual void exitDemo();

    void animateToClosesItem();
    void moveIcons(float tmpAngle);
    float floatCubicEaseOut(float t, float b, float c, float d);
    float floatBackEaseInIut(float t, float b, float c, float d);

    void reorderIcons();

    virtual void SetautoAnimate ( bool value ) { autoAnimate = value; tickCounter = 0; }
    virtual bool GetautoAnimate () { return autoAnimate; }
protected:
    
    int HEADER_FADE_DURATION    ;
    int ICONS_MOVE_DURATION     ;
    int DESCRIPTION_Y_1         ;
    int DESCRIPTION_Y_2  ;
    int DESCRIPTION_Y_3 ;

    Callback<MainView, const MoveAnimator<iconContainer>&> moveAnimationEndedCallback;
    void moveAnimationEndedHandler(const MoveAnimator<iconContainer>& src);

    MoveAnimator<shadowContainer> iconsShadows[4];
    MoveAnimator<iconContainer> icons[4];

    int radius_x;
    int radius_y;

    float lastAngle;
    float angle;

    float originX;
    float originY;

    float mininalSize;
    float scalingFactor;
    float offset;

    float iconsAngle[4];
    float stepSizes;
    float steps;
    float stepCounter;
    float moveDistance;
    float startValue;
    int closetsIcon;

    int yMax;
    int yMin;

    bool animate;
    bool autoAnimate;
    bool animateBounce;
    bool scaleStart;

    int scaleCounter;
    int tickCounter;

    bool isDragged;

    int backIcon;
    int lastIconChanged;
    bool moveRight;
    bool demoInfo;

    DEMOS centerIcon;
    int selectedIcon;

    bool playPressed;
};

#endif // MAINVIEW_HPP
