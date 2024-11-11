/* ============================================================================
    $File: $
    $Date: 2024-11-11
    $Revision: $
    $Creator: Pedro Gutierrez
   ========================================================================= */

#include "game.h"

internal void RenderWierdGradient(gamescreen_buffer *buffer, int x_offset, int y_offset)
{
    // NOTE: Big-endian architecture, pixel order is format reversed
    u8 *row = (u8*)buffer->memory;
    for (int y = 0; y < buffer->height; y++)
    {
        u32 *pixel = (u32*)row;
        for (int x = 0; x < buffer->width; x++)
        {
            u8 blue  = (x + x_offset);
            u8 red   = (y + y_offset);

            *pixel++ =((red << 16) | blue);
        }

        row += buffer->pitch;
    }
}

void GameUpdateAndRender(gamescreen_buffer *buffer, int x_offset, int y_offset)
{
    RenderWierdGradient(buffer, x_offset, y_offset);
}
