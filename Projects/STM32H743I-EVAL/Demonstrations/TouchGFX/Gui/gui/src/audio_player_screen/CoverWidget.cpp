#include <touchgfx/Color.hpp>
#include <gui/audio_player_screen/CoverWidget.hpp>
#include <math.h>
#include "BitmapDatabase.hpp"

#ifdef SIMULATOR
uint8_t visualization_samples[64];
#else
extern "C" uint8_t visualization_samples[64];
#endif

CoverWidget::CoverWidget() :
    progressAction(0),
    playerState(STOPPED),
    isBeingDragged(false),
    visualization(visualization_samples, 64)
{
    Bitmap shadowBmp(BITMAP_ALBUM_CIRCLE_SHADOW_ID);
    setWidth(shadowBmp.getWidth());
    setHeight(shadowBmp.getHeight());
    shadow.setBitmap(shadowBmp);

    Bitmap alphaBmp(BITMAP_ALBUM_CIRCLE_256_ID);
    Bitmap dynBmp = Bitmap(Bitmap::dynamicBitmapCreate(alphaBmp.getWidth(), alphaBmp.getHeight(), Bitmap::ARGB8888));
    const uint8_t* srcAlpha = alphaBmp.getData();
    uint8_t* dstAlpha = const_cast<uint8_t*>(dynBmp.getData());
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

    visualization.setX((shadow.getWidth() - visualization.getWidth()) / 2);
    visualization.setY((shadow.getHeight() - visualization.getHeight()) / 2);
    visualization.setVisible(false);

    setTouchable(true);
    
    add(progressBackground);
    add(img);
    add(shadow);
    add(progressArc);
    add(progressKnob);
    add(visualization);
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
        setProgress(0);
    }
    else if (state == STOPPED)
    {
        setProgress(0);
    }
    else if (state == PAUSED)
    {
        arcPainter.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
        progressArc.invalidate();
    }

    playerState = state;
}

void CoverWidget::renderVisualization()
{
    if (playerState == PLAYING)
        visualization.render();
}

void CoverWidget::nextVisualization()
{
    if (playerState == PLAYING)
        visualization.next();
}

void CoverWidget::setView(enum COVER_VIEW view)
{
    switch (view) {
    case NO_MEDIA_VIEW:
        setTouchable(false);
        visualization.setVisible(false);
        getParent()->invalidate();
        break;
    case PROGRESS_VIEW:
        progressBackground.setVisible(true);
        img.setVisible(true);
        shadow.setVisible(true);
        progressArc.setVisible(true);
        visualization.setVisible(false);
        setTouchable(true);
        getParent()->invalidate();
        break;
    case VISUALIZATION_VIEW:
        progressBackground.setVisible(false);
        img.setVisible(false);
        shadow.setVisible(false);
        progressArc.setVisible(false);
        setTouchable(true);
        visualization.setVisible(true);
        getParent()->invalidate();
        break;
    }
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
    Bitmap imgBmp = Bitmap(img.getBitmap());
    const uint8_t* src = bmp.getData();
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
