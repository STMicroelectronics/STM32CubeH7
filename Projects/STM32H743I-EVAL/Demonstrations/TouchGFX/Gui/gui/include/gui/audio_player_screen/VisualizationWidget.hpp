#ifndef VISUALIZATION_WIDGET__
#define VISUALIZATION_WIDGET__

#include <touchgfx/hal/Types.hpp>
#include <touchgfx/widgets/Widget.hpp>

#include <touchgfx/widgets/canvas/Canvas.hpp>
#include <touchgfx/widgets/canvas/CanvasWidget.hpp>

#include "gui/audio_player_screen/visualization.h"

namespace touchgfx
{

class VisualizationWidget : public CanvasWidget {
public:
    VisualizationWidget(uint8_t *samples, int count);

    Rect getMinimalRect() const;
    Rect getSolidRect() const;

    void draw(const Rect &invalidatedArea) const;
    bool drawCanvasWidget(const Rect& invalidatedArea) const;

    void render();
    void next();

    void handleClickEvent(const ClickEvent &evt);

private:
    uint8_t *waveform;
    int num_samples;
};

}

#endif // VISUALIZATION_WIDGET__
