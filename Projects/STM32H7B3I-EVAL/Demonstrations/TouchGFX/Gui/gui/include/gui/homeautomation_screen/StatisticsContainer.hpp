#ifndef STATISTICS_CONTAINER_HPP
#define STATISTICS_CONTAINER_HPP

#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/containers/Container.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <gui/homeautomation_screen/Graph.hpp>
#include <touchgfx/widgets/canvas/Circle.hpp>
#include <touchgfx/widgets/TiledImage.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>

#if !defined(USE_BPP) || USE_BPP==16
#include <touchgfx/widgets/canvas/PainterRGB565.hpp>
#include <touchgfx/widgets/canvas/PainterRGB565Bitmap.hpp>
#include <gui/homeautomation_screen/PainterRGB565TiledBitmap.hpp>
#elif USE_BPP==24
#include <touchgfx/widgets/canvas/PainterRGB888.hpp>
#include <touchgfx/widgets/canvas/PainterRGB888Bitmap.hpp>
#include <gui/homeautomation_screen/PainterRGB888TiledBitmap.hpp>
#else
#error Unknown USE_BPP
#endif

using namespace touchgfx;

class StatisticsContainer : public Container
{
public:
    StatisticsContainer();
    virtual ~StatisticsContainer();

    virtual void setup();

    virtual void handleTickEvent();
    virtual void handleClickEvent(const ClickEvent& evt);
    virtual void handleDragEvent(const DragEvent& evt);

    void update(int x);
private:
    enum
    {
        STAT_STATE_NONE,
        STAT_STATE_INIT,
        STAT_STATE_SHOW_OFF,
        STAT_STATE_READY
    };
    Image upper;
    TiledImage lower;
    Graph graph[3];
    Box verticalBar;
    Circle circle[3];

#if !defined(USE_BPP) || USE_BPP==16
    PainterRGB565 bgPainter;
    PainterRGB565Bitmap circlePainter[3];
    PainterRGB565TiledBitmap painter[3];
#elif USE_BPP==24
    PainterRGB888 bgPainter;
    PainterRGB888Bitmap circlePainter[3];
    PainterRGB888TiledBitmap painter[3];
#else
#error Unknown USE_BPP
#endif

    Image icon[3];
    TextAreaWithOneWildcard pctValue[3];
    Unicode::UnicodeChar buffer[3][4];
    TextArea pctSign[3];
    static int yScale;
    int numValues;
    int statState;
    float randFreq[3][5];
    float randAdd[3][5];
    int tick;
};

#endif /* STATISTICS_CONTAINER_HPP */
