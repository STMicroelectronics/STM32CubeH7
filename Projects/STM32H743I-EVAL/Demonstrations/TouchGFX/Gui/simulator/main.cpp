

#include <platform/hal/simulator/sdl2/HALSDL2.hpp>
#include <gui/common/FrontendHeap.hpp>
#include <touchgfx/hal/NoDMA.hpp>
#include <common/TouchGFXInit.hpp>
#include <platform/driver/touch/SDL2TouchController.hpp>
#if !defined(USE_BPP) || USE_BPP==16
#if !defined(USE_BPP) || USE_BPP==16
#include <platform/driver/lcd/LCD16bpp.hpp>
#elif USE_BPP==24
#include <platform/driver/lcd/LCD24bpp.hpp>
#elif USE_BPP==1
#include <platform/driver/lcd/LCD1bpp.hpp>
#endif
#elif USE_BPP==24
#include <platform/driver/lcd/LCD24bpp.hpp>
#elif USE_BPP==1
#include <platform/driver/lcd/LCD1bpp.hpp>
#endif
#include <stdlib.h>

#include <stdio.h>

#include <gui/common/CommonDefinitions.hpp>

using namespace touchgfx;

const uint32_t movies = PLAYLIST_LENGTH * RGB24BYTES(190, 114);
const uint32_t songs = PLAYLIST_LENGTH * (256 * 256 * 3 + 60 * 60 * 4) + 256 * 256 * 4;
const uint32_t bmpCacheSize = MAX(movies, songs) + 200000;
uint8_t bmpCache[bmpCacheSize + 200000];

const uint32_t dynBitmaps = PLAYLIST_LENGTH * 2 + 1; //playlist + audio player

#ifdef WIN32
extern "C" {
    size_t jpeg_write_file (FILE  *file, uint8_t *buf, uint32_t sizeofbuf);

    size_t jpeg_write_file (FILE  *file, uint8_t *buf, uint32_t sizeofbuf)
    {
        return 0;
    }
}
#endif

int main(int argc, char** argv)
{
    NoDMA dma; //For windows/linux, DMA transfers are simulated
#if !defined(USE_BPP) || USE_BPP==16
#if !defined(USE_BPP) || USE_BPP==16
    LCD16bpp lcd;
#elif USE_BPP==24
    LCD24bpp lcd;
#elif USE_BPP==1
    LCD1bpp lcd;
#endif
#elif USE_BPP==24
    LCD24bpp lcd;
#elif USE_BPP==1
    LCD1bpp lcd;
#endif
    SDL2TouchController tc;

    // Create hardware layer. Use a display size of 800x480.
    HAL& hal = touchgfx_generic_init<HALSDL2>(dma, lcd, tc, 640, 480, (uint16_t*)bmpCache, sizeof(bmpCache), dynBitmaps);

    // Initialize SDL
    if (!dynamic_cast<HALSDL2&>(hal).sdl_init(argc, argv))
    {
        return EXIT_FAILURE;
    }

    int siz = BitmapDatabase::getInstanceSize();
    printf("Number of bitmaps: %d\r\n", siz);
    int acc = 0;
    for (int i = 0; i < siz; i++)
    {
        Bitmap bmp(i);
        if (bmp.getFormat() == Bitmap::RGB888)
        {
            acc += bmp.getWidth() * bmp.getHeight() * 3;
        }
        else if (bmp.getFormat() == Bitmap::RGB565)
        {
            acc += bmp.getWidth() * bmp.getHeight() * 2;
        }
        else if (bmp.getFormat() == Bitmap::ARGB8888)
        {
            acc += bmp.getWidth() * bmp.getHeight() * 4;
        }
        else
        {
            printf("Err bmp %d has unknown format\r\n", i);
        }
    }
    printf("Total size: %d\r\n", acc);

    printf("RAM usage:\r\n");
    printf("  MainMenuCarouselView:         %d\r\n", sizeof(MainMenuCarouselView));
    printf("  MainMenuAnimatingButtonsView: %d\r\n", sizeof(MainMenuAnimatingButtonsView));
    printf("  Game2048View:                 %d\r\n", sizeof(Game2048View));
    printf("  GameKnightView:               %d\r\n", sizeof(GameKnightView));
    printf("  ControlsView:                 %d\r\n", sizeof(ControlsView));
    printf("  HomeAutomationView:           %d\r\n", sizeof(HomeAutomationView));
    printf("  SettingsView:                 %d\r\n", sizeof(SettingsView));
    printf("  AudioPlayerView:              %d\r\n", sizeof(AudioPlayerView));
    printf("  VideoPlayerView:              %d\r\n", sizeof(VideoPlayerView));
    printf("---\r\n");
    printf("  MainMenuPresenter:            %d\r\n", sizeof(MainMenuPresenter));
    printf("  Game2048Presenter:            %d\r\n", sizeof(Game2048Presenter));
    printf("  GameKnightPresenter:          %d\r\n", sizeof(GameKnightPresenter));
    printf("  ControlsPresenter:            %d\r\n", sizeof(ControlsPresenter));
    printf("  HomeAutomationPresenter:      %d\r\n", sizeof(HomeAutomationPresenter));
    printf("  SettingsPresenter:            %d\r\n", sizeof(SettingsPresenter));
    printf("  AudioPlayerPresenter:         %d\r\n", sizeof(AudioPlayerPresenter));
    printf("  VideoPlayerPresenter:         %d\r\n", sizeof(VideoPlayerPresenter));
    printf("---\r\n");
    printf("  Model:                        %d\r\n", sizeof(Model));
    printf("TOTAL: %d\r\n", sizeof(FrontendHeap));


    touchgfx::HAL::getInstance()->taskEntry(); //Never returns

    return EXIT_SUCCESS;
}
