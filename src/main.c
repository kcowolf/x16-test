// Writing tilemap to the foreground layer (layer 1) works.  Writing it to background layer (layer 0) does not.
// (See initTileMap function for where this is written).

#include <cx16.h>
#include <stdio.h>
#include <stdint.h>
#include <cbm.h>
#include "load.h"
#include "vload.h"

#include "GFX.h"
#include "GFX_vera.h"

// VERA Memory map
// 0x0000-0x0FFF background tilemap (64 x 32 x 2 = 4096 bytes)
// 0x1000-0x1FFF foreground tilemap (64 x 32 x 2 = 4096 bytes)
// 0x2000-0x9FFF background tiles (1024 tiles * 32 bytes per tile = 32768 bytes)
// 0xA000-0x11FFF foreground tiles (1024 tiles * 32 bytes per tile = 32768 bytes)
// 0x12000-0x1F9BF unused

#define BACKGROUND_MAP_BASE_ADDR 0x0
#define FOREGROUND_MAP_BASE_ADDR 0x1000
#define BACKGROUND_TILE_BASE_ADDR 0x2000
#define FOREGROUND_TILE_BASE_ADDR 0xA000

#define PALETTE_ADDR 0x1FA00

#define PLANE_WIDTH 64
#define PLANE_WIDTH_TIMES_TWO 128
#define PLANE_HEIGHT 32

#define BANKED_RAM_START 0xA000

void initTileMap();

void initVera();

int loadResources();

void setBackgroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    );

void setForegroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    );

int main()
{
    initVera();

    if (!loadResources())
    {
        return 1;
    }

    return 0;
}

void initVera()
{
    uint8_t x = 0;
    uint8_t y = 0;

    // Initialize VERA
    VERA.control = 0;

    // Initialize VERA
    VERA.display.hscale = VERA_SCALE_1x;
    VERA.display.vscale = VERA_SCALE_1x;

    VERA.layer0.config = VERA_CONFIG_MAP_WIDTH_64 | VERA_CONFIG_MAP_HEIGHT_32 | VERA_CONFIG_4BPP;
    VERA.layer0.mapbase = BACKGROUND_MAP_BASE_ADDR >> 9;
    VERA.layer0.tilebase = BACKGROUND_TILE_BASE_ADDR >> 9 | VERA_TILE_HEIGHT_8 | VERA_TILE_WIDTH_8;
    VERA.layer0.hscroll = 0;
    VERA.layer0.vscroll = 0;

    VERA.layer1.config = VERA_CONFIG_MAP_WIDTH_64 | VERA_CONFIG_MAP_HEIGHT_32 | VERA_CONFIG_4BPP;
    VERA.layer1.mapbase = FOREGROUND_MAP_BASE_ADDR >> 9;
    VERA.layer1.tilebase = FOREGROUND_TILE_BASE_ADDR >> 9 | VERA_TILE_HEIGHT_8 | VERA_TILE_WIDTH_8;
    VERA.layer1.hscroll = 0;
    VERA.layer1.vscroll = 0;

    VERA.display.video = VERA_VIDEO_LAYER0_ENABLED | VERA_VIDEO_LAYER1_ENABLED | VERA_VIDEO_OUTPUT_VGA;

    videomode(VIDEOMODE_40x30);

    // Initialize both tilemaps to all empty tiles.
    for (y = 0; y < PLANE_HEIGHT; y++)
    {
        for (x = 0; x < PLANE_WIDTH; x++)
        {
            setBackgroundTile(x, y, 0);
            setForegroundTile(x, y, 0);
        }
    }
}

int loadResources()
{
    int result;

    // Load palette
    result = vload_host(PALBG1_FILENAME, PALETTE_ADDR);
    if (!result)
    {
        printf("  failed to load palette\n");
        return 0;
    }

    // Load background tileset
    result = vload_host(TSBG1_FILENAME, BACKGROUND_TILE_BASE_ADDR);
    if (!result)
    {
        printf("  failed to load bg tiles\n");
        return 0;
    }

    // Load foreground tileset
    result = vload_host(TSBG1_FILENAME, FOREGROUND_TILE_BASE_ADDR);
    if (!result)
    {
        printf("  failed to load fg tiles\n");
        return 0;
    }

    // Load background tilemap to VRAM
    result = vload_host(TMBG1_FILENAME, FOREGROUND_MAP_BASE_ADDR);  // TODO -- Using BACKGROUND_MAP_BASE_ADDR only shows black screen.
    if (!result) 
    {
        printf("  failed to load bg tilemap\n");
        return 0;
    }

    return 1;
}

void setBackgroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    )
{
    vpoke(tile >> 8, (y * PLANE_WIDTH_TIMES_TWO) + (2 * x) + 1 + BACKGROUND_MAP_BASE_ADDR);
    vpoke(tile & 0xFF, (y * PLANE_WIDTH_TIMES_TWO) + (2 * x) + BACKGROUND_MAP_BASE_ADDR);
}

void setForegroundTile
    (
    uint8_t x,
    uint8_t y,
    uint16_t tile
    )
{
    vpoke(tile >> 8, (y * PLANE_WIDTH_TIMES_TWO) + (2 * x) + 1 + FOREGROUND_MAP_BASE_ADDR);
    vpoke(tile & 0xFF, (y * PLANE_WIDTH_TIMES_TWO) + (2 * x) + FOREGROUND_MAP_BASE_ADDR);
}
