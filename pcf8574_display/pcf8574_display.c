#include "pcf8574_display.h"

static uint8_t digits_on_line;
static uint8_t ten_dot_diget;
static uint8_t lines_on_display;
static uint8_t cur_line;

// Initialize lcd display for usage
// _digits_on_line - number of digits per line
// _ten_dot_diget - How many dots per digit
//		0 - 5x8DOTS 
//		1 - 5x10DOTS
// _lines_on_display - how many lines display have
void pcf8574_display_init(uint8_t _digits_on_line, uint8_t _ten_dot_diget, uint8_t _lines_on_display)
{	
	uint8_t f_set;
	
	digits_on_line = _digits_on_line;
	ten_dot_diget = _ten_dot_diget;
	lines_on_display = _lines_on_display;
	cur_line = 1;
	
	pcf8574_init(PCF8574A_MODULE_ADDR_W);
	
	// According to datasheet, we need at least 40ms after power rises above 2.7V
	_delay_ms(50);
	
	// Clear RS and RW lines 
	pcf8574_display_write_byte(0x00);
	_delay_ms(1000);
	
	// Set display to 4bit interface (according to Hitachi datasheet pg. 24, figure 24)
	pcf8574_display_init_4bit();
	
	f_set = LCD_FUNCTIONSET | LCD_1LINE | LCD_4BITMODE | LCD_5x8DOTS;
	
	if(ten_dot_diget)
		f_set = f_set | LCD_5x10DOTS;
	
	if(_lines_on_display > 1)
		f_set = f_set | LCD_2LINE;
	
	pcf8574_display_send_command(f_set);
	
	pcf8574_display_send_command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);
	pcf8574_display_clear();
	pcf8574_display_return();
}

// Initialize display for using 4bit interface 
// Returns 0 on success, otherwise i2c bus status code
uint8_t pcf8574_display_init_4bit()
{
	uint8_t error = 0;			// for some reason compiler think, that I\m not using this variable and optimize it away...So I've declared it as volatile 
	
	// send data to pcf8574
	if(!error)
		error = pcf8574_display_send(0x30);
	
	if(!error)
	{
		_delay_us(4500);	
		error = pcf8574_display_send(0x30);
	}
	
	if(!error)
	{		
		_delay_us(4500);
		error = pcf8574_display_send(0x30);
	}
	
	if(!error)
	{
		_delay_us(150);	
		
		// finally set interface to 4 bit
		error = pcf8574_display_send(0x20);
	}
	
	return error;
}

// Write string to display
// _str - string to write
// _len - length of the string
void pcf8574_display_write_string(char* _str, uint8_t _len)
{
	uint8_t i;
	
	for(i = 0; i < _len; i++)
	{
		if(i > (digits_on_line * lines_on_display))
			break;
		
		if(i >= digits_on_line && cur_line < 2)
			pcf8574_display_set_cursor(2, 0);
		
		pcf8574_display_send_data(_str[i]);
	}
}

// Set display cursor position
// _line_no - Line number
// _pos_in_line - Position in line (column number)
void pcf8574_display_set_cursor(uint8_t _line_no, uint8_t _pos_in_line)
{
	uint8_t addr_offset[] = {0x00, 0x40};	// DDRAM Address offset (by lines) according to Hitachi documentation pg. 29 (Set DDRAM Address)
	
	pcf8574_display_send_command(LCD_SETDDRAMADDR | addr_offset[_line_no - 1] + _pos_in_line);
	_delay_us(50);
	cur_line = _line_no;
}

// Clears display
void pcf8574_display_clear()
{
	pcf8574_display_send_command(LCD_CLEARDISPLAY);
	_delay_ms(2);
}

// Sets DDRAM address 0 in address counter. 
// Also returns display from being shifted to original position.
// DDRAM contents remain unchanged.
void pcf8574_display_return()
{
	pcf8574_display_send_command(LCD_RETURNHOME);
	_delay_ms(2);
	cur_line = 1;
}

// Send data to LCD display
// _data - data to send
void pcf8574_display_send_data(uint8_t _data)
{
	// send high nibble of data
	pcf8574_display_send((_data & 0xF0) | LCD_BACKLIGHT | LCD_RS_LINE);
	// send low nibble of data
	pcf8574_display_send((_data << 4) | LCD_BACKLIGHT | LCD_RS_LINE);
}

// Send command to LCD display
// _cmd - command to send
void pcf8574_display_send_command(uint8_t _cmd)
{
	// send high nibble of command
	pcf8574_display_send((_cmd & 0xF0) | LCD_BACKLIGHT);
	// send low nibble of command
	pcf8574_display_send((_cmd << 4) | LCD_BACKLIGHT);
}

// Send command or data to LCD display
// _data - data or command to send
uint8_t pcf8574_display_send(uint8_t _data)
{
	uint8_t error = 0;
	
	error = pcf8574_display_write_byte(_data);
	
	if(!error)
		error = pcf8574_display_pulse(_data);
	
	return error;
}

// Set and clear LCD Display EN Line 
uint8_t pcf8574_display_pulse(uint8_t _data)
{
	uint8_t error = 0;	
	// send pulse
	error = pcf8574_display_write_byte(_data | LCD_EN_LINE);
	_delay_us(1);
	error = pcf8574_display_write_byte(_data & ~LCD_EN_LINE);
	_delay_us(50);
	
	return error;
}

// Write 2 bytes to I2C bus
// _data - data to write to i2c bus
uint8_t pcf8574_display_write_byte(uint8_t _data)
{
	uint8_t error = 0;			// for some reason compiler think, that I\m not using this variable and optimize it away...So I've declared it as volatile 
	uint8_t status;
	
	// send start condition to i2c bus
	error = pcf8574_write_start_cnd(&status);
	
	// send pcf8574 write address
	if(!error)
		error = pcf8574_write_addr(&status);
	
	// send data to pcf8574
	if(!error)
		error = pcf8574_write_data(_data, &status);
	
	twi_send_stop_condition();
	
	return !error ? 0 : status;
}