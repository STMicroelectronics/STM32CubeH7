#include "gui/audio_player_screen/visualization.h"

#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#ifdef STM32H743xx
#ifdef __GNUC__
#define LOCATION_DTCM_ATTRIBUTE __attribute__ ((section ("DTCMSection"))) __attribute__ ((aligned(4)))
#define LOCATION_DTCM_PRAGMA
#define LOCATION_SRAM1_ATTRIBUTE __attribute__ ((section ("SRAM1Section"))) __attribute__ ((aligned(4)))
#define LOCATION_SRAM1_PRAGMA
#elif defined(__arm__)
#define LOCATION_DTCM_ATTRIBUTE __attribute__ ((section ("DTCMSection"))) __attribute__ ((aligned(4)))
#define LOCATION_DTCM_PRAGMA
#define LOCATION_SRAM1_ATTRIBUTE __attribute__ ((section ("SRAM1Section"))) __attribute__ ((aligned(4)))
#define LOCATION_SRAM1_PRAGMA
#else
#include <intrinsics.h>
#define LOCATION_DTCM_PRAGMA _Pragma("location=\"DTCMSection\"")
#define LOCATION_DTCM_ATTRIBUTE
#define LOCATION_SRAM1_PRAGMA _Pragma("location=\"SRAM1Section\"")
#define LOCATION_SRAM1_ATTRIBUTE
#endif
#endif

#ifdef WIN32
static uint8_t front_buffer[VIZ_WIDTH * VIZ_HEIGHT];
static uint8_t back_buffer[VIZ_WIDTH * VIZ_HEIGHT];
static int16_t deform_offset_x[VIZ_WIDTH * VIZ_HEIGHT];
static int16_t deform_offset_y[VIZ_WIDTH * VIZ_HEIGHT];
#else
#ifdef VIZ_WORKSPACE_IN_SDRAM
static uint8_t *front_buffer;
static uint8_t *back_buffer;
static int16_t *deform_offset_x;
static int16_t *deform_offset_y;
#else
static uint8_t front_buffer[VIZ_WIDTH * VIZ_HEIGHT];
static uint8_t back_buffer[VIZ_WIDTH * VIZ_HEIGHT];
LOCATION_SRAM1_PRAGMA static int16_t deform_offset_x[VIZ_WIDTH * VIZ_HEIGHT] LOCATION_SRAM1_ATTRIBUTE;
LOCATION_SRAM1_PRAGMA static int16_t deform_offset_y[VIZ_WIDTH * VIZ_HEIGHT] LOCATION_SRAM1_ATTRIBUTE;
#endif
#endif

#define PI          3.141529f

static int current_effect;
static struct viz_palette palette;

static void to_polar(int x, int y, float* d, float* r)
{
    float distance, angle;

    distance = sqrtf((float)(x * x + y * y));
    angle = acosf((float)(x / distance));

    if (y < 0)
        angle = -angle;

    *d = distance;
    *r = angle;
}

static void to_cartesian(float d, float r, int* x, int* y)
{
    *x = (int)(d * cosf(r));
    *y = (int)(d * sinf(r));
}

static void deform(uint8_t* src, uint8_t* dest)
{
    int x, y;
    int offset_x, offset_y;
    int index = 0;

    for (y = 0; y < VIZ_HEIGHT; y++) {
        for (x = 0; x < VIZ_WIDTH; x++) {
            offset_x = deform_offset_x[index];
            offset_y = deform_offset_y[index];
            if (offset_x >= 0 && offset_x < VIZ_WIDTH
                && offset_y >= 0 && offset_y < VIZ_HEIGHT)
                dest[index] = src[offset_x + offset_y * VIZ_WIDTH];
            else dest[index] = 0;
            index++;
        }
    }
}

static void horizontal_deform(void)
{
    int x, y;
    int offset = 0;

    for (y = 0; y < VIZ_HEIGHT; y++) {
        for (x = 0; x < VIZ_WIDTH; x++) {
            deform_offset_x[offset] = (int)((VIZ_WIDTH / 2 - x) / 5 + x);
            deform_offset_y[offset] = (int)y;
            offset++;
        }
    }
}

#define PROJECTION  (4.0f / 3)

static void radial_deform(void)
{
    int x, y;
    int offset = 0;

    for (y = 0; y < VIZ_HEIGHT; y++) {
        for (x = 0; x < VIZ_WIDTH; x++) {
            float u = (float)(x - VIZ_WIDTH / 2);
            float v = (float)(y - VIZ_HEIGHT / 2);
            float d = sqrtf(u * u + v * v);
            float z = 40.0f * fabsf(sinf(d / 30)) + 40;
            deform_offset_x[offset] = (int)((PROJECTION * u / z - u) / 32.0f + x);
            deform_offset_y[offset] = (int)((PROJECTION * v / z - v) / 32.0f + y);
            offset++;
        }
    }
}

static void rotation_deform(void)
{
    int x, y;
    int offset = 0;

    for (y = 0; y < VIZ_HEIGHT; y++) {
        for (x = 0; x < VIZ_WIDTH; x++) {
            float u = (float)(x - VIZ_WIDTH / 2);
            float v = (float)(y - VIZ_HEIGHT / 2);
            deform_offset_x[offset] = (int)(cosf(0.0349f) * u + sinf(0.0349f) * v - u + x);
            deform_offset_y[offset] = (int)(-sinf(0.0349f) * u + cosf(0.0349f) * v - v + y);
            offset++;
        }
    }
}

static void swirl_deform(void)
{
    int x, y;
    int offset = 0;

    for (y = 0; y < VIZ_HEIGHT; y++) {
        for (x = 0; x < VIZ_WIDTH; x++) {
            float r, d;
            int u = x - VIZ_WIDTH / 2;
            int v = y - VIZ_HEIGHT / 2;
            to_polar(u, v, &d, &r);
            r = r + 0.1f * cosf(d / 20);
            to_cartesian(d, r, &u, &v);
            deform_offset_x[offset] = (int)(u + VIZ_WIDTH / 2);
            deform_offset_y[offset] = (int)(v + VIZ_HEIGHT / 2);
            offset++;
        }
    }
}

static void mosaic_deform(void)
{
    int x, y;
    int offset = 0;

    for (y = 0; y < VIZ_HEIGHT; y++) {
        for (x = 0; x < VIZ_WIDTH; x++) {
            float u = (float)(x - VIZ_WIDTH / 2) / (float)(VIZ_WIDTH / 2);
            float v = (float)(y - VIZ_HEIGHT / 2) / (float)(VIZ_HEIGHT / 2);
            float tmpx = u * 0.94f + sinf(v * PI * 8) * 0.03f;
            float tmpy = v * 0.94f + sinf(u * PI * 8) * 0.03f;
            deform_offset_x[offset] = (int)((VIZ_WIDTH / 2) * tmpx + VIZ_WIDTH / 2);
            deform_offset_y[offset] = (int)((VIZ_HEIGHT / 2) * tmpy + VIZ_HEIGHT / 2);
            offset++;
        }
    }
}

static uint32_t quarter_mask = 0xf8f8f8f8;
static uint32_t half_mask = 0xfefefefe;
#if defined(STM32H743xx)
static uint32_t attenuation_mask = 0x01010101;
#endif
#ifdef __GNUC__
__attribute__( ( always_inline ) ) inline uint32_t __uqsub8(uint32_t op1, uint32_t op2)
{
    uint32_t result;

    __asm volatile ("uqsub8 %0, %1, %2" : "=r" (result) : "r" (op1), "r" (op2) );
    return(result);
}
#endif

static void blur_filter(struct viz_effect *effect, uint8_t *src, uint8_t *dest)
{
    register uint32_t pixel0, pixel1, pixel2, pixel3, pixel4;

    int count = (VIZ_WIDTH * (VIZ_HEIGHT - 2)) / 4; /* skip two lines */

    src += VIZ_WIDTH;
    dest += VIZ_WIDTH;

    while (count-- > 0) {
        pixel0 = *(uint32_t*)src;
        pixel0 &= half_mask;
        pixel0 >>= 1;

        pixel1 = *(uint32_t*)(src - 1);
        pixel1 &= quarter_mask;
        pixel1 >>= 3;
        pixel0 += pixel1;

        pixel2 = *(uint32_t*)(src + 1);
        pixel2 &= quarter_mask;
        pixel2 >>= 3;
        pixel0 += pixel2;

        pixel3 = *(uint32_t*)(src - VIZ_WIDTH);
        pixel3 &= quarter_mask;
        pixel3 >>= 3;
        pixel0 += pixel3;

        pixel4 = *(uint32_t*)(src + VIZ_WIDTH);
        pixel4 &= quarter_mask;
        pixel4 >>= 3;
        pixel0 += pixel4;

        *(uint32_t*)dest = pixel0;

        src += 4;
        dest += 4;
    }

    if (effect->attenuation) {
        /* attenuate pixels to avoid stationary state */
#if defined(STM32H743xx)
        count = (VIZ_WIDTH * (VIZ_HEIGHT - 2)) / 4;
        dest -= VIZ_WIDTH * (VIZ_HEIGHT - 1);
        while (count-- > 0) {
            uint32_t pixel = *(uint32_t*)dest;
#ifdef __arm__
            *(uint32_t*)dest = __uqsub8(pixel, attenuation_mask);
#else
            *(uint32_t*)dest = __UQSUB8(pixel, attenuation_mask);
#endif
            dest += 4;
        }
#else /* WIN32 */
        count = (VIZ_WIDTH * (VIZ_HEIGHT - 2));
        dest -= VIZ_WIDTH * (VIZ_HEIGHT - 1);
        while (count-- > 0) {
            uint8_t pixel = *dest;
            if (pixel > 0)
                pixel--;
            *dest++ = pixel;
        }
#endif
    }
}

struct viz_effect viz_effects[5] = {
    { radial_deform, deform, "radial effect", true},
    { horizontal_deform, deform, "zoom effect", false },
    { rotation_deform, deform, "rotation effect", true },
    { swirl_deform, deform, "swirl effect", true },
    { mosaic_deform, deform, "mosaic effect", true }
};

static uint32_t fire_palette[256] = {
#include "palette.txt"
};

void visualization_initialize()
{
    current_effect = 0;

#ifdef VIZ_WORKSPACE_IN_SDRAM
#define OFFSET 0x200000
    front_buffer = (uint8_t*)(0xC0000000 + OFFSET);
    back_buffer = front_buffer + VIZ_WIDTH * VIZ_HEIGHT;
    deform_offset_x = (uint16_t*)(back_buffer + VIZ_WIDTH * VIZ_HEIGHT);
    deform_offset_y = deform_offset_x + VIZ_WIDTH * VIZ_HEIGHT;
#endif

    memset(front_buffer, 0, VIZ_WIDTH * VIZ_HEIGHT);
    memset(back_buffer, 0, VIZ_WIDTH * VIZ_HEIGHT);

    viz_effects[current_effect].init();
#if 0
    for (int i = 0; i < 256; i++) {
        palette.color[i].r = i;
        palette.color[i].g = i;
        palette.color[i].b = i;
    }
#else
    for (int i = 0; i < 256; i++) {
        palette.color[i].r = (fire_palette[i] & 0xff0000) >> 16;
        palette.color[i].g = (fire_palette[i] & 0xff00) >> 8;
        palette.color[i].b = fire_palette[i] & 0xff;
    }
#endif
}

void visualization_set_palette(struct viz_palette* p)
{
    memcpy(&palette, p, sizeof(struct viz_palette));
}

struct viz_effect* visualization_current()
{
    return &viz_effects[current_effect];
}

void visualization_next()
{
    current_effect++;
    current_effect %= sizeof(viz_effects) / sizeof(struct viz_effect);

    viz_effects[current_effect].init();
}

static void draw_line(uint8_t *framebuffer, int x1, int y1, int x2, int y2)
{
    int x, y, dx, dy, dx1, dy1;
    int px, py, xe, ye, i;

    dx = x2 - x1;
    dy = y2 - y1;
    dx1 = abs(dx);
    dy1 = abs(dy);
    px = 2 * dy1 - dx1;
    py = 2 * dx1 - dy1;
    
    if (dy1 <= dx1) {
        if (dx >= 0){
            x = x1;
            y = y1;
            xe = x2;
        } else {
            x = x2;
            y = y2;
            xe = x1;
        }

        framebuffer[x + y * VIZ_WIDTH] = 255;

        for(i = 0; x < xe; i++) {
            x = x + 1;
            if (px < 0) {
                px = px + 2 * dy1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    y = y + 1;
                } else {
                    y = y - 1;
                }
                px = px + 2 * (dy1 - dx1);
            }
            framebuffer[x + y * VIZ_WIDTH] = 255;
        }
    } else {
        if(dy >= 0) {
            x = x1;
            y = y1;
            ye = y2;
        } else {
            x = x2;
            y = y2;
            ye = y1;
        }

        framebuffer[x + y * VIZ_WIDTH] = 255;

        for(i = 0; y < ye; i++) {
            y = y + 1;
            if (py <= 0) {
                py = py + 2 * dx1;
            } else {
                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
                    x = x + 1;
                } else {
                    x = x - 1;
                }
                py = py + 2 * (dx1 - dy1);
            }
            framebuffer[x + y * VIZ_WIDTH] = 255;
        }
    }
}

void visualization_render(uint8_t *samples, int num_samples)
{
    /* draw new audio samples on top of the dest buffer */
    float x = 0, step = (float)(VIZ_WIDTH / num_samples);
    int prev_sample = ((int)samples[0] - 64) + (VIZ_HEIGHT / 2);
    for (int i = 1; i < num_samples; i++) {
        int new_sample = ((int)samples[i] - 64) + (VIZ_HEIGHT / 2);
        draw_line(front_buffer, (int)x, prev_sample, (int)(x + step), new_sample);
        x += step;
        prev_sample = new_sample;
    }

    /* apply deformation */
    viz_effects[current_effect].function(front_buffer, back_buffer);

    /* blur the dest buffer back into the src buffer */
    blur_filter(&viz_effects[current_effect], back_buffer, front_buffer);
}

void visualization_copy_framebuffer(void* framebuffer, int pitch, int x, int y, int width, int height)
{
#if !defined(USE_BPP) || USE_BPP==16
    uint8_t *src = front_buffer + x + y * VIZ_WIDTH;
    uint16_t *dest = (uint16_t*)framebuffer;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            uint8_t index = *src++;
            *dest++ = ((palette.color[index].r & 0xf8) << 8)
                        | ((palette.color[index].g & 0xfc) << 3)
                        | ((palette.color[index].b & 0xf8) >> 3);
        }
        dest += pitch - VIZ_WIDTH;
    }
#elif USE_BPP==24
    uint8_t *src = front_buffer + x + y * VIZ_WIDTH;
    uint8_t *dest = (uint8_t*)framebuffer;

    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            uint8_t index = *src++;
            *dest++ = (palette.color[index].r & 0xff);
            *dest++ = (palette.color[index].g & 0xff);
            *dest++ = (palette.color[index].b & 0xff);
        }
        dest += (pitch - VIZ_WIDTH) * 3;
    }
#else
#error Unknown USE_BPP
#endif
}
