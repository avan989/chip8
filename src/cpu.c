#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "cpu.h"
#include "display.h"
#include "instruction.h"

void cpu_init()
{
    memset(&Chip8, 0, sizeof(Chip8_t));
    Chip8.PC = 0x200;
    memset(&Chip8.Keyboard, 0xFF, sizeof(Chip8.Keyboard));
    srand(time(NULL));
}

uint16_t read_memory(int address)
{
    return (Chip8.MEMORY_MAP[address] << 8 | Chip8.MEMORY_MAP[address + 1]);

}

void increase_pc(int value)
{
    Chip8.PC += value;
}

void decode(uint16_t instruction)
{
    //printf("Optcode %04x\n", instruction);
    Chip8.Optcode = (instruction & 0xF000) >> 12;
    Chip8.SecondNibble = (instruction & 0x0F00) >> 8;
    Chip8.ThirdNibble = (instruction & 0x00F0) >> 4;
    Chip8.FourthNibble = (instruction & 0x000F);
/*
    printf("\ninstruction: %04x, optcode: %01x, Second Nibble: %01x, Third Nibble: %01x, Fourth Nibble: %01x, address: %d\n", 
           instruction,
           Chip8.Optcode,
           Chip8.SecondNibble,
           Chip8.ThirdNibble,
           Chip8.FourthNibble,
           Chip8.PC);
*/
/*
    for(int i = 0; i < 16; i++)
    {
        printf("V_%d: %04x\n", i, Chip8.Vx[i]);
    }
*/
    switch(Chip8.Optcode)
    {
        case 0x0:
            if (Chip8.FourthNibble == 0)
            {
                clr();
            }
            else if(Chip8.FourthNibble == 0xE)
            {
                rtn();
            }
            break;
        case 0x1:
            jmp(instruction & 0x0FFF);
            break;
        case 0x2:
            call(instruction & 0x0FFF);
            break;
        case 0x3:
            skip_equal(instruction & 0x00FF);
            break;
        case 0x4: 
            skip_not_equal(instruction & 0x00FF);
            break;
        case 0x5:
            skip_xy();
            break;
        case 0x6:
            store_v_register(instruction & 0x00FF, Chip8.SecondNibble);
            break;
        case 0x7:
            add_v_register((instruction & 0x00FF), Chip8.SecondNibble);
            break;
        case 0x8:
            switch(Chip8.FourthNibble)
            {
                case 0x0:
                    store();
                    break;
                case 0x1:
                    x_or_y();
                    break;
                case 0x2:
                    x_and_y();
                    break; 
                case 0x3:
                    x_xor_y();
                    break;
                case 0x4:
                    x_add_y();
                    break;
                case 0x5:
                    x_sub_y();
                    break;
                case 0x6:
                    x_shr_y();
                    break;
                case 0x7:
                    x_subn_y();
                    break;                   
                case 0xE:
                    x_shl_y();
                    break;
                default:
                    printf("invalid instuction for 0x8: %04x", instruction);
                    exit(1);
            }
            break;
        case 0x9:
            sne();
            break;
        case 0xA:
            store_i_register(instruction & 0x0FFF);
            break;
        case 0xB:
            jmp_add_v0(instruction & 0x0FFF);
            break;
        case 0xC:
            random_inst(instruction & 0x00FF);
            break;
        case 0xD:
            draw();
            increase_pc(2);
            break;
        case 0xE:
            switch(instruction & 0x00FF)
            {
                case 0x9E:
                    skip_press();
                    break;
                case 0xA1:
                    skip_not_press();
                    break;
                default:
                    printf("Invalid Code For instrucntion 0xF: %04x", instruction & 0x00FF);
                    exit(1);
            }
            break;
        case 0xF:
            switch(instruction & 0x00FF)
            {
                case 0x9E:
                    skip_press();
                    break;
                case 0xA1:
                    skip_not_press();
                    break;
                case 0x07:
                    vx_delay_time();
                    break;
                case 0x0A:
                    
                    store_key();
                    break;
                case 0x15:
                    set_delay_timer();
                    break;
                case 0x18:
                    set_sound_timer();
                    break;
                case 0x1E:
                    add_i_vx();
                    break;
                case 0x29:
                    ld_f_vx();
                    break;
                case 0x33:
                    store_bcd();
                    break;
                case 0x55:
                    copy_vo_vx_memory();
                    break;
                case 0x65:
                    read_i_to_vo_vx();
                    break;
                default:
                    printf("Invalid Code For instrucntion 0xF: %04x", instruction & 0x00FF);
                    exit(1);
            }

            break;
        default:
            printf("Invalid optcode %04x\n", instruction);
            exit(1);
    }

}

