#include "lcd_i2c.h"

#define lcd_i2c_id 0b01001110		//set i2c id

void lcd_sendCommand(uint8_t command){
	uint8_t arrayCmd[4];
	uint8_t cmd_up, cmd_down;
	cmd_up = command & 0b11110000;
	cmd_down = (command & 0b00001111) << 4;
	arrayCmd[0] = cmd_up | 0b1100;
	arrayCmd[1] = cmd_up | 0b1000;
	arrayCmd[2] = cmd_down | 0b1100;
	arrayCmd[3] = cmd_down | 0b1000;
	HAL_I2C_Master_Transmit(&lcdi2c, lcd_i2c_id, (uint8_t *)arrayCmd, 4, 100);
	HAL_Delay(1);
}

void lcd_init(){
	//initialization
	HAL_Delay(60);
	HAL_I2C_Master_Transmit(&lcdi2c, lcd_i2c_id, (uint8_t *)0b00110000, 1, 100);	//function set
	HAL_Delay(10);
	HAL_I2C_Master_Transmit(&lcdi2c, lcd_i2c_id, (uint8_t *)0b00110000, 1, 100);	//function set
	HAL_Delay(10);
	HAL_I2C_Master_Transmit(&lcdi2c, lcd_i2c_id, (uint8_t *)0b00110000, 1, 100);	//function set
	HAL_Delay(10);
	HAL_I2C_Master_Transmit(&lcdi2c, lcd_i2c_id, (uint8_t *)0b00100000, 1, 100);	//4 bit mode
	HAL_Delay(10);

	//function set DL N F
	lcd_sendCommand(0b00101000);
	HAL_Delay(1);
	//display off
	lcd_sendCommand(0b00001000);
	HAL_Delay(1);
	//display clear
	lcd_sendCommand(0b00000001);
	HAL_Delay(1);
	//entry mode set
	lcd_sendCommand(0b00000110);
	HAL_Delay(1);
	//display turn on
	lcd_sendCommand(0b00001100);
	HAL_Delay(1);
	lcd_clear();
}

void lcd_setCursor(uint8_t col, uint8_t row){
	//1,add,add,add,add,add,add,add,0,0
	uint8_t data = 0b10000000;
	switch(row){
		case 0:
			data = data | (col + 0);
			break;
		case 1:
			data = data | (col + 64);
			break;
		case 2:
			data = data | (col + 20);
			break;
		case 3:
			data = data | (col + 84);
			break;
	}
	lcd_sendCommand(data);
	HAL_Delay(1);
}

void lcd_clear(){
	lcd_sendCommand(0b00000001);
	HAL_Delay(1);
}

void lcd_printChar(char character){
	uint8_t arrayChar[4];
	uint8_t chr, chr_up, chr_down;
	chr = character;
	chr_up = chr & 0b11110000;
	chr_down = (chr & 0b00001111) << 4;
	arrayChar[0] = chr_up | 0b1101;
	arrayChar[1] = chr_up | 0b1001;
	arrayChar[2] = chr_down | 0b1101;
	arrayChar[3] = chr_down | 0b1001;
	HAL_I2C_Master_Transmit(&lcdi2c,0b01001110, (uint8_t*) arrayChar, 4, 100);
	HAL_Delay(1);
}

void lcd_print(char *string){
	while(*string != 0x0){
		lcd_printChar(*string++);
	}
}

void lcd_clearString(uint8_t col_a,uint8_t col_b, uint8_t row){
	if(col_a >= 0 && col_b < 20 && col_a < col_b && row >= 0 && row < 4){
		lcd_setCursor(col_a,row);
		for(int i = col_a; i <= col_b; i++){
			lcd_print(" ");
		}
	}
}
