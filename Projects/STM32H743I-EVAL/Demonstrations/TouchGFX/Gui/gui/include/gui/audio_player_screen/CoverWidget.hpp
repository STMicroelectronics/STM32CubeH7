#ifndef COVERWIDGET_HPP
#define COVERWIDGET_HPP

#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Image.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#else
#error Unknown USE_BPP
#endif
#include <touchgfx/widgets/canvas/Circle.hpp>

#include <gui/common/CommonDefinitions.hpp>
#include <touchgfx/mixins/FadeAnimator.hpp>

#include <gui/audio_player_screen/VisualizationWidget.hpp>

using namespace touchgfx;

class CoverWidget : public Container
{
public:
    enum COVER_VIEW
    {
        NO_MEDIA_VIEW,
        PROGRESS_VIEW,
        VISUALIZATION_VIEW
    };

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
    void handleClickEvent(const ClickEvent& evt);
    void handleDragEvent(const DragEvent& evt);
    void setAction(GenericCallback<uint32_t>& act) { progressAction = &act; }

    void renderVisualization();
    void nextVisualization();

    void setView(enum COVER_VIEW view);

protected:
    GenericCallback<uint32_t>* progressAction; 
    void setSelectedPosition(int newX, int newY, bool filter);
    Circle progressArc;
#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 arcPainter;
#elif USE_BPP==24
    PainterRGB888 arcPainter;
#else
#error Unknown USE_BPP
#endif
    FadeAnimator<Image> img;
    FadeAnimator<Image> shadow;
    Image progressBackground;
    Image progressKnob;
    PlayerState playerState;
    touchgfx::BitmapId imgId;
    bool isBeingDragged;
    VisualizationWidget visualization;
};

#endif /* COVERWIDGET_HPP */
