#include <touchgfx/hal/HAL.hpp>
#include <touchgfx/hal/BoardConfiguration.hpp>
#include <touchgfx/InternalFlashFont.hpp>
#include <touchgfx/Color.hpp>
#include <touchgfx/lcd/LCD.hpp>
#include <touchgfx/Bitmap.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>

#include <gui/audio_player_screen/VisualizationWidget.hpp>

namespace touchgfx
{

VisualizationWidget::VisualizationWidget(uint8_t *samples, int count) : waveform(samples), num_samples(count)
{
    Drawable::setWidth(VIZ_WIDTH);
    Drawable::setHeight(VIZ_HEIGHT - 1);
    
    for (int i = 0; i < num_samples; i++)
        waveform[i] = 64;

    visualization_initialize();
}

Rect VisualizationWidget::getMinimalRect() const
{
    return Rect(0, 0, VIZ_WIDTH, VIZ_HEIGHT - 1);
}

Rect VisualizationWidget::getSolidRect() const
{
    return Rect(0, 0, VIZ_WIDTH, VIZ_HEIGHT - 1);
}

void VisualizationWidget::draw(const Rect &invalidatedArea) const
{
    HAL* hal = HAL::getInstance();

    Rect rect(0, 0, getWidth(), getHeight());
    translateRectToAbsolute(rect);

    uint8_t *framebuffer = (uint8_t*)hal->lockFrameBuffer();
    uint8_t bitDepth = HAL::lcd().bitDepth();
    framebuffer += rect.x * (bitDepth/8) + rect.y * HAL::DISPLAY_WIDTH * (bitDepth/8);

    visualization_copy_framebuffer(framebuffer, HAL::DISPLAY_WIDTH,
                                   0, 0, getWidth(), getHeight());

    hal->unlockFrameBuffer();
}

bool VisualizationWidget::drawCanvasWidget(const Rect& invalidatedArea) const
{
    return true;
}

#ifdef SIMULATOR
static unsigned int rand_pool = 0x12345678;
static unsigned int rand_add = 0x87654321;

static inline unsigned int myrand()
{
    rand_pool ^= ((rand_pool << 7) | (rand_pool >> 25));
    rand_pool += rand_add;
    rand_add += rand_pool;

    return rand_pool;
}
#endif

void VisualizationWidget::render()
{
#ifdef SIMULATOR
    /* simulate audio samples */
    for (int i = 0; i < num_samples; i++)
        waveform[i] = (uint8_t)(myrand() & 0xff) >> 1; /* 0, 128 */
#endif

    visualization_render(waveform, num_samples);

    invalidate();
}

void VisualizationWidget::next()
{
    visualization_next();
}

void VisualizationWidget::handleClickEvent(const ClickEvent &evt)
{
}

}
