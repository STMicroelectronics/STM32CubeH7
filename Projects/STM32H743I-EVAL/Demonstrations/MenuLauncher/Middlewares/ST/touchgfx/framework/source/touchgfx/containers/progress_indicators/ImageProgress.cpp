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

#include <touchgfx/containers/progress_indicators/ImageProgress.hpp>

namespace touchgfx
{
ImageProgress::ImageProgress()
    : AbstractDirectionProgress(), image(), fixedPosition(true)
{
    container.add(image);
    progressIndicatorContainer.add(container);
}

ImageProgress::~ImageProgress()
{
}

void ImageProgress::setProgressIndicatorPosition(int16_t x, int16_t y, int16_t width, int16_t height)
{
    container.setPosition(0, 0, width, height);

    AbstractProgressIndicator::setProgressIndicatorPosition(x, y, width, height);
}

void ImageProgress::setAnchorAtZero(bool anchorAtZero)
{
    fixedPosition = anchorAtZero;
    setValue(getValue());
}

bool ImageProgress::getAnchorAtZero() const
{
    return fixedPosition;
}

void ImageProgress::setBitmap(touchgfx::BitmapId bitmapId)
{
    image.setBitmap(Bitmap(bitmapId));
}

touchgfx::BitmapId ImageProgress::getBitmap() const
{
    return image.getBitmap();
}

void ImageProgress::setAlpha(uint8_t alpha)
{
    image.setAlpha(alpha);
}

uint8_t ImageProgress::getAlpha() const
{
    return image.getAlpha();
}

void ImageProgress::setValue(int value)
{
    AbstractProgressIndicator::setValue(value);
    container.invalidate();
    uint16_t maxProgress = 0;
    if (progressDirection == AbstractDirectionProgress::RIGHT || progressDirection == AbstractDirectionProgress::LEFT)
    {
        maxProgress = progressIndicatorContainer.getWidth();
    }
    else
    {
        maxProgress = progressIndicatorContainer.getHeight();
    }
    int16_t progress = AbstractProgressIndicator::getProgress(maxProgress);
    if (fixedPosition)
    {
        switch (progressDirection)
        {
        case AbstractDirectionProgress::RIGHT:
            container.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            image.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::LEFT:
            container.setPosition(getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
            image.setPosition(-container.getX(), 0, progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::DOWN:
            container.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            break;
        case AbstractDirectionProgress::UP:
            container.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, -container.getY(), progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
            break;
        }
    }
    else
    {
        switch (progressDirection)
        {
        case AbstractDirectionProgress::RIGHT:
            container.setPosition(0, 0, progress, getHeight());
            image.setPosition(progress - progressIndicatorContainer.getWidth(), 0, progressIndicatorContainer.getWidth(), getHeight());
            break;
        case AbstractDirectionProgress::LEFT:
            container.setPosition(progressIndicatorContainer.getWidth() - progress, 0, progress, progressIndicatorContainer.getHeight());
            image.setPosition(0, 0, progress, progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::DOWN:
            container.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, progress - progressIndicatorContainer.getHeight(), progressIndicatorContainer.getWidth(), progressIndicatorContainer.getHeight());
            break;
        case AbstractDirectionProgress::UP:
            container.setPosition(0, progressIndicatorContainer.getHeight() - progress, progressIndicatorContainer.getWidth(), progress);
            image.setPosition(0, 0, progressIndicatorContainer.getWidth(), progress);
            break;
        }
    }
    container.invalidate();
}
}
