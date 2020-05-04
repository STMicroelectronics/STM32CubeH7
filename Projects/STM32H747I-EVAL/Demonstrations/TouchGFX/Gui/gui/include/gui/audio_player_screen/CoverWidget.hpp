

#ifndef COVERWIDGET_HPP
#define COVERWIDGET_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>

#include <gui/common/CommonDefinitions.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

using namespace touchgfx;

class CoverWidget : public Container
{
public:
    CoverWidget();
    virtual ~CoverWidget();
    void setBitmap(Bitmap& bmp);
    BitmapId getBitmap() const
    {
        return imgId;
    }
    void setProgress(int32_t value);
    void setPlayState(PlayerState state);
    void setTouched(bool touched);
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);
    void setAction(GenericCallback<uint32_t>& act) { progressAction = &act; }
protected:
    GenericCallback<uint32_t>* progressAction; 
    void setSelectedPosition(int newX, int newY, bool filter);
    Circle progressArc;
    PainterRGB888 arcPainter;
    FadeAnimator<Image> img;
    FadeAnimator<Image> shadow;
    Image progressBackground;
    Image progressKnob;
    PlayerState playerState;
    touchgfx::BitmapId imgId;
    bool isBeingDragged;
};

#endif /* COVERWIDGET_HPP */
