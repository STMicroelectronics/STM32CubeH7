#include <touchgfx/Color.hpp>
#include <gui/video_player_screen/VideoProgressBar.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

VideoProgressBar::VideoProgressBar() :
    numberOfFrames(1)
{
    setWidth(800);
    setHeight(30);
    setTouchable(true);

    left.setBitmap(Bitmap(BITMAP_TIME_LINE_ACTIVE_ID));
    right.setBitmap(Bitmap(BITMAP_TIME_LINE_ID));

    left.setXY(0, 0);
    left.setHeight(30);

    right.setPosition(0, 0, 640, 30);

    progressText.setTypedText(TypedText(T_MOVIE_PROGRESS_TIME));
    progressText.setWildcard(progressWildcardBuf);
    progressText.setColor(Color::getColorFrom24BitRGB(0xFF, 0xFF, 0xFF));
    progressWildcardBuf[0] = 0;

    progressText.setPosition(0, 0, 80, 30);

    add(right);
    add(left);
    add(progressText);
}

void VideoProgressBar::setCallback(GenericCallback< const uint32_t >& callback)
{
    dragCallback = &callback;
}

void VideoProgressBar::setMovieData(uint32_t numberOfFrames, uint32_t msPerFrame)
{
    this->numberOfFrames = numberOfFrames;
    this->msPerFrame = msPerFrame;
}

void VideoProgressBar::setProgress(uint32_t frames)
{
    uint32_t width = frames * 640 / numberOfFrames;
    left.setWidth(width);
    left.invalidate();

    int seconds = (frames * msPerFrame + 500) / 1000;
    int minutes = seconds / 60;
    seconds %= 60;
    int hours = minutes / 60;
    minutes %= 60;
    uint16_t textWidth = 0;
    if (hours == 0)
    {
        Unicode::snprintf(progressWildcardBuf, 10, "%d:%02d", minutes, seconds);
        textWidth = progressText.getTextWidth();
    }
    else
    {
        Unicode::snprintf(progressWildcardBuf, 10, "%d:%02d:%02d", hours, minutes, seconds);
        textWidth = progressText.getTextWidth();
    }

    if (width > textWidth + 20u)
    {
        progressText.moveTo(width - textWidth - 10, 0);
    }
    else
    {
        progressText.moveTo(10, 0);
    }
}

void VideoProgressBar::handleClickEvent(const ClickEvent& evt)
{
    if (evt.getType() == ClickEvent::RELEASED)
    {
        left.invalidate();
        setProgress(evt.getX()*numberOfFrames / 640);
        dragCallback->execute(left.getWidth() * 100 / getWidth());
    }
}

void VideoProgressBar::handleDragEvent(const DragEvent& evt)
{
    left.invalidate();
    setProgress(evt.getNewX()*numberOfFrames / 640);

    dragCallback->execute(left.getWidth() * 100 / getWidth());
}
