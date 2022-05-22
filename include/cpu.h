#ifndef cpu_file
#define cpu_file

    #include <stdint.h>
    #include "display.h"
    #include <stdbool.h>
    #include <SDL.h>

    /* Struct */
    typedef struct
    {
        uint8_t MEMORY_MAP[4096];
        uint8_t Vx[16]; /* General Purpose Register */ 
        uint16_t I; 
        uint8_t DELAY;
        uint8_t SOUND_TIMER;

        uint16_t PC; /* Program Counter */
        uint8_t SP; /* Stack Pointer */

        uint16_t Stack[16];
        uint8_t Optcode;
        uint8_t SecondNibble;
        uint8_t ThirdNibble;
        uint8_t FourthNibble;

        uint32_t Screen[Screen_Height * Screen_Width]; /* Height (x) * width (y) */
        uint8_t Keyboard[16];
        bool    IsKeyPress;

        SDL_Window * Window;
        SDL_Renderer * Renderer;
        SDL_Texture * Texture;
    } Chip8_t;

    /* Types */
    extern Chip8_t Chip8;

    /* prototype */
    void cpu_init(void);
    void increase_pc(int value);
    uint16_t read_memory(int counter);
    void decode(uint16_t instruction);   

#endif