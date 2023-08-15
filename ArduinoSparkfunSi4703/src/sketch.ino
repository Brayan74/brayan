#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>


Adafruit_PCD8544 display = Adafruit_PCD8544(18,23,4,2,15);
//Adafruit_PCD8544(int8_t sclk_pin, int8_t din_pin, int8_t dc_pin,
//                 int8_t cs_pin, int8_t rst_pin);
#define LCD_BL        13
#define LCD_BL_ON  digitalWrite(13, 1);
#define LCD_BL_OFF digitalWrite(13, 0);







void setup()
{
	
}

void loop()
{
	
}
