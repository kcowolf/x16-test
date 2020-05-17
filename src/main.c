#include <cx16.h>
#include <stdio.h>
#include <stdint.h>
#include <cbm.h>
#include "load.h"
#include "vload.h"

#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"
#include "GFX_vera.h"

void initVera();
int loadResources();

int main()
{
    if (!loadResources())
    {
        return 1;
    }

    initVera();

    GAME_init();

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
    VERA.layer0.mapbase = GFX_BACKGROUND_MAP_BASE_ADDR >> 9;
    VERA.layer0.tilebase = GFX_BACKGROUND_TILE_BASE_ADDR >> 9 | VERA_TILE_HEIGHT_8 | VERA_TILE_WIDTH_8;
    VERA.layer0.hscroll = 0;
    VERA.layer0.vscroll = 0;

    VERA.layer1.config = VERA_CONFIG_MAP_WIDTH_64 | VERA_CONFIG_MAP_HEIGHT_32 | VERA_CONFIG_4BPP;
    VERA.layer1.mapbase = GFX_FOREGROUND_MAP_BASE_ADDR >> 9;
    VERA.layer1.tilebase = GFX_FOREGROUND_TILE_BASE_ADDR >> 9 | VERA_TILE_HEIGHT_8 | VERA_TILE_WIDTH_8;
    VERA.layer1.hscroll = 0;
    VERA.layer1.vscroll = 0;

    VERA.display.video = VERA_VIDEO_LAYER0_ENABLED | VERA_VIDEO_LAYER1_ENABLED | VERA_VIDEO_OUTPUT_VGA;

    videomode(VIDEOMODE_40x30);

    for (y = 0; y < GFX_PLANE_HEIGHT; y++)
    {
        for (x = 0; x < GFX_PLANE_WIDTH; x++)
        {
            GFX_setBackgroundTile(x, y, 0);
            GFX_setForegroundTile(x, y, 0);
        }
    }
}

int loadResources()
{
    int result;

    // Load palette
    result = vload_host(PALBG1_FILENAME, GFX_PALETTE_ADDR);
    if (!result)
	{
		printf("  failed to load palette\n");
		return 0;
	}

	// Load background tileset
    result = vload_host(TSBG1_FILENAME, GFX_BACKGROUND_TILE_BASE_ADDR);
    if (!result)
	{
        printf("  failed to load bg tiles\n");
        return 0;
    }

	// Load foreground tileset
    result = vload_host(TSBG1_FILENAME, GFX_FOREGROUND_TILE_BASE_ADDR);
    if (!result)
	{
        printf("  failed to load fg tiles\n");
        return 0;
    }

	// Load background tilemap to bank
    result = load_bank_host(TMBG1_FILENAME, GAME_BKG1_MBANK);
    if (!result) 
	{
        printf("  failed to load bg tilemap\n");
        return 0;
	}

    return 1;
}
