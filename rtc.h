#include "Wire.h"
#define DS3231_I2C_ADDRESS 0x68
#define AT24C32_I2C_ADDRESS 0x57
#include "esp_task_wdt.h"
 


// si4703 i2c dirección es 0x10


// iniciar en el setup 
//Wire.begin();

byte decToBcd(byte val)
{
  return( (val/10*16) + (val%10) );
}
// Convert binary coded decimal to normal decimal numbers
byte bcdToDec(byte val)
{
  return( (val/16*10) + (val%16) );
}

void setDS3231time(byte second, byte minute, byte hour, byte dayOfWeek, byte
dayOfMonth, byte month, byte year){
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
  Wire.write(0); // set next input to start at the seconds register

  esp_task_wdt_reset();

  Wire.write(decToBcd(second)); // set seconds

  esp_task_wdt_reset();

  Wire.write(decToBcd(minute)); // set minutes
  Wire.write(decToBcd(hour)); // set hours

  esp_task_wdt_reset();

  Wire.write(decToBcd(dayOfWeek)); // set day of week (1=Sunday, 7=Saturday)
  esp_task_wdt_reset();
  Wire.write(decToBcd(dayOfMonth)); // set date (1 to 31)
  Wire.write(decToBcd(month)); // set month

  esp_task_wdt_reset();

  Wire.write(decToBcd(year)); // set year (0 to 99)
  Wire.endTransmission();


  esp_task_wdt_reset();

}

void readDS3231time(byte *second,
byte *minute,
byte *hour,
byte *dayOfWeek,
byte *dayOfMonth,
byte *month,
byte *year)
{
  Wire.beginTransmission(DS3231_I2C_ADDRESS);
 

  Wire.write(0); // set DS3231 register pointer to 00h
  Wire.endTransmission();
 

  Wire.requestFrom(DS3231_I2C_ADDRESS, 7);
  
  // request seven bytes of data from DS3231 starting from register 00h
  *second = bcdToDec(Wire.read() & 0x7f);

 

  *minute = bcdToDec(Wire.read());
  esp_task_wdt_reset();
  *hour = bcdToDec(Wire.read() & 0x3f);
  esp_task_wdt_reset();
  *dayOfWeek = bcdToDec(Wire.read());
 
  
  *dayOfMonth = bcdToDec(Wire.read());
  esp_task_wdt_reset();
  *month = bcdToDec(Wire.read());

  esp_task_wdt_reset();

  *year = bcdToDec(Wire.read());

  esp_task_wdt_reset();
  

}

