#include "lcd_can.h"

LCD_CAN::LCD_CAN()
{
    init();
}

void LCD_CAN::reset_array()
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            array_value[j][i] = 0x20; // ASCII code for space
        }
    }
}

void LCD_CAN::print(char *str, ...)
{
    va_list args;
    va_start(args, str);
    char buffer[2*16];
    vsnprintf(buffer, 2*16, str, args);
    va_end(args);

    for (int i = 0; i < strlen(buffer); i++)
    {
        if(p_cursor_y == 0)
        {
            if(i < 7) array_value[0][i + p_cursor_x] = buffer[i];
            else if(i < 16) array_value[1][i - 8 + p_cursor_x] = buffer[i];
        }
        else
        {
            pass
        }
    }
    send_can();
}

void LCD_CAN::setCursor(unsigned char &x, unsigned char &y)
{
    if (x > 15)
    {
        x = 0;
    }
    if (y > 1)
    {
        y = 0;
    }
    p_cursor_x = x;
    p_cursor_y = y;
}

void LCD_CAN::setCursor()
{
    p_cursor_x = 0;
    p_cursor_y = 0;
}

void LCD_CAN::actualize_array()
{
    for (int i = 0; i < 4; i++)
    {
        memccpy(array_msg[i].data, array_value[i], 1, 8);
    }
}

void LCD_CAN::init()
{
    array_msg[0].id = LCD_CAN_10;
    array_msg[1].id = LCD_CAN_11;
    array_msg[2].id = LCD_CAN_21;
    array_msg[3].id = LCD_CAN_22;

    for(int i = 0; i<4; i++)
    {
        array_msg[i].len = 8;
        array_msg[i].format = CANStandard;
    }
    reset_array();
    actualize_array();
    cls_msg.id = LCD_CAN_CLS;
    clear();
    setCursor();
}

void LCD_CAN::clear()
{
    my_can.write(cls_msg);
}

void LCD_CAN::send_can()
{
    actualize_array();
    for (int i = 0; i < 4; i++)
    {
        my_can.write(array_msg[i]);
    }
}