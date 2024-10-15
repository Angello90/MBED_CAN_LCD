#ifndef LCD_CAN_H
#define LCD_CAN_H

#include <stdint.h>
#include <typeinfo>
#include <mbed.h> 

#define LCD_CAN_10      0x700       
#define LCD_CAN_11      0x701      
#define LCD_CAN_21      0x702       
#define LCD_CAN_22      0x703     
#define LCD_CAN_CLS     0x77F      
#define CAN_SPEED       1000000   
#define CAN_H           PB_8
#define CAN_L           PB_9

typedef enum // Types of input %c, %d, %f
{
    CHAR_,
    INT_,
    FLOAT_
} types_input;

class LCD_CAN
{
private:
    char p_cursor_x;
    char p_cursor_y;
    char array_value[4][8];
    CANMessage array_msg[4];
    CANMessage cls_msg;
    CAN my_can(CAN_H, CAN_L, CAN_SPEED);

    types_input parse(char *str);
    void reset_array();
    void send_can();
    void init();
    void actualize_array();
    template<typename T>
    bool is_same_type(types_input type, T& value);

public:
    LCD_CAN();
    template <typename T>
    void print(char *str,  T& value);
    void clear();
    void setCursor(unsigned char &x, unsigned char &y);
    void setCursor();
};

#endif