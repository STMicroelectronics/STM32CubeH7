#include <platform/hal/simulator/sdl2/HALSDL2.hpp>
#include <touchgfx/hal/NoDMA.hpp>
#include <common/TouchGFXInit.hpp>
#include <gui_generated/common/SimConstants.hpp>
#include <platform/driver/touch/SDL2TouchController.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#include <platform/driver/lcd/LCD16bpp.hpp>
#elif USE_BPP==24
#include <platform/driver/lcd/LCD24bpp.hpp>
#elif USE_BPP==4
#include <platform/driver/lcd/LCD4bpp.hpp>
#elif USE_BPP==2
#include <platform/driver/lcd/LCD2bpp.hpp>
#elif USE_BPP==1
#include <platform/driver/lcd/LCD1bpp.hpp>
#else
#error Unknown USE_BPP
#endif
#include <stdlib.h>

//#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
//#define CANVAS_BUFFER_SIZE (3600)

using namespace touchgfx;

#ifdef __linux__
int main(int argc, char** argv)
{
#else
#include <shellapi.h>
#ifdef _UNICODE
#error Cannot run in unicode mode
#endif
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    int argc;
    char** argv = HALSDL2::getArgv(&argc);
#endif
    NoDMA dma; //For windows/linux, DMA transfers are simulated
#if !defined(USE_BPP) || USE_BPP==16
    LCD16bpp lcd;
#elif USE_BPP==24
    LCD24bpp lcd;
#elif USE_BPP==4
    LCD4bpp lcd;
#elif USE_BPP==2
    LCD2bpp lcd;
#elif USE_BPP==1
    LCD1bpp lcd;
#else
#error Unknown USE_BPP
#endif
    SDL2TouchController tc;

    // Create hardware layer. Use a display size of 480x272.
    HAL& hal = touchgfx_generic_init<HALSDL2>(dma, lcd, tc, SIM_WIDTH, SIM_HEIGHT, 0, 0);

    // Simulate hardware running at 60Hz generating a vsync every 16.6667 ms
    static_cast<HALSDL2&>(hal).setVsyncInterval(16.6667f);
    static_cast<HALSDL2&>(hal).setWindowTitle(SIM_TITLE);

    // Initialize SDL
    bool sdl_init_result = static_cast<HALSDL2&>(hal).sdl_init(argc, argv);
    assert(sdl_init_result && "Error during SDL initialization");

    // Ensure there is a console window to print to using printf() or
    // std::cout, and read from using e.g. fgets or std::cin.
    // Alternatively, instead of using printf(), always use
    // touchgfx_printf() which will ensure there is a console to write
    // to.
    //touchgfx_enable_stdio();

    // Setup the CanvasWidgetRenderer. ONLY needed if you use CanvasWidgets
    // in your application. The CANVAS_BUFFER_SIZE can be adjusted to match
    // your needs in performance vs. RAM usage. Read more on this in the
    // TouchGFX Manual.
    //static uint8_t canvasBuffer[CANVAS_BUFFER_SIZE];
    //CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);

    touchgfx::HAL::getInstance()->taskEntry(); //Never returns

    return EXIT_SUCCESS;
}
