#ifndef GAME_H
#define GAME_H

/* ============================================================================
    $File: $
    $Date: 2024-11-11
    $Revision: $
    $Creator: Pedro Gutierrez
   ========================================================================= */

typedef struct
{
    void *memory;
    int width;
    int height;
    int pitch;
    int bytes_per_pixel; 
} gamescreen_buffer;

void GameUpdateAndRender(gamescreen_buffer *buffer, int x_offset, int y_offset);

#endif
