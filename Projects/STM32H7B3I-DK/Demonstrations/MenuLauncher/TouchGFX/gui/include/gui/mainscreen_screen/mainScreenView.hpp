#ifndef MAINSCREENVIEW_HPP
#define MAINSCREENVIEW_HPP

#include <gui_generated/mainscreen_screen/mainScreenViewBase.hpp>
#include <gui/mainscreen_screen/mainScreenPresenter.hpp>
#include <gui/containers/infoBox.hpp>

#include <touchgfx/mixins/FadeAnimator.hpp>
#include <gui/common/enums.hpp>

class mainScreenView : public mainScreenViewBase
{
public:
    mainScreenView();
    virtual ~mainScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    virtual void moduleSelectorWheelUpdateItem(optionButtonSelected& item, int16_t itemIndex);

    virtual void popUpBackPressed();
    virtual void popUpStartPressed();    

    void setFirmwareVersion(const char* fwVersion);

protected:
    void itemSelectedHandler(int16_t item);
    Callback<mainScreenView, int16_t> itemSelectedCallback;

    Callback<mainScreenView> animationEnddedCallback;
    void animationEnddedHandler();
    
    Callback<mainScreenView, int16_t> animateToCallback;
    void animateToHandler(int16_t itemSelected);

    int selectedModule;
    int boxColor[NUMBER_OF_MODULES];
    int16_t totalDrag;
    bool isDragged;
};

#endif // MAINSCREENVIEW_HPP
