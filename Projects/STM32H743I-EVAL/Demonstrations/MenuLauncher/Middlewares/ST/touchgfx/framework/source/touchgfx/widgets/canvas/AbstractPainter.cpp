/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.12.3 distribution.
  *
  * @attention
  *
  * Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
