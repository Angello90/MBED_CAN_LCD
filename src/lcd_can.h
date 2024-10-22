#ifndef LCD_CAN_H
#define LCD_CAN_H

#include "mbed.h"
#include <stdint.h>
#include <typeinfo> 
#include <stdarg.h>
#include <string.h>


#define LCD_CAN_10      0x700       
#define LCD_CAN_11      0x701      
#define LCD_CAN_21      0x702       
#define LCD_CAN_22      0x703     
#define LCD_CAN_CLS     0x77F      
#define CAN_SPEED       1000000   
#define CAN_H           PB_0
#define CAN_L           PB_1


extern CAN my_can;

class LCD_CAN
{
private:
    char p_cursor_x;
    char p_cursor_y;
    // possible de le faire passer en tableau de 2 dimensions
    // char array_value[2][16];
    // CANMessage array_msg[2];
    char array_value[4][8];
    CANMessage array_msg[4];
    CANMessage cls_msg;

    void reset_array();
    void send_can();
    void init();
    void actualize_array();

public:
    LCD_CAN();
    void print(char *str, ...);
    void clear();
    // Si on ne change pas entre 2 écritures, les parametres sont les mêmes
    void setCursor(unsigned char &x, unsigned char &y);
    void setCursor();
};

#endif