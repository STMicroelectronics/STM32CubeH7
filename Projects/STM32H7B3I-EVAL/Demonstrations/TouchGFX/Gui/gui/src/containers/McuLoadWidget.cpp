#include <gui/containers/McuLoadWidget.hpp>

McuLoadWidget::McuLoadWidget()
{
    Unicode::snprintf(mcuLoadValueTxtBuffer, 5, "%d", 0);
}

void McuLoadWidget::updateProcessorLoad(uint8_t mcuLoad)
{
    Unicode::snprintf(mcuLoadValueTxtBuffer, MCULOADVALUETXT_SIZE, "%d", mcuLoad);
    mcuLoadValueTxt.invalidate();
}

void McuLoadWidget::setColor(colortype color)
{
    mcuLoadTxt.setColor(color);
    mcuLoadValueTxt.setColor(color);
    invalidate();
}
