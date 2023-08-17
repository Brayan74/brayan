//-----------------------------------  keys ---------------------------------------------------------//

const float VERSION_CODE = 1.0;


String string_date_sistema = "";
String string_date_emergency = "";
String string_date_LCD ="";
String string_date_LCD_emergency ="";

String device_name = "SASPe_";

char  NUMBER_DEVICE_ID[30] ;

int NUMBER_DEVICE = 0;

 
const int buttonPin = 13;     // Pin to which the button is connected
int buttonState = HIGH;       // current state of the button
int lastButtonState = HIGH;   // previous state of the button
unsigned long startTime = 0;  // time in milliseconds of the falling edge
unsigned long endTime = 0;    // time in milliseconds of the risiing edge
bool timing = false;          // indicador to measure time