#ifndef TYPES_H
#define TYPES_H

#include <stdint.h>

/* Packed color format: 0xAABBGGRR. The PPM writer ignores the alpha byte. */
#define RED        UINT32_C(0x000000ff)
#define GREEN      UINT32_C(0x0000ff00)
#define BLUE       UINT32_C(0x00ff0000)
#define YELLOW     UINT32_C(0x0000ffff)
#define GRAY       UINT32_C(0x0022252b)
#define WHITE      UINT32_C(0xffffffff)

#define BLACK      UINT32_C(0x00000000)
#define ORANGE     UINT32_C(0x0000a5ff)
#define PURPLE     UINT32_C(0x00800080)
#define MAGENTA    UINT32_C(0x00ff00ff)
#define CYAN       UINT32_C(0x00ffff00)
#define PINK       UINT32_C(0x00cbc0ff)
#define BROWN      UINT32_C(0x002a2aa5)
#define DARK_GRAY  UINT32_C(0x00404040)
#define LIGHT_GRAY UINT32_C(0x00d3d3d3)
#define NAVY       UINT32_C(0x00800000)
#define TEAL       UINT32_C(0x00808000)
#define LIME       UINT32_C(0x0000ff00)
#define GOLD       UINT32_C(0x0000d7ff)
#define SKY_BLUE   UINT32_C(0x00ebce87)
#define CORAL      UINT32_C(0x00507fff)
#define VIOLET     UINT32_C(0x00ee82ee)

uint32_t G_COLORS[] = {BLACK, ORANGE, PURPLE, MAGENTA, CYAN, PINK, BROWN, DARK_GRAY, LIGHT_GRAY, NAVY, TEAL, LIME, GOLD, SKY_BLUE, CORAL, VIOLET};

typedef struct {uint8_t a, b, g, r} Color;
typedef struct {float x; float y;} Vec2;

#endif /* TYPES_H */
