#ifndef VIZ_DEFORM__
#define VIZ_DEFORM__

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#undef VIZ_WORKSPACE_IN_SDRAM

#ifndef VIZ_WORKSPACE_IN_SDRAM
#define VIZ_WIDTH   320 /* must be a multiple of 4 */
#define VIZ_HEIGHT  200 /* must be a multiple of 4 */
#else
#define VIZ_WIDTH   640 /* must be a multiple of 4 */
#define VIZ_HEIGHT  200 /* must be a multiple of 4 */
#endif

struct viz_effect {
    void (*init) (void);
    void (*function)(uint8_t* src, uint8_t* dest);
    char description[32];
    bool attenuation;
};

extern struct viz_effect viz_effects[5];

struct viz_palette {
    struct {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } color[256];
};

void visualization_initialize(void);
struct viz_effect* visualization_current(void);

void visualization_set_palette(struct viz_palette* palette);
void visualization_next(void);

/* pitch is in pixels */
void visualization_render(uint8_t *samples, int num_samples);
void visualization_copy_framebuffer(void* framebuffer, int pitch, int x, int y, int width, int height);

#ifdef __cplusplus
}
#endif

#endif
