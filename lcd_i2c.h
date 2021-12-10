#include "stm32l1xx_hal.h"

I2C_HandleTypeDef lcdi2c;

void lcd_init(void);
void lcd_clear(void);
void lcd_print(char *stringa);
void lcd_clearString(uint8_t col, uint8_t col_b, uint8_t row);
void lcd_setCursor(uint8_t col, uint8_t row);
void lcd_sendCommand(uint8_t command);
void lcd_printChar(char character);
