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
} window_buffer;

typedef struct {
    int height;
    int width;
} window_dimensions;

global_variable bool running;
global_variable window_buffer global_window_buffer;
// TODO: Change it to not be global. 
SDL_Texture *color_texture;

internal void MacOsSetupScreen(SDL_Renderer *renderer, 
                               window_buffer *buffer,
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
    
    // Todo: Should this function called be here??
    color_texture = SDL_CreateTexture(renderer,
                                      SDL_PIXELFORMAT_ARGB8888,
                                      SDL_TEXTUREACCESS_STREAMING,
                                      buffer->width,
                                      buffer->height);
}

internal void MacOsHandleEvent(SDL_Window *window) 
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch(event.type)
    {
        case SDL_KEYDOWN: 
        {
        } break;

        case SDL_KEYUP:
        {
            SDL_KeyCode key = event.key.keysym.sym;
            bool is_down = ((key & 1 << 30) != 0); 

            if(is_down)
            {
                // TODO: Add more keys - space, esc, q, e, etc
                if(key == SDLK_a)
                {
                    printf("A\n");
                }
                else if(key == SDLK_w) 
                {
                    printf("W\n");
                }
                else if(key == SDLK_s) 
                {
                    printf("S\n");
                }
                else if(key == SDLK_d) 
                {
                    printf("D\n");
                }
            }
        } break;

        case SDL_WINDOWEVENT:
        {
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

internal void MacOsUpdateScreen(window_buffer *buffer, 
                                u32 x_offset, 
                                u32 y_offset)
{
    // Note: Big-endian architecture -- specified pixel format as -> ARGB 
    // Memory format as -> BGRA
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

internal void MacOsRenderToScreen(SDL_Renderer *renderer, window_buffer *buffer)
{
    SDL_UpdateTexture(color_texture, 
                      NULL, 
                      buffer->memory, 
                      buffer->pitch);
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
            MacOsSetupScreen(renderer, &global_window_buffer, 
                             display_area.w, display_area.h);

            u32 x_offset = 0;
            u32 y_offset = 0;

            // Game loop
            running = true; 
            while(running)
            {
                MacOsHandleEvent(window);
               
                // Note: First, open a controller, but to find it we must loop over 
                // all the joysticks to find a valid game pad. 
                SDL_GameController *controller = NULL;
                
                for(int joystick = 0; joystick < SDL_NumJoysticks(); joystick++)
                {
                    // Point the valid joystick to the controller and open it.  
                    if(SDL_IsGameController(joystick))
                    {
                        controller = SDL_GameControllerOpen(joystick);
                    }
             
                    // Check that the controller is valid and plugged in.
                    // Get input from controllers. 
                    if(controller != 0 && SDL_GameControllerGetAttached(controller))
                    {
                        bool up    = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_UP);
                        bool down  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
                        bool left  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
                        bool right = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);
                        bool start = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_START);
                        bool back  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_BACK);
                        bool left_shoulder  = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_LEFTSHOULDER);
                        bool right_shoulder = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER);
                        bool a_button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_A);
                        bool b_button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_B);
                        bool x_button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_X);
                        bool y_button = SDL_GameControllerGetButton(controller, SDL_CONTROLLER_BUTTON_Y);

                        u16 stick_x = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTX);
                        u16 stick_y = SDL_GameControllerGetAxis(controller, SDL_CONTROLLER_AXIS_LEFTY);

                        if(x_button)
                        {
                            y_offset += 10; 
                        }
                    }
                    else
                    {
                        // TODO: This controller is not plugged in.
                    } 
                }

                // TODO: When should we close the controller?

                MacOsUpdateScreen(&global_window_buffer, x_offset, y_offset);
                MacOsRenderToScreen(renderer, &global_window_buffer);

                // TODO: Should I be clearing the memory buffer in each frame??
                
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
