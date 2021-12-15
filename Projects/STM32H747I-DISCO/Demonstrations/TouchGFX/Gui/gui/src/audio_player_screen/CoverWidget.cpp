

#include <touchgfx/Color.hpp>
#include <gui/audio_player_screen/CoverWidget.hpp>
#include <math.h>
#include <stdio.h>
#include "BitmapDatabase.hpp"
CoverWidget::CoverWidget() :
    progressAction(0),
    playerState(STOPPED),
    isBeingDragged(false)
{
    Bitmap shadowBmp(BITMAP_ALBUM_CIRCLE_SHADOW_ID);
    setWidth(shadowBmp.getWidth());
    setHeight(shadowBmp.getHeight());
    shadow.setBitmap(shadowBmp);

    Bitmap alphaBmp(BITMAP_ALBUM_CIRCLE_256_ID);
    Bitmap dynBmp = Bitmap(Bitmap::dynamicBitmapCreate(alphaBmp.getWidth(), alphaBmp.getHeight(), Bitmap::ARGB8888));
    const uint8_t* srcAlpha = alphaBmp.getData();
    uint8_t* dstAlpha = const_cast<uint8_t*>(dynBmp.getData());
    assert(dstAlpha && "could not allocate memory for dynBmp");
    for (int i = 0; i < alphaBmp.getHeight(); i++)
    {
        for (int j = 0; j < alphaBmp.getWidth(); j++)
        {
            srcAlpha += 3;
            dstAlpha += 3;
            *dstAlpha = *srcAlpha;
            dstAlpha++;
            srcAlpha++;
        }
    }
    img.setBitmap(dynBmp);
    img.setX((shadow.getWidth() - img.getWidth()) / 2);
    img.setY((shadow.getHeight() - img.getHeight()) / 2);

    progressBackground.setBitmap(Bitmap(BITMAP_PROCESS_CIRCLE_ID));
    progressBackground.setX((getWidth() - progressBackground.getWidth()) / 2);
    progressBackground.setY((getHeight() - progressBackground.getHeight()) / 2);

    progressArc.setPosition(progressBackground.getX(), progressBackground.getY(), progressBackground.getWidth(), progressBackground.getHeight());
    progressArc.setCenter(progressArc.getWidth() / 2, progressArc.getHeight() / 2);
    progressArc.setRadius(147);
    progressArc.setLineWidth(3);
    progressArc.setPainter(arcPainter);
    progressArc.setArc(0, 0);

    progressKnob.setBitmap(Bitmap(BITMAP_PROCESS_CIRCLE_KNOP_ID));

    progressArc.setVisible(false);
    progressKnob.setVisible(false);
    progressBackground.setVisible(false);

    //setTouchable true to enable audio seek.
    setTouchable(true);
    
    add(progressBackground);
    add(img);
    add(shadow);
    add(progressArc);
    add(progressKnob);
}

CoverWidget::~CoverWidget()
{
    Bitmap::dynamicBitmapDelete(img.getBitmap());
}

void CoverWidget::setPlayState(PlayerState state)
{
    if ((playerState == STOPPED || playerState == PAUSED) && state == PLAYING)
    {
        progressArc.setVisible(true);
        progressBackground.setVisible(true);
        arcPainter.setColor(Color::getColorFrom24BitRGB(0, 0xC0, 0xFE));
        progressBackground.invalidate();
        //        progressKnob.setVisible(true);
        setProgress(0);
    }
    else if (state == STOPPED)
    {
        setProgress(0);
        progressArc.setVisible(false);
        progressKnob.setVisible(false);
    }
    else if (state == PAUSED)
    {
        arcPainter.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        progressArc.invalidate();
    }

    playerState = state;
}

void CoverWidget::setTouched(bool touched)
{
    img.setAlpha(touched ? 128 : 255);
    img.invalidate();
}

void CoverWidget::setProgress(int32_t value)
{
    if (!isBeingDragged)
    {
        progressArc.updateArcEnd(value % 360);
    }
}

void CoverWidget::setBitmap(Bitmap& bmp)
{
   if (bmp.getFormat() != Bitmap::ARGB8888
       && bmp.getFormat() != Bitmap::RGB888)
       return;

    imgId = bmp.getId();
    img.setAlpha(128);
    img.startFadeAnimation(255, 10, EasingEquations::cubicEaseIn);
    shadow.setAlpha(128);
    shadow.startFadeAnimation(255, 10, EasingEquations::cubicEaseIn);
    const uint8_t* src = bmp.getData();
    Bitmap imgBmp = Bitmap(img.getBitmap());
    uint8_t* dst = const_cast<uint8_t*>(Bitmap(img.getBitmap()).getData());
    for (int i = 0; i < bmp.getHeight(); i++)
    {
        for (int j = 0; j < bmp.getWidth(); j++)
        {
            *dst++ = *src++;
            *dst++ = *src++;
            *dst++ = *src++;
            if (bmp.getFormat() == Bitmap::ARGB8888)
                src++;
            if (imgBmp.getFormat() == Bitmap::ARGB8888)
                dst++;
        }
    }
}

void CoverWidget::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::PRESSED)
    {
        isBeingDragged = true;
        setSelectedPosition(evt.getX(), evt.getY(), false);
    }
    else
    {
        isBeingDragged = false;
    }
}

void CoverWidget::handleDragEvent(const DragEvent& evt)
{
    setSelectedPosition(evt.getNewX(), evt.getNewY(), true);
}

void CoverWidget::setSelectedPosition(int newX, int newY, bool filter)
{
    int centerX;
    int centerY;
    progressArc.getCenter(centerX, centerY);
    centerX += progressArc.getX();
    centerY += progressArc.getY();

    int16_t vx = newX - centerX;
    int16_t vy = newY - centerY;

    int length;
    int angleInDegrees = CWRUtil::angle<int>(vx, vy, length);



    if (length > 220)
    {
        return;
    }
    if (filter)
    {
        if ((progressArc.getArcEnd() > 270) && (angleInDegrees < 90))
        {
            angleInDegrees = 360;
        }
        else if ((angleInDegrees > 270) && (progressArc.getArcEnd() < 90))
        {
            angleInDegrees = 0;
        }
    }
    progressArc.updateArcEnd(angleInDegrees);
    if (progressAction && progressAction->isValid())
    {
        progressAction->execute(angleInDegrees);
    }
}
