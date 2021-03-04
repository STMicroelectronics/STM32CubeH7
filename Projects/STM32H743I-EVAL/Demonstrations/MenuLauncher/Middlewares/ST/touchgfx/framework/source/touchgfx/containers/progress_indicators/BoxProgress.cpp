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

#include <touchgfx/containers/progress_indicators/BoxProgress.hpp>

namespace touchgfx
{
BoxProgress::BoxProgress()
    : AbstractDirectionProgress(), box()
{
    progressIndicatorContainer.add(box);
}

BoxProgress::~BoxProgress()
{
}

void BoxProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    box.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void BoxProgress::setColor(colortype color)
{
    box.setColor(color);
}

touchgfx::colortype BoxProgress::getColor() const
{
    return box.getColor();
}

void BoxProgress::setAlpha(uint8_t alpha)
{
    box.setAlpha(alpha);
}

uint8_t BoxProgress::getAlpha() const
{
    return box.getAlpha();
}

void BoxProgress::setValue(int value)
{
    AbstractProgressIndicator::setValue(value);
    box.invalidate();
    int16_t progress = 0;
    switch (progressDirection)
    {
    case AbstractDirectionProgress::RIGHT:
    case AbstractDirectionProgress::LEFT:
        progress = AbstractProgressIndicator::getProgress(progressIndicatorContainer.getWidth());
        break;
    case AbstractDirectionProgress::DOWN:
    case AbstractDirectionProgress::UP:
        progress = AbstractProgressIndicator::getProgress(progressIndicatorContainer.getHeight());
        break;
    }
    switch (progressDirection)
    {
    case AbstractDirectionProgress::RIGHT:
        box.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
        break;
    case AbstractDirectionProgress::LEFT:
        box.setPosition(getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
        break;
    case AbstractDirectionProgress::DOWN:
        box.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
        break;
    case AbstractDirectionProgress::UP:
        box.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
        break;
    }
    box.invalidate();
}
}
