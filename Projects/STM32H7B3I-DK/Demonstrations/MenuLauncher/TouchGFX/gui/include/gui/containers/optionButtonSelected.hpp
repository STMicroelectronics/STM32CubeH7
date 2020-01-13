#ifndef OPTIONBUTTONSELECTED_HPP
#define OPTIONBUTTONSELECTED_HPP

#include <gui_generated/containers/optionButtonSelectedBase.hpp>

class optionButtonSelected : public optionButtonSelectedBase
{
public:
    optionButtonSelected();
    virtual ~optionButtonSelected() {}

    virtual void initialize();

    void setColor(int colorVal);
    void setModule(int index);

    void setSize(bool large);
    void setCurrrentItem(int currentItem);
    int getCurrrentItem();

protected:

    int currentItem;
};

#endif // OPTIONBUTTONSELECTED_HPP
