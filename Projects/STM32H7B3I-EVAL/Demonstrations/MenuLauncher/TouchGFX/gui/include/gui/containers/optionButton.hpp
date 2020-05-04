#ifndef OPTIONBUTTON_HPP
#define OPTIONBUTTON_HPP

#include <gui_generated/containers/optionButtonBase.hpp>

class optionButton : public optionButtonBase
{
public:
    optionButton();
    virtual ~optionButton() {}

    virtual void initialize();

    void setColor(int colorVal);
    void setModule(int index);

    void setSize(bool large);
    void setCurrrentItem(int currentItem);
    int getCurrrentItem();

protected:

    int currentItem;
};

#endif // OPTIONBUTTON_HPP
