#ifndef display_file
#define display_file

    #include <stdint.h>
    #include <SDL.h>
    #include <stdbool.h>

    /* Display */
    #define Scale           15
    #define Screen_Height   32
    #define Screen_Width    64
    #define SizeFont        80
    #define NumberOfKey     16

    /* Prototype */
    void display_init(void);
    void destory_display(void);
    void draw(void);
    void print_terminal(void);
    int  keyboard();
    void update_keyboard(int key, bool value);

#endif