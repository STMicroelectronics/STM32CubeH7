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

#include <touchgfx/widgets/canvas/AbstractPainter.hpp>

namespace touchgfx
{
AbstractPainter::AbstractPainter() :
    areaOffsetX(0),
    areaOffsetY(0),
    widgetAlpha(255)
{
}

AbstractPainter::~AbstractPainter()
{
}

void AbstractPainter::setOffset(uint16_t offsetX, uint16_t offsetY)
{
    areaOffsetX = offsetX;
    areaOffsetY = offsetY;
}

void AbstractPainter::setWidgetAlpha(const uint8_t alpha)
{
    widgetAlpha = alpha;
}
}
