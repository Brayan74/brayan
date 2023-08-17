#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>

#include "logo.h"


Adafruit_PCD8544 display = Adafruit_PCD8544(18, 23, 4, 2, 15);//(CLK,DIN,DC,CE,RST)


const int pinLcd = 26;
const int rotationLcd = 0; //lcd blue

String lo_LCD = "Long : ";  
String la_LCD = "Lat : "; 
String rad_LCD = "R. Ac : ";  
String freq_LCD = "Frec : ";   
String rds_LCD = "Rds : ";
String time1 = ""; 
//---------------------- Constants ----------------------------//
String alertMsg = "----ALERTA----";
String eventMsg = "----EVENTO----";
String saspeMsg = "-----SASPe----";
String updateMsg = "-----UPDT-----";
String middleDashes = "--------------";