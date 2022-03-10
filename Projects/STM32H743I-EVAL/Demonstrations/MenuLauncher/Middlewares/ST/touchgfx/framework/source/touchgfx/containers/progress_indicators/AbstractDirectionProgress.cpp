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

#include <touchgfx/containers/progress_indicators/AbstractDirectionProgress.hpp>

namespace touchgfx
{
AbstractDirectionProgress::AbstractDirectionProgress()
    : AbstractProgressIndicator()
{
    AbstractDirectionProgress::setDirection(RIGHT);
}

AbstractDirectionProgress::~AbstractDirectionProgress()
{
}

void AbstractDirectionProgress::setDirection(DirectionType direction)
{
    progressDirection = direction;
    setValue(getValue());
}

touchgfx::AbstractDirectionProgress::DirectionType AbstractDirectionProgress::getDirection() const
{
    return progressDirection;
}
}
