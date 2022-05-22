#include <stdio.h>
#include <SDL.h>

#include "main.h"
#include "cpu.h"
#include "display.h"
#include <time.h>


/* Global */
Chip8_t Chip8;

/* main.c */
int main(int argc, char *argv[])
{
    int status;
    int address;
    uint16_t instruction;

    /* Init CPU */
    cpu_init(); 

    /* Copy Rom */
    //status = copyToMemory("IBM Logo.ch8");
    //status = copyToMemory("test_opcode.ch8");
    //status = copyToMemory("c8_test.ch8");
    //status = copyToMemory("chip8-test-rom.ch8");
    //status = copyToMemory("SCTEST.CH8");
    //status = copyToMemory("pong.rom");
    //status = copyToMemory("CONNECT4");
    //status = copyToMemory("invader");
    //status = copyToMemory("keypadtest");
    //status = copyToMemory("pong1");
    status = copyToMemory("tetris.rom");

    if(!status)
    {
        printf("error copying file");
        return -1;
    }

    /* Setup Graphics */
    display_init();
    //draw();
    /* Fetch -> Decode -> Execute */

    while (1)
    {
        address = Chip8.PC;
        instruction = read_memory(address);
        decode(instruction); 
        keyboard();

        update_timer();

        SDL_Delay(2);
    }
    
    destory_display();
}


int copyToMemory(char *file){

    FILE *fp = fopen(file, "r");
    long size;

    if (fp == NULL )
    {
        printf( "file failed to open." );
        fclose(fp);
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    if(size > 3584)
    {
        printf("sizes too big");
        fclose(fp);
        return -1;
    }
    rewind(fp);

    fread(&Chip8.MEMORY_MAP[512], sizeof(uint8_t), size, fp);
    
    fclose(fp);

    return 1;
}

void update_timer(void)
{
    if(Chip8.DELAY > 0)
    {
        Chip8.DELAY--;
    }

    if(Chip8.SOUND_TIMER > 0)
    {
        Chip8.SOUND_TIMER--;
    }
}


