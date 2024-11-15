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

typedef struct 
{
    real32 half_transition_count;
    real32 ended_down;
} game_button_state;

typedef struct 
{
    bool is_analog; 
    
    real32 start_x;
    real32 start_y;

    real32 min_x;
    real32 min_y;

    real32 max_x;
    real32 max_y;

    real32 end_x;
    real32 end_y;
 
    union
    {
        game_button_state buttons[6];
        struct
        {
            game_button_state up;
            game_button_state down;
            game_button_state left; 
            game_button_state right;
            game_button_state left_shoulder;
            game_button_state right_shoulder;
        };
    };
} game_controller_input;

typedef struct
{
    game_controller_input controllers[2];
} game_input;

void GameUpdateAndRender(game_input *input, gamescreen_buffer *buffer);

#endif
