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

types_input LCD_CAN::parse(char *str)
{
    types_input type;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '%')
        {
            if (str[i + 1] == 'c')
            {
                type = CHAR_;
            }
            else if (str[i + 1] == 'd')
            {
                type = INT_;
            }
            else if (str[i + 1] == 'f')
            {
                type = FLOAT_;
            }
        }
    }
    return type;
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

template <typename T>
bool is_same_type(types_input type, T &value)
{
    bool same_type = false;
    if ((type == CHAR_) && (typeid(value).name() == char))
        same_type = true;
    else if ((type == INT_) && (typeid(value).name() == int))
        same_type = true;
    else if ((type == FLOAT_) && (typeid(value).name() == float))
        same_type = true;
    return same_type;
}

template <typename T>
void LCD_CAN::print(char *str, T &value)
{
    types_input type = parse(str);
    if (is_same_type(type, value))
    {
        for (int i = 0; i < strlen(str); i++)
        {
            if (p_cursor_y == 0)
            {
                if (i < 7) array_value[0][i + p_cursor_x] = str[i];
                else array_value[1][i - 8 + p_cursor_x] = str[i];
            }
            else if (p_cursor_y == 1)
            {
                if (i < 7) array_value[2][i + p_cursor_x] = str[i];
                else array_value[3][i + p_cursor_x - 8] = str[i];
            }
        }
        send_can();
    }
    else
    {
        // Error
    }
}

void LCD_CAN::actualize_array()
{
    array_msg[0].id = LCD_CAN_10;
    array_msg[1].id = LCD_CAN_11;
    array_msg[2].id = LCD_CAN_21;
    array_msg[3].id = LCD_CAN_22;

    for (int i = 0; i < 4; i++)
    {
        memcpy(array_msg[i].data[0], array_value[i], 1, 8);
        array_msg[i].len = 8;
        array_msg[i].format = CANStandard;
    }
}

void LCD_CAN::init()
{
    reset_array();
    actualize_array();
    p_cursor_x = 0;
    p_cursor_y = 0;
    cls_msg.id = LCD_CAN_CLS;
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