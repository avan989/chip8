#include <SDL.h>

#include "display.h"
#include "cpu.h"
#include <stdbool.h>

uint8_t keyboard_map[NumberOfKey] = 
{
    SDLK_x, /* 0 */
    SDLK_1, /* 1 */
    SDLK_2, /* 2 */
    SDLK_3, /* 3 */
    SDLK_q, /* 4 */
    SDLK_w, /* 5 */
    SDLK_e, /* 6 */
    SDLK_a, /* 7 */
    SDLK_s, /* 8 */
    SDLK_d, /* 9 */
    SDLK_z, /* A */
    SDLK_c, /* B */
    SDLK_4, /* C */
    SDLK_r, /* D */
    SDLK_f, /* E */
    SDLK_v  /* F */
};

const uint8_t FontDisplay[SizeFont] = 
    {
        0xF0, 0x90, 0x90, 0x90, 0xF0, /* 0 */
        0x20, 0x60, 0x20, 0x20, 0x70, /* 1 */
        0xF0, 0x10, 0xF0, 0x80, 0xF0, /* 2 */
        0xF0, 0x10, 0xF0, 0x10, 0xF0, /* 3 */
        0x90, 0x90, 0xF0, 0x10, 0x10, /* 4 */
        0xF0, 0x80, 0xF0, 0x10, 0xF0, /* 5 */
        0xF0, 0x80, 0xF0, 0x90, 0xF0, /* 6 */
        0xF0, 0x10, 0x20, 0x40, 0x40, /* 7 */
        0xF0, 0x90, 0xF0, 0x90, 0xF0, /* 8 */
        0xF0, 0x90, 0xF0, 0x10, 0xF0, /* 9 */
        0xF0, 0x90, 0xF0, 0x90, 0x90, /* A */
        0xE0, 0x90, 0xE0, 0x90, 0xE0, /* B */
        0xF0, 0x80, 0x80, 0x80, 0xF0, /* C */
        0xE0, 0x90, 0x90, 0x90, 0xE0, /* D */
        0xF0, 0x80, 0xF0, 0x80, 0xF0, /* E */
        0xF0, 0x80, 0xF0, 0x80, 0x80  /* F */
    };

void display_init(void){

    for(int i = 0; i < SizeFont; i++)
    {
        Chip8.MEMORY_MAP[i] = FontDisplay[i];
    }

    SDL_Init(SDL_INIT_VIDEO); /* Initialize SDL2 */

    Chip8.Window = SDL_CreateWindow("Chip 8 Emulator", 
                              SDL_WINDOWPOS_CENTERED, 
                              SDL_WINDOWPOS_CENTERED, 
                              Screen_Width * Scale, 
                              Screen_Height * Scale, 
                              SDL_WINDOW_RESIZABLE);

    Chip8.Renderer = SDL_CreateRenderer(Chip8.Window, -1, SDL_RENDERER_ACCELERATED);
    Chip8.Texture = SDL_CreateTexture(Chip8.Renderer,
                                SDL_PIXELFORMAT_RGBA8888, 
                                SDL_TEXTUREACCESS_TARGET, 
                                Screen_Width,
                                Screen_Height);

    /* Make screen black */
    SDL_SetRenderDrawColor(Chip8.Renderer, 0, 0, 0, 0);
    SDL_RenderClear(Chip8.Renderer);
    SDL_RenderPresent(Chip8.Renderer);
}

void destory_display(void)
{
    SDL_DestroyWindow(Chip8.Window);
    SDL_DestroyRenderer(Chip8.Renderer);
    SDL_DestroyTexture(Chip8.Texture);
    SDL_Quit();

}

void draw(void)
{
    /* 1. Read From Memory Address I 
    ** 2. Width of 8
    ** 3. Height of N-> Determine by last bytes in instruction
    ** 4. Draw Start at Vx, Vy on Screen 
    */

    /*
    Chip8.I = 5;
    uint8_t N_Height = 5;
    uint8_t Vx = 0;
    uint8_t Vy = 0;
    */

    /* Get Value from instruction */
    uint8_t Vx = Chip8.Vx[Chip8.SecondNibble]; /* Get X position to draw */
    uint8_t Vy = Chip8.Vx[Chip8.ThirdNibble]; /* Get Y position to draw */
    uint8_t N_Height = Chip8.FourthNibble;
    uint8_t * Sprite = &Chip8.MEMORY_MAP[Chip8.I]; /* Get sprite from I */

    uint16_t InitalPos = ((Screen_Width * Vy) + Vx);

    Chip8.Vx[0xF] = 0;
    for(int y = 0; y < N_Height; y++) /* Iterate through the Y axis*/ 
    {
        for(int x = 0; x < 8; x++) /* iterate through the x axis */
        {
            /* If the bit is set, set the corresponding value in the screen */
            //printf("%01x", (Sprite[y] >> (7 - x)) & 0x1);
            if(((Sprite[y] >> (7 - x)) & 0x1) > 0)
            {
                /* Single Array */
                if(Chip8.Screen[(InitalPos + ((Screen_Width * y) + x)) % (2048)] > 0)
                {
                    Chip8.Vx[0xF] = 1;
                }

                Chip8.Screen[(InitalPos + ((Screen_Width * y) + x)) % (2048)] ^= 0xFFFFFFFF;
            }   
            //printf("Position: %d\n", InitalPos + ((Screen_Width * y) + x));
            //printf("Sprite: %d, Screen: %u\n", (Sprite[y] >> (7 - x)) & 0x1, Chip8.Screen[InitalPos + ((Screen_Width * y) + x)]);
        }
    }

    //print_terminal();
    /*
    for(int i = 0; i < 32 * 64; i++)
    {
        printf("i: %d, value: %x\n", i, Chip8.Screen[i]);
    }
    printf("\n*********** END *************\n");
    */

    /*
    printf("VF: %d", Chip8.VF);
    
    */
    SDL_UpdateTexture(Chip8.Texture, NULL, Chip8.Screen, Screen_Width * sizeof(uint32_t));
    SDL_RenderClear(Chip8.Renderer);
    SDL_RenderCopy(Chip8.Renderer, Chip8.Texture, NULL, NULL);
    SDL_RenderPresent(Chip8.Renderer);
    
}

void print_terminal(void)
{
    printf("\n");
    for(int i = 0; i < Screen_Height * Screen_Width; i++)
    {
        if(i % Screen_Width == 0 && i != 0)
        {
            printf("\n");
        }

        if(Chip8.Screen[i] == 1)
        {
            printf("*");
        }
        else
        {
            printf(" ");
        }  
    }
}

int keyboard()
{
    SDL_Event evt;
    bool return_code = false;
    int i;

    while(SDL_PollEvent(&evt))
    {
        if(evt.type == SDL_QUIT)
        {
            printf("Quit");
            exit(1);
        }
        else if( evt.type == SDL_KEYDOWN )
        {
            for(i = 0; i < NumberOfKey; i++)
            {
                if(evt.key.keysym.sym == keyboard_map[i])
                {
                    update_keyboard(i, true);
                    return_code = true;
                }
            }
        }
        else if(evt.type == SDL_KEYUP)
        {
            for(i = 0; i < NumberOfKey; i++)
            {
                if(evt.key.keysym.sym == keyboard_map[i])
                {
                    update_keyboard(i, false);
                    return_code = false;
                }
            }

        }
    }

    return return_code;
}

void update_keyboard(int key, bool value)
{
    Chip8.Keyboard[key] = value;
}