#include <cx16.h>
#include <stdint.h>
#include "GAME.h"
#include "GFX.h"
#include "GFX_util.h"

#define BANKED_RAM_START 0xA000

void GAME_init()
{
	uint8_t x = 0;
    uint8_t y = 0;
    uint16_t* tilemap = (uint16_t*)BANKED_RAM_START;

    // Write background tilemap to VRAM
    VIA1.pra = GAME_BKG1_MBANK;

    for (y = 0; y < TMBG1_TILE_HEIGHT; y++)
	{
        for (x = 0; x < TMBG1_TILE_WIDTH; x++)
		{
            uint16_t tile = tilemap[(y * TMBG1_TILE_WIDTH) + x];
			//GFX_setBackgroundTile(x, y, tile);  // Doesn't work -- black screen
            GFX_setForegroundTile(x, y, tile);  // Works as expected
        }
	}
}
