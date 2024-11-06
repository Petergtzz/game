/* ============================================================================
    $File: $
    $Date: 04/11/2024
    $Revision: $
    $Creator: Pedro Gutierrez
   ========================================================================= */

#include "main.h"

#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdbool.h>

#define internal static
#define global_variable static

typedef struct {
    void *memory; 
    s32 width;
    s32 height;
    u32 pitch;
    u32 bytes_per_pixel; 
} screen_buffer;

typedef struct {
    int height;
    int width;
} screen_dimensions;

global_variable bool running;
global_variable screen_buffer global_screen_buffer;

SDL_Texture *color_texture;

internal void osx_setup_screen(SDL_Renderer *renderer, 
                                screen_buffer *buffer,
                                int width,
                                int height)
{
    // Check if buffer memory already exists, if it does, free it. 
    if(buffer->memory)
    {
        free(buffer->memory);
    }

    buffer->width = width;
    buffer->height = height; 
    buffer->bytes_per_pixel = 4;
    buffer->pitch = width * buffer->bytes_per_pixel;
    
    buffer->memory = malloc(buffer->bytes_per_pixel * 
                            buffer->width * 
                            buffer->height);
    
    // Todo: Should this function call be here??
    color_texture = SDL_CreateTexture(renderer,
                                      SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      buffer->width,
                                      buffer->height);
}

internal void osx_process_event(SDL_Window *window) 
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type)
    {
        case SDL_WINDOWEVENT:
        {
            /*
            switch (event.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    int width, height;
                    SDL_GetWindowSize(window, &width, &height);
                    osx_resize_screen(renderer, &global_screen_buffer, width, height);
                } break; 
            } break;
            */
        } break;

        case SDL_QUIT:
        {
            running = false;
        } break;

        
        default:
        {
            // TODO
        } break;
    }
}

internal void osx_update_screen(screen_buffer *buffer, 
                                u32 x_offset, 
                                u32 y_offset)
{
    // Remember: big-endian architecture -- we specified pixel format as ARGB, 
    // but in memory pixels are stored as BGRA
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

internal void osx_render_to_screen(SDL_Renderer *renderer, screen_buffer buffer)
{
    SDL_UpdateTexture(color_texture, 
                      NULL, 
                      buffer.memory, 
                      buffer.pitch);
    SDL_RenderCopy(renderer, 
                   color_texture, 
                   NULL, 
                   NULL);
    SDL_RenderPresent(renderer);

}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        fprintf(stderr, "Error: Could not initialize SDL.\n");
    }

    SDL_DisplayMode display_area;
    SDL_GetDisplayMode(0, 0, &display_area);

    SDL_Window *window = SDL_CreateWindow("Game",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          display_area.w,
                                          display_area.h,
                                          SDL_WINDOW_RESIZABLE);

    if(window)
    {
        SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

        if(renderer)
        {
            // Todo: Change function name
            osx_setup_screen(renderer, &global_screen_buffer, 
                             display_area.w, display_area.h);

            u32 x_offset = 0;
            u32 y_offset = 0;

            // Game loop
            running = true; 
            while(running)
            {
                // Process input
                // TODO: Function does not take either global buffer or render.
                // Make sure to clean it up. 
                osx_process_event(window);
                
                SDL_GameController *game_controller;
                SDL_GameControllerGetAttached(game_controller);

                // Render
                osx_update_screen(&global_screen_buffer, x_offset, y_offset);
                osx_render_to_screen(renderer, global_screen_buffer);

                // Todo: Should I be clearing the memory buffer in each frame??
                
                x_offset++;
            }
        }
        else
        {
            fprintf(stderr, "Error: Unable to initialize renderer.\n");
        }
    }
    else
    {
        fprintf(stderr, "Error: Unable to initialize window handle.\n");
    }

    return 0;
}
