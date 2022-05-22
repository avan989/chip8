#include "cpu.h"
#include "instruction.h"
#include "stdio.h"
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* Optcode 00E0 */
void clr(void)
{
    for(int i = 0; i < Screen_Width * Screen_Height; i++)
    {
        Chip8.Screen[i] = 0;
    }

    increase_pc(2);
}

/* Optcode 00EE */
void rtn(void)
{
    Chip8.SP--;                         /* Decrement Stack Pointer */
    Chip8.PC = Chip8.Stack[Chip8.SP];   /* Set PC to previous Stack Pointer */
    increase_pc(2);
    
}

/* 1nnn */
void jmp(uint16_t address) 
{

    Chip8.PC = address;
    //printf("jump command: %04x", Chip8.PC);
}

/* 2nnn */
void call(uint16_t value)
{
    Chip8.Stack[Chip8.SP] = Chip8.PC;
    Chip8.SP++;
    Chip8.PC = value;

}

/* 3xkk */
void skip_equal(uint8_t address)
{
    increase_pc(2);
    if (Chip8.Vx[Chip8.SecondNibble] == address)
    {
        increase_pc(2);
    }
}

/* 4xkk */
void skip_not_equal(uint8_t address)
{
    increase_pc(2);
    if (Chip8.Vx[Chip8.SecondNibble] != address)
    {
        increase_pc(2);
    }
}

/* 5xy0 */
void skip_xy(void)
{
    increase_pc(2);    
    if(Chip8.Vx[Chip8.SecondNibble] == Chip8.Vx[Chip8.ThirdNibble])
    {
        increase_pc(2);
    }
}

/* 6xkk -> Put Value kk into register Vx */
void store_v_register(uint16_t value, uint8_t register_add)
{
    Chip8.Vx[Chip8.SecondNibble] = value;
    increase_pc(2);
} 

/* 7xkk */
void add_v_register(uint16_t value, uint8_t register_add)
{
    Chip8.Vx[Chip8.SecondNibble] += value;
    increase_pc(2);
}

/* 8xy0 */
void store(void)
{
    Chip8.Vx[Chip8.SecondNibble] = Chip8.Vx[Chip8.ThirdNibble];
    increase_pc(2);
}

/* 8xy1 */
void x_or_y(void)
{
    Chip8.Vx[Chip8.SecondNibble] |= Chip8.Vx[Chip8.ThirdNibble];
    //printf("Vx: %04x", Chip8.Vx[Chip8.SecondNibble]);
    increase_pc(2);
}

/* 8xy2 */
void x_and_y(void)
{
    Chip8.Vx[Chip8.SecondNibble] &= Chip8.Vx[Chip8.ThirdNibble];
    increase_pc(2);
}

/* 8xy3 */
void x_xor_y(void)
{
    Chip8.Vx[Chip8.SecondNibble] ^= Chip8.Vx[Chip8.ThirdNibble];
    increase_pc(2);

}

/* 8xy4 */
void x_add_y(void)
{
    uint16_t result;
    result = Chip8.Vx[Chip8.SecondNibble] + Chip8.Vx[Chip8.ThirdNibble];
    Chip8.Vx[Chip8.SecondNibble] = result;

    Chip8.Vx[0xF] = 0;
    if(result > 255)
    {
        Chip8.Vx[0xF] = 1;
    }

    increase_pc(2);
}

/* 8xy5 */
void x_sub_y(void)
{
    Chip8.Vx[0xF] = 0;
    if(Chip8.Vx[Chip8.SecondNibble] > Chip8.Vx[Chip8.ThirdNibble])
    {
        Chip8.Vx[0xF] = 1;
    }

    Chip8.Vx[Chip8.SecondNibble] -= Chip8.Vx[Chip8.ThirdNibble];
    increase_pc(2);

}

/* 8xy6 */
void x_shr_y(void)
{
    if((Chip8.Vx[Chip8.SecondNibble] & 0x0001) == 1)
    {
        Chip8.Vx[0xF]  = 1;
    }
    else
    {
        Chip8.Vx[0xF]  = 0;
    }

    Chip8.Vx[Chip8.SecondNibble] /= 2;
    increase_pc(2);
}

/* 8xy7 */
void x_subn_y(void)
{
    Chip8.Vx[0xF]  = 0;
    if(Chip8.Vx[Chip8.SecondNibble] < Chip8.Vx[Chip8.ThirdNibble])
    {
        Chip8.Vx[0xF]  = 1;
    }

    Chip8.Vx[Chip8.SecondNibble] = Chip8.Vx[Chip8.ThirdNibble] - Chip8.Vx[Chip8.SecondNibble];
    increase_pc(2);
}

/* 8xyE */
void x_shl_y(void)
{
    Chip8.Vx[0xF]  = 0;
    if((Chip8.Vx[Chip8.SecondNibble] & 0x80) > 0)
    {
        Chip8.Vx[0xF] = 1;
    }

    Chip8.Vx[Chip8.SecondNibble] *= 2;
    increase_pc(2);
    
}

/* 9xy0 */
void sne(void)
{
    increase_pc(2);
    if(Chip8.Vx[Chip8.SecondNibble] != Chip8.Vx[Chip8.ThirdNibble])
    {
        increase_pc(2);
    }

}

/* Annn */
void store_i_register(uint16_t value)
{
    Chip8.I = value;
    increase_pc(2);
}

/* Bnnn */
void jmp_add_v0(uint16_t address)
{
    Chip8.PC = address + Chip8.Vx[0];
}

/* Cxkk */
void random_inst(uint8_t value)
{
    uint8_t random_num = rand() % 256;

    Chip8.Vx[Chip8.SecondNibble] = random_num & value;
    increase_pc(2);
}

/* Ex9E */
void skip_press(void)
{
    increase_pc(2);
    if(Chip8.Keyboard[Chip8.Vx[Chip8.SecondNibble]] == true)
    {
        increase_pc(2);
    }
    //printf("Optcode: %04x, PC: %d\n", Chip8.Optcode, Chip8.PC );
}

/* ExA1 */
void skip_not_press(void)
{
    increase_pc(2);
    if(Chip8.Keyboard[Chip8.Vx[Chip8.SecondNibble]] != true)
    {
        increase_pc(2);
    }
    //printf("keyboard %d: %04x", Chip8.Vx[Chip8.SecondNibble], Chip8.Keyboard[Chip8.Vx[Chip8.SecondNibble]]);
    //printf("Optcode: %04x, PC: %d\n", Chip8.Optcode, Chip8.PC );
}

/* Fx07 */
void vx_delay_time(void)
{
    Chip8.Vx[Chip8.SecondNibble] = Chip8.DELAY;
    increase_pc(2);

}

/* Fx0A */
void store_key(void)
{
    for(int i =0; i < NumberOfKey; i++ )
    {
        if(Chip8.Keyboard[i] == true)
        {
            Chip8.Vx[Chip8.SecondNibble] = i;
            increase_pc(2);
        }
    }

}

/* Fx15 */
void set_delay_timer(void)
{
    Chip8.DELAY = Chip8.Vx[Chip8.SecondNibble];
    increase_pc(2);
} 

/* Fx18 */
void set_sound_timer(void)
{
    Chip8.SOUND_TIMER = Chip8.Vx[Chip8.SecondNibble];
    increase_pc(2);
}

/* Fx1E */
void add_i_vx(void)
{
    Chip8.I += Chip8.Vx[Chip8.SecondNibble];
    increase_pc(2);
}

/* Fx29 */
void ld_f_vx(void)
{
    /* Font is located in multiple of 5. 0 is address 0 - 4, 1 is 5 - 9 ..etc.
    ** Chip8.I should point to the beginning of each font. 
    */
    Chip8.I = Chip8.Vx[Chip8.SecondNibble] * 5; 
    increase_pc(2);
}

/* Fx33 */
void store_bcd(void)
{
    /* Store Decimal. 
    ** To Get Most Significant Digit. divide by 100: Example 255/100 => 2 (throw away decimal)
    ** To Get the 10s => 255/10 => 25 % 10 => 5
    ** To Get teh 100s => 255 % 100 => 55 % 10 => 5
    ** Another Example: 143
    ** 143/100 => 1
    ** 143 / 10 => 14 % 10 => 4
    ** 143 % 100 => 43 % 10 => 3
    */  
    Chip8.MEMORY_MAP[Chip8.I] = Chip8.Vx[Chip8.SecondNibble] / 100; /* Max Number = 255 for 8 bit integer. To  */
    Chip8.MEMORY_MAP[Chip8.I + 1] = (Chip8.Vx[Chip8.SecondNibble] / 10) % 10; 
    Chip8.MEMORY_MAP[Chip8.I + 2] = (Chip8.Vx[Chip8.SecondNibble] % 100) % 10;

    increase_pc(2);
}

/* Fx55 */
void copy_vo_vx_memory(void)
{
    for (int i = 0; i <= Chip8.SecondNibble; i++)
    {
        Chip8.MEMORY_MAP[Chip8.I + i] = Chip8.Vx[i];
    }

    increase_pc(2);
}

/* Fx65 */
void read_i_to_vo_vx(void)
{
    for(int i = 0; i <= Chip8.SecondNibble; i++)
    {
        Chip8.Vx[i] = Chip8.MEMORY_MAP[Chip8.I + i];
    }

    increase_pc(2);

}

void unimplement(char *message)
{
    printf("Not Implmeneted: %s", message);

}
