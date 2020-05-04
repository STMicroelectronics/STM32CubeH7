#ifndef MCULOADWIDGET_HPP
#define MCULOADWIDGET_HPP

#include <gui_generated/containers/McuLoadWidgetBase.hpp>

class McuLoadWidget : public McuLoadWidgetBase
{
public:
    McuLoadWidget();
    virtual ~McuLoadWidget() {}

    void updateProcessorLoad(uint8_t mcuLoad);
    void setColor(colortype color);
protected:
};

#endif // MCULOADWIDGET_HPP
