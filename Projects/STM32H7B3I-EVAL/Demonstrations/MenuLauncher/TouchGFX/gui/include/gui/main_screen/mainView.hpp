#ifndef MAINVIEW_HPP
#define MAINVIEW_HPP

#include <gui_generated/main_screen/mainViewBase.hpp>
#include <gui/main_screen/mainPresenter.hpp>

#include <gui/common/enums.hpp>

class mainView : public mainViewBase
{
public:
    mainView();
    virtual ~mainView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    virtual void moduleSelectorWheelUpdateItem(optionButton& item, int16_t itemIndex);

    virtual void popUpBackPressed();
    virtual void popUpStartPressed();

    void setFirmwareVersion(const char* fwVersion);

protected:
    void itemSelectedHandler(int16_t item);
    Callback<mainView, int16_t> itemSelectedCallback;

    Callback<mainView> animationEnddedCallback;
    void animationEnddedHandler();

    Callback<mainView, int16_t> animateToCallback;
    void animateToHandler(int16_t itemSelected);

    int selectedModule;
    int boxColor[NUMBER_OF_MODULES];
    int16_t totalDrag;
    bool isDragged;
};

#endif // MAINVIEW_HPP
