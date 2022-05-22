#ifndef instruction_file
#define instruction_file

    void clr(void);
    void rtn(void);
    void jmp(uint16_t address);
    void store_v_register(uint16_t value, uint8_t register_add);
    void store_i_register(uint16_t value);
    void add_v_register(uint16_t value, uint8_t register_add);
    void call(uint16_t value);
    void skip_equal(uint8_t address);
    void skip_not_equal(uint8_t address);
    void skip_xy(void);
    void store(void);
    void x_or_y(void);
    void x_and_y(void);
    void x_xor_y(void);
    void x_add_y(void);
    void x_sub_y(void);
    void x_shr_y(void);
    void x_subn_y(void);
    void x_shl_y(void);
    void sne(void);
    void jmp_add_v0(uint16_t address);
    void random_inst(uint8_t value);
    void vx_delay_time(void);
    void set_delay_timer(void);
    void set_sound_timer(void);
    void add_i_vx(void);
    void ld_f_vx(void);
    void copy_vo_vx_memory(void);
    void read_i_to_vo_vx(void);
    void store_bcd(void);
    void store_key(void);
    void skip_press(void);
    void skip_not_press(void);

    void unimplement(char *message);
#endif