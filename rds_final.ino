//***********************************************************//
//****************************Library************************//
//***********************************************************//

//Nokia 5110 Display
#include <Arduino.h>
 
#include <WiFi.h>
#include <SparkFunSi4703.h>
#include <SPI.h>
#include <Wire.h>
#include "audio1.h"
#include "audio2.h"
#include "myfunction.h"
#include "identifier_pty.h"
#include "identifier_type.h"
#include "bluetooth.h"

#include "esp_task_wdt.h"
#include "logo.h"
#include "rtc.h"
#include "lcd_file.h"

#include "eeprom.h"
#include "keys.h"
 
#define INTERRUPT_PIN 16
// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif
// #if !defined(CONFIG_BT_SPP_ENABLED)
// #error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
// #endif


//--------------------------------- Define constanst ---------------------------------

#define WDT_TIMEOUT 60

SemaphoreHandle_t spiMutex;


volatile char display_STATUS_GLOBAL = 'N';


#include "ota_file.h"


const unsigned char bitmap_image [] PROGMEM ={
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xfe, 
  0x07, 0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x04, 0x01, 0xff, 0xff, 0xff, 
  0xf0, 0xff, 0xff, 0xff, 0x88, 0x00, 0x0c, 0x00, 0x1f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfc, 0x08, 
  0x00, 0x18, 0x00, 0x0f, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xf0, 0x18, 0x00, 0x18, 0x00, 0x18, 0x7f, 
  0xff, 0xf0, 0xff, 0xff, 0x80, 0x18, 0x00, 0x10, 0x00, 0x10, 0x1f, 0xff, 0xf0, 0xff, 0xfe, 0x00, 
  0x18, 0x00, 0x10, 0x00, 0x30, 0x03, 0xff, 0xf0, 0xff, 0xfc, 0x00, 0x18, 0x00, 0x10, 0x00, 0x30, 
  0x01, 0xff, 0xf0, 0xff, 0xf0, 0x00, 0x18, 0x00, 0x10, 0x00, 0x10, 0x00, 0x7f, 0xf0, 0xff, 0xe0, 
  0x00, 0x18, 0x00, 0x10, 0x00, 0x18, 0x00, 0x3f, 0xf0, 0xff, 0xff, 0x80, 0x18, 0x00, 0x18, 0x00, 
  0x0c, 0x00, 0x1f, 0xf0, 0xff, 0x00, 0x3e, 0x08, 0x00, 0x08, 0x00, 0x06, 0x00, 0x0f, 0xf0, 0xfe, 
  0x00, 0x00, 0xfc, 0x00, 0x0c, 0x00, 0x01, 0xc0, 0x77, 0xf0, 0xfe, 0x00, 0x00, 0x07, 0x00, 0x06, 
  0x00, 0x00, 0x3f, 0x83, 0xf0, 0xfc, 0x00, 0x00, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x01, 0xf0, 
  0xf8, 0x00, 0x00, 0x00, 0x1c, 0x01, 0x80, 0x00, 0x00, 0x00, 0xf0, 0xf8, 0x00, 0x00, 0x00, 0x03, 
  0x00, 0xc0, 0x00, 0x00, 0x00, 0xf0, 0xf0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x20, 0x00, 0x00, 0x00, 
  0x70, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x30, 0x1c, 0x00, 0x00, 0x00, 0x70, 0xf3, 0xff, 0x00, 0x00, 
  0x00, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x70, 0xe0, 0x00, 0xf8, 0x00, 0x00, 0x03, 0x00, 0xe0, 0x00, 
  0x00, 0xf0, 0xe0, 0x00, 0x07, 0x00, 0x00, 0x00, 0xc0, 0x1f, 0xc0, 0x2f, 0x70, 0xe0, 0x00, 0x00, 
  0x70, 0x00, 0x00, 0x60, 0x00, 0x7f, 0xc0, 0x30, 0xe0, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x18, 0x00, 
  0x00, 0x00, 0x30, 0xe0, 0x00, 0x00, 0x03, 0x80, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x30, 0xe0, 0x00, 
  0x00, 0x00, 0xe0, 0x00, 0x06, 0x00, 0x00, 0x00, 0x70, 0xf0, 0x00, 0x00, 0x00, 0x30, 0x00, 0x01, 
  0x00, 0x00, 0x00, 0x70, 0xfd, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x80, 0x00, 0x00, 0x70, 0xf8, 
  0xfc, 0x00, 0x00, 0x02, 0x00, 0x00, 0x40, 0x00, 0x00, 0xf0, 0xf8, 0x03, 0xc0, 0x00, 0x01, 0x80, 
  0x00, 0x20, 0x00, 0x00, 0xf0, 0xf8, 0x00, 0x38, 0x00, 0x00, 0xc0, 0x00, 0x3f, 0x80, 0x3f, 0xf0, 
  0xfc, 0x00, 0x0e, 0x00, 0x00, 0x20, 0x00, 0x18, 0xff, 0x83, 0xf0, 0xfe, 0x00, 0x01, 0x80, 0x00, 
  0x10, 0x00, 0x08, 0x00, 0x03, 0xf0, 0xff, 0x00, 0x00, 0xc0, 0x00, 0x08, 0x00, 0x04, 0x00, 0x07, 
  0xf0, 0xff, 0x80, 0x00, 0x30, 0x00, 0x0c, 0x00, 0x06, 0x00, 0x0f, 0xf0, 0xff, 0xc0, 0x00, 0x08, 
  0x00, 0x06, 0x00, 0x02, 0x00, 0x1f, 0xf0, 0xff, 0xe0, 0x00, 0x04, 0x00, 0x02, 0x00, 0x03, 0x00, 
  0x3f, 0xf0, 0xff, 0xf8, 0x00, 0x02, 0x00, 0x03, 0x00, 0x03, 0x00, 0xff, 0xf0, 0xff, 0xfc, 0x00, 
  0x03, 0x00, 0x01, 0x00, 0x01, 0x01, 0xff, 0xf0, 0xff, 0xff, 0x00, 0x01, 0x80, 0x01, 0x80, 0x01, 
  0x87, 0xff, 0xf0, 0xff, 0xff, 0xc0, 0x00, 0x80, 0x00, 0x80, 0x01, 0x9f, 0xff, 0xf0, 0xff, 0xff, 
  0xf0, 0x00, 0xc0, 0x00, 0x80, 0x00, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xfe, 0x00, 0xc0, 0x00, 0xc0, 
  0x03, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xe0, 0x40, 0x00, 0xc0, 0x3f, 0xff, 0xff, 0xf0, 0xff, 
  0xff, 0xff, 0xff, 0xc0, 0x00, 0xc7, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
  0xff, 0xff, 0xff, 0xff, 0xf0, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0
};


//*************************************************************************************
//----------------------------------- Constants --------------------------------------
//*************************************************************************************

//------------------------------------   LCD -------------------------------------------
 
int ledState = LOW; 
 

//------------------------------------ Sirena ------------------------------------------
volatile unsigned long time0_ALARM = 0;
volatile unsigned long time0_update = 0;
//sirena
//#include "audio.h"
 
const int pinAudio2 = 14; 
int freq2 = 2200;
int channel2 = 0;
int resolution2 = 8;

float latitud_emergency = 0;
float radio_emergency = 0 ;
float longitud_emergency =0;

//---------------------------------- Bluetooth -----------------------------------------



//-----------------Semaforos
SemaphoreHandle_t i2cMutex;

//---------------------------------  Radio RDS ------------------------------------------

Si4703_Breakout radio(5, 21, 22, 19); //i2c (RSTPin, SDIO, SCLK, SCT)
const int GPIO1_Si = 17;
const int GPIO2_Si = 16;

//Blocks for read blocks rds
uint16_t blocks[4];

//set 0 as volume in production 
const int volumen = 15;
 
//customize
process_read process_rds;

//----------------------------------- Relay ---------------------------------
const int pinRelay = 27;
bool STATUS_RELAY = false;
// ----------------------------- Tasks for core ----------------------------//
// TaskHandle_t Task1;
// TaskHandle_t Task2;
//---------------------- Tasks for secundary code ---------------------//
 
TaskHandle_t Task_for_sirena;
TaskHandle_t Task_for_RTC;
TaskHandle_t Task_for_BT;
TaskHandle_t Task_for_RDS;
 
TaskHandle_t Task_for_INT;
TaskHandle_t Task_for_display;
TaskHandle_t Task_for_OTA;


//------------------------------ Parametros SASPe ---------------------------------//

float latitud_center = -12.77;
float longitud_center = -76.3;
float FREQ_RDS = 101.7;
String departamento = "Lima";
String provincia = "Lima Met.";


//kilometers
const float dlat = 110.57;
const float dlon = 111.32;

//------------------------------- General functions ----------------------------------

void RDS_newfrequency  (float frequency)
{
  radio.setChannel(frequency);
  esp_task_wdt_reset();

  process_rds.reset();
  esp_task_wdt_reset();

  FREQ_RDS = float(frequency/10);

  // guardamos en la eeprom
  // llamamos semaforos
  xSemaphoreTake(i2cMutex, portMAX_DELAY);
  //frecuencia
  EepromRTC.writeFloat(13,FREQ_RDS); 
  xSemaphoreGive(i2cMutex);


}
 


double vabs(double value)
{
  if (value<0) return value*-1;

}
bool check_event(double R_, double lat_, double lon_){  
  Serial.println("Diferencias");
  Serial.println(abs(latitud_center-lat_));


  double diff_lat = abs(latitud_center-lat_)*110.57;
  double diff_lon = abs(longitud_center-lon_)*111.32;

  Serial.println("part1");
  double diff = sqrt(((double)pow(diff_lat,2))+((double)pow(diff_lon,2)));
  Serial.println(diff_lat);
  Serial.println(diff_lon);

  Serial.println("Distancia entre puntos: ");
  Serial.println(diff);
  if(diff<=R_) {return true;}
  else {return false;}
} 


 
 

//*************************************************************************************
//------------------------------ Interrupciones ----------------------------------------
//*************************************************************************************

void IRAM_ATTR my_interruption()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(Task_for_RDS, 0, eNoAction, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }
}

void IRAM_ATTR button_interrupt()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    xTaskNotifyFromISR(Task_for_INT, 0, eNoAction, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken == pdTRUE) {
        portYIELD_FROM_ISR();
    }


}


void processINT_Function(void *pvParameters)
{
  while(1)
  {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    startTime  = millis();

    bool option1 = true;
    bool option2 = true;
    bool option3 = true;

    Serial.println("Boton activado");

    buttonState = digitalRead(buttonPin);

    while (buttonState == LOW)
    { 
      buttonState = digitalRead(buttonPin);
      
      if (((millis()-startTime)>2.5*1000) && option1) 
      {
        //apagamos la alarma en caso esté prendido
        Serial.println("Primer caso");

        option1 = false;
        time0_ALARM = 0;
        vTaskSuspend(Task_for_sirena);

      }

      if (((millis()-startTime)>6.5*1000) && option2) 
      {
        //apagamos el BT 
        Serial.println("Segundo caso");

        option2 = false;
        //emitimos un pitido 


      // primer pitido
        freq2=440;ledcWrite(channel2,   freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=880;ledcWrite(channel2,   freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=1320;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=2200;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=2500;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));
      }

      if (((millis()-startTime)>10*1000) && option3) 
      {
        //formateamos el ESP32
        option3 = false;

        //emitimos dos pitidos
        
      // primer pitido
        freq2=440;ledcWrite(channel2,   freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=880;ledcWrite(channel2,   freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=1320;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=2200;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=2500;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));

        vTaskDelay(pdMS_TO_TICKS(350));

      // segundo pitido
        freq2=440;ledcWrite(channel2,   freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=880;ledcWrite(channel2,   freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=1320;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=2200;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));
        freq2=2500;ledcWrite(channel2,  freq2);vTaskDelay(pdMS_TO_TICKS(100));

        //salimos porque no hay más opciones;
        break;



      }

      vTaskDelay(pdMS_TO_TICKS(25));

    

    
    }




  }
}

void processRDS_Function(void *pvParameters) {
  while(1)
  {
    //Ejecución de la interrupción
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // notificación desbloqueada
    xSemaphoreTake(i2cMutex, portMAX_DELAY);
    radio.getBlocksInterrupt(blocks);
 
    xSemaphoreGive(i2cMutex);

    process_rds.set_block(blocks);
      

    // verificamos si hay emergencia
    if(process_rds.EMERGENCY_SIGNAL)
    { 
 


      latitud_emergency   = process_rds.latitud_emergency;
      radio_emergency     = process_rds.radio_emergency;
      longitud_emergency  = process_rds.longitud_emergency;

      bool flag           = check_event(radio_emergency,latitud_emergency, longitud_emergency);
      
      string_date_emergency = string_date_sistema;

      process_rds.EMERGENCY_SIGNAL = false;

      if (flag)
      {
        //--------------------- Nos encontramos dentro del radio de acción -----------------------------------------//
        // activamos alarma
        time0_ALARM = millis();
        vTaskResume(Task_for_sirena);
 
        display_STATUS_GLOBAL = 'E'; 
        //actualizamos LCD
      
      }
      else
      { 
        //Solo se displaya el LCD 
        time0_ALARM = millis();
        vTaskSuspend(Task_for_sirena);
        display_STATUS_GLOBAL = 'E';
      }
      
    }

  }
}





//*************************************************************************************
//----------------------------------- Core 1  ----------------------------------------
//*************************************************************************************




//----------------------------------- Functions Core 1  ----------------------------------------
volatile long time0_RELE = 0;

void Task_for_rele_function(void* parameter)
{
  //define your code for rele

  while(1)
  {
    if (!(millis() - time0_RELE > 1000*60))
    {
      digitalWrite(pinRelay, HIGH);
      esp_task_wdt_reset();
      delay(10);
    }
    else
    {
      digitalWrite(pinRelay, LOW);
      esp_task_wdt_reset();
      delay(10);

      //suspendemos la tarea
      //vTaskSuspend(Task_for_rele);

    }
  }
}


void Task_for_sirena_function(void* parameter)
{
   // define your code for sirena
  // ACTIVAMOS EL RELÉ
  // probamos la sirena por un minuto 
  
 

  
  while(1)
  {
 
  if (!(millis() - time0_ALARM > 1000*60))
  { 
    digitalWrite(pinRelay, LOW);
    digitalWrite(pinLcd, HIGH);


    display_STATUS_GLOBAL = 'E';

    freq2=440;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    freq2=880;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));
    digitalWrite(pinLcd, LOW);

    freq2=1320;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    freq2=2200;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));
    digitalWrite(pinLcd, HIGH);


    freq2=2500;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));

  }
  else
  { 
    display_STATUS_GLOBAL = 'N';
    digitalWrite(pinRelay, HIGH);
    digitalWrite(pinLcd,LOW);

    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10));
    // suspendemos la tarea
    vTaskSuspend(NULL);
  }
}
}

void Task_for_display_function(void* parameter)
{
 
  //presentacion 
  //logo 
  xSemaphoreTake(spiMutex, portMAX_DELAY);
  //----------------------------------------------

  {
  
  //logo
  display.clearDisplay();
  display.drawBitmap(0, 0, bitmap_image, 84, 48, 1);
  display.display();
 
  //---------------------------------------------------------
  xSemaphoreGive(spiMutex);
  }
  vTaskDelay(pdMS_TO_TICKS(1500));
  //------------ Presentacion SASPe ----------------------//
  xSemaphoreTake(spiMutex, portMAX_DELAY);

  
  {
  display.clearDisplay();
  display.setRotation(rotationLcd);
  display.setTextSize(1);
  display.setTextColor(BLACK); 
  display.setCursor(0,0);
  display.println("");
  display.setCursor(0,10);
  display.println(middleDashes.c_str());
  display.setCursor(0,20);
  display.println(saspeMsg.c_str());
  display.setCursor(0,30);
  display.println(middleDashes.c_str());
  display.setCursor(0,40);
  display.println("");
  display.display();
  

  vTaskDelay(pdMS_TO_TICKS(1000));


  }
  
  xSemaphoreGive(spiMutex);

  while(1)
  {
    xSemaphoreTake(spiMutex, portMAX_DELAY);
    //-----------------------------------------------------------------------------------//
    if (display_STATUS_GLOBAL == 'N')
    {
      // cargamos pantalla normal;
      
      String o_t_msg = saspeMsg;
      String o_t_time = string_date_LCD;  
      String o_t_rds = rds_LCD + String(round(process_rds.signal_RDS))+"%"; 
      String o_t_freq = freq_LCD + String(FREQ_RDS);  
      String o_t_lo = lo_LCD + String(longitud_center);
      String o_t_la = la_LCD + String(latitud_center); 

      display.clearDisplay();
      display.setRotation(rotationLcd);
      display.setTextSize(1);
      display.setTextColor(BLACK); 
      display.setCursor(0,0);
      display.println(o_t_msg.c_str());
      display.setCursor(0,8);
      display.println(o_t_time.c_str());
      display.setCursor(0,16);
      display.println(o_t_rds.c_str());
      display.setCursor(0,24);
      display.println(o_t_freq.c_str());
      display.setCursor(0,32); 
      display.println(o_t_lo.c_str());
      display.setCursor(0,40);
      display.println(o_t_la.c_str());
      display.display();
   
    }
    if (display_STATUS_GLOBAL == 'U')
    {
      if ((millis() - time0_update > 1000*90))
      {
        //Seteamos a operación normal cuando se acabe el evento
        display_STATUS_GLOBAL = 'N';
        time0_update = 0;
        digitalWrite(pinLcd,LOW);
      }
      digitalWrite(pinLcd,HIGH);

      String o_t_msg = updateMsg;
      String o_t_time = string_date_LCD;  
      String o_t_rds = rds_LCD + String(round(process_rds.signal_RDS))+"%"; 
      String o_t_freq = freq_LCD + String(FREQ_RDS);  
      String o_t_lo = lo_LCD + String(longitud_center);
      String o_t_la = la_LCD + String(latitud_center); 

      display.clearDisplay();
      display.setRotation(rotationLcd);
      display.setTextSize(1);
      display.setTextColor(BLACK); 
      display.setCursor(0,0);
      display.println(o_t_msg.c_str());
      display.setCursor(0,8);
      display.println(o_t_time.c_str());
      display.setCursor(0,16);
      display.println(o_t_rds.c_str());
      display.setCursor(0,24);
      display.println(o_t_freq.c_str());
      display.setCursor(0,32); 
      display.println(o_t_lo.c_str());
      display.setCursor(0,40);
      display.println(o_t_la.c_str());
      display.display();
      // cargamos pantalla UPDATE;
    }
    if (display_STATUS_GLOBAL == 'V')
    {
      //LCD Event
      if ((millis() - time0_ALARM > 1000*60))
      {
        //Seteamos a operación normal cuando se acabe el evento
        display_STATUS_GLOBAL = 'N';
      }
        String e_t_msg = alertMsg;
        String e_t_time = string_date_LCD_emergency; 
        String e_t_lo = lo_LCD + String(longitud_emergency);
        String e_t_la = la_LCD + String(latitud_emergency); 
        String e_t_alertRad = rad_LCD + String(radio_emergency); 


        display.clearDisplay();
        display.setRotation(rotationLcd);
        display.setTextSize(1);

        display.setTextColor(BLACK); 
        display.setCursor(0,0);
        display.println(e_t_msg.c_str());
        display.setCursor(0,8); 
        display.println("");
        display.setCursor(0,16);
        display.println(e_t_time.c_str());
        display.setCursor(0,24); 
        display.println(e_t_lo.c_str());
        display.setCursor(0,32); 
        display.println(e_t_la.c_str());
        display.setCursor(0,40);
        display.println(e_t_alertRad.c_str());
        display.display();


        

    }

   
   
    
    if ((display_STATUS_GLOBAL == 'E') )
    {
      //emergencia
      if ((millis() - time0_ALARM > 1000*60))
      {
        //Seteamos a operación normal cuando se acabe el evento
        display_STATUS_GLOBAL = 'N';
      }
        String e_t_msg = eventMsg;
        String e_t_time = string_date_LCD_emergency; 
        String e_t_lo = lo_LCD + String(longitud_emergency);
        String e_t_la = la_LCD + String(latitud_emergency); 
        String e_t_alertRad = rad_LCD + String(radio_emergency); 

      

        display.clearDisplay();
        display.setRotation(rotationLcd);
        display.setTextSize(1);

        display.setTextColor(BLACK); 
        display.setCursor(0,0);
        display.println(e_t_msg.c_str());
        display.setCursor(0,8); 
        display.println("");
        display.setCursor(0,16);
        display.println(e_t_time.c_str());
        display.setCursor(0,24); 
        display.println(e_t_lo.c_str());
        display.setCursor(0,32); 
        display.println(e_t_la.c_str());
        display.setCursor(0,40);
        display.println(e_t_alertRad.c_str());
        display.display();
      
    }
    //----------------------------------------------------------------------------//
    xSemaphoreGive(spiMutex);
  
    vTaskDelay(pdMS_TO_TICKS(1000*0.8));
 
 
      
  }

}
  



//*************************************************************************************
//----------------------------------- Core 2  ----------------------------------------
//*************************************************************************************


//----------------------------------- Functions Core 2  ----------------------------------------
void processBT_Function(void * parameter){

 

  while(1)
  { 
    // process_rds.check_timeout(false);
 
    if(SerialBT.available())
    {

      bool flag_send = true;
      String chain_send = "";
      //------------------- check timeout ---------------------------------
      
      //------------------------- process-----------------------//

      String data = bluetoothReadLine();

      //----------------------- login -------------------------
      if (data.indexOf("login")!=-1)
      {
        writeSerialBT("CONFIGURED");
      }

      //----------------------- text send----------------------------
      else if (data.indexOf("sim_test")!=-1)
      {
        int firstDelimiter  = data.indexOf('|');
        int secondDelimiter = data.indexOf('|',firstDelimiter+1);
        int thirdDelimiter = data.indexOf('|',secondDelimiter+1);
        int fourthDelimiter = data.indexOf('|',thirdDelimiter+1);
        
        String lat_ = data.substring(firstDelimiter+1,secondDelimiter);
        String lon_ = data.substring(secondDelimiter+1,thirdDelimiter);
        String rad_ = data.substring(thirdDelimiter+1,fourthDelimiter);

        process_rds.insert_test((float)atof(lat_.c_str()),(float)atof(lon_.c_str()),(float)atof(rad_.c_str()));
 


        // verificamos emergencia

        // verificamos si hay emergencia
        if(process_rds.EMERGENCY_SIGNAL)
        { 
          latitud_emergency = process_rds.latitud_emergency;
          radio_emergency =process_rds.radio_emergency;
          longitud_emergency =process_rds.longitud_emergency;

          bool flag = check_event(radio_emergency,latitud_emergency, longitud_emergency);
          
          string_date_emergency = string_date_sistema;
          string_date_LCD_emergency = string_date_LCD;

          process_rds.EMERGENCY_SIGNAL = false;

          if (flag)
          {
            //--------------------- Nos encontramos dentro del radio de acción ----------------------------------------------
            // activamos alarma
            time0_ALARM = millis();
            vTaskResume(Task_for_sirena);
    
            display_STATUS_GLOBAL = 'E';
            //actualizamos LCD
          
          }
          else
          {
            time0_ALARM = millis();


            display_STATUS_GLOBAL = 'E';
            vTaskSuspend(Task_for_sirena);

          }
          
        }
        
      }

      else if (data.indexOf("rds_test")!=-1)
      {
        String chain_send = data_RDSTest_generator("rds_test");
        writeSerialBT(chain_send);
      }

      else if (data.indexOf("latlon_test")!=-1)
      {
        String chain_send = data_latlonTest_generator();
        writeSerialBT(chain_send);
      }
      else if (data.indexOf("latencia_test")!=-1)
      {
        String chain_send = data_latenciaTest_generator();
        writeSerialBT(chain_send);
      }
      else if (data.indexOf("info_test")!=-1)
      {
        String chain_send = data_infoTest_generator();
        writeSerialBT(chain_send);
      }
      // ------------------------- comandos de cambio-----------------
      /*
      Se piensa implementar un cambio de hora
      */
      else if (data.indexOf("new_frequency|")!=-1)
      {

        int firstDelimiter  = data.indexOf('|');


        String value = data.substring(firstDelimiter+1);
 

        RDS_newfrequency(value.toFloat());


      }

      else if (data.indexOf("new_latlon")!=-1)
      {

        int firstDelimiter  = data.indexOf('|');
        int secondDelimiter = data.indexOf('|',firstDelimiter+1);
        int thirdDelimiter = data.indexOf('|',secondDelimiter+1);
        
        String lat = data.substring(firstDelimiter+1,secondDelimiter);
        String lon = data.substring(secondDelimiter+1,thirdDelimiter);


 

        latitud_center = atof(lat.c_str());
        longitud_center = atof(lon.c_str());


        // guardamos en la eeprom
        xSemaphoreTake(i2cMutex, portMAX_DELAY);
        //frecuencia
        EepromRTC.writeFloat(1,latitud_center); 
        EepromRTC.writeFloat(5,longitud_center); 
        xSemaphoreGive(i2cMutex);
 
        
 
      }



      //------------------------ comandos de test LCD, sirena y RELÉ -------------
      else if (data.indexOf("ALARM_test_ON")!=-1)
      {
        time0_ALARM = millis();
        vTaskResume(Task_for_sirena);
        
        flag_send = false;

      }
      else if (data.indexOf("ALARM_test_OFF")!=-1)
      {
        time0_ALARM = 0;
        vTaskSuspend(Task_for_sirena);
        display_STATUS_GLOBAL = 'N';
        digitalWrite(pinRelay, HIGH);
        flag_send = false;
      }      

      else if (data.indexOf("RELE_test_ON")!=-1)
      {
        // vTaskResume(Task_for_rele);
        digitalWrite(pinRelay, LOW);
        flag_send = false;
      }

      else if (data.indexOf("RELE_test_OFF")!=-1)
      {
        // 
 
        digitalWrite(pinRelay, HIGH);
        
        flag_send = false;
      }

      else if (data.indexOf("update")!=-1)
      {
        //actualizamos por OTA
        display_STATUS_GLOBAL =  'U';
        time0_update = millis();
        //deshabilitamos todas las tareas
        // pausamos todas las tareas 

        vTaskDelay(pdMS_TO_TICKS(50));

        vTaskSuspend(Task_for_sirena);
        vTaskSuspend(Task_for_RDS);
        vTaskSuspend(Task_for_display);
        vTaskSuspend(Task_for_RTC);

        vTaskDelay(pdMS_TO_TICKS(200));
        //apagamos el BT
        SerialBT.end();
        // desactivamos el bit de interrupcion
        detachInterrupt(GPIO2_Si);

        xTaskNotifyGive(Task_for_OTA);
        vTaskSuspend(NULL);

      }

      else if (data.indexOf("version")!=-1)
      {
        writeSerialBT(String(VERSION_CODE));
      }
      else 
      {
        flag_send = false;
      }

  }
  esp_task_wdt_reset();
  vTaskDelay(pdMS_TO_TICKS(5));
}
}



String data_infoTest_generator(void)
{ 
  //----------------------------- getting date-------------------------------



  //------------------------------ making chain --------------------------------

  String chain = "info_test";

  chain += "|" + string_date_sistema + "|" + departamento + ", " + provincia + "|" + String(latitud_center)+ "|" + String(longitud_center) + "|" + String(process_rds.signal_RDS) + "|" + String(FREQ_RDS);

  return chain;
}





String data_RDSTest_generator(String init)
{
  String chain_send=init;
  if (process_rds.check_valid() == true)
  {
    for (int r=0; r<4;r++)
    {
      chain_send += "|"+String(int(blocks[r]));
    }

  } 
  else 
  {
    for (int r=0; r<4;r++)
    {
      chain_send += "|"+String("NULL");
    }
  }

 

  String buffer_type[2];
  //-------------------------- pty -------------------------------------
  identifier_type(process_rds.get_type(),buffer_type);
  chain_send=chain_send+"|"+buffer_type[0]+"|"+buffer_type[1];

  //---------------------- Name ------------------------------------------
  if (process_rds.FLAG_NAME == true)
  {
    chain_send+="|";

    for (int r=0;r<8;r++)
    {
      chain_send+=char(process_rds.buffer_[r]);
    }
  }
  else
  {
    chain_send=chain_send+"|none";
  }  

  //-------------------------- PTY ------------------------------

  if (process_rds.check_valid() == true)
  {
    String pty = identifier_pty(process_rds.get_pty());

    chain_send += "|"+ String(pty);
  }
  else
  {
    chain_send += "|none";

  }

  //----------------------- Strength RDS -----------------------
  chain_send += "|" + String(process_rds.signal_RDS);

  //--------------------- Text Radio ---------------------------
  chain_send += "|";
  if (process_rds.FLAG_RADIO_TEXT)
  {
    char type = process_rds.type_text;

    

    if (type=='A')
    {
      for (int r=0;r<64;r++)
      {
        chain_send +=  char(process_rds.radio_textA[r]);
      }
    }
    else if (type=='B')
    {
      for (int r=0;r<64;r++)
      {
      chain_send += char(process_rds.radio_textB[r]);
      }
    }
    else 
    {
      chain_send += String("Obteniendo texto");
    }
  }
  else
  {
    chain_send += String("No signal");
  }

  chain_send += "|" + String(FREQ_RDS);

  return chain_send;
}


String data_latlonTest_generator(void)
{
  String chain_send = data_RDSTest_generator("latlon_test");

  if (process_rds.FLAG_LONLAT)
  {
    chain_send += "|" + String(process_rds.lat) + "|" + String(process_rds.lon) ;
    chain_send += "|" + String(process_rds.radio);
  }
  else
  {
    chain_send += "|none|none|none";
  }

  return chain_send;


}



String data_latenciaTest_generator(void)
{
  String chain_send = data_RDSTest_generator("latencia_test");

  if (process_rds.FLAG_MS_NEW)
  {
    process_rds.FLAG_MS_NEW = false;
    chain_send += "|" + String(process_rds.ms_total);
  }
  else
  {
    chain_send += "|none";
  }

  return chain_send;
}



void processRTC_Function(void * pvParameters)
{ while(1){
      byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
    xSemaphoreTake(i2cMutex, portMAX_DELAY);
    readDS3231time( &second, &minute, &hour, &dayOfWeek, &dayOfMonth, &month, &year);
    xSemaphoreGive(i2cMutex);
    esp_task_wdt_reset();
 

 

    string_date_sistema = String(year) + "/"+String(month) + "/" + String(dayOfMonth) + "  " + String(hour) + ":" + String(minute) + ":"+String((int)second);
  
    string_date_LCD = String(year) + "/"+String(month) + "/" + String(dayOfMonth) + "  " + String(hour) + ":" + String(minute);
  
    vTaskDelay(pdMS_TO_TICKS(0.5*1000));
    esp_task_wdt_reset();
}
}
 


//*************************************************************************************
//-------------------------------------- Setup ----------------------------------------
//*************************************************************************************
void setup()
{
  Wire.begin();
  //------------------------------ Interrupcion -----------------------------------
 
  pinMode(GPIO2_Si, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  //------------------------------- Lectura de variablE EEPROM ----------------------
  latitud_center = EepromRTC.readFloat(1);
  longitud_center = EepromRTC.readFloat(5);
  NUMBER_DEVICE = (int)EepromRTC.readFloat(9);
  FREQ_RDS = (float)EepromRTC.readFloat(13);

  EepromRTC.readChars(17,NUMBER_DEVICE_ID,30);
  

  device_name = device_name + String(NUMBER_DEVICE);
  //---------------------------------------------------------------------------------
 
  //read_EEPROM('A');
  //configuramos wdt
  esp_task_wdt_init(WDT_TIMEOUT, true);  

  Serial.begin(9600);
  
  //--------------------------------- Bluetooth ----------------------------------
  
  SerialBT.begin(device_name); //Bluetooth device name
  
  //---------------------------------Nokia 5110 Display---------------------
  display.begin();

  display.setContrast(50);
  display.clearDisplay();
  pinMode(pinLcd, OUTPUT);

  //Pantalla apagada
  digitalWrite(pinLcd,LOW);



          
  //---------------------------------------Sirena--------------------------------
  ledcAttachPin(pinAudio2, channel2);
  ledcSetup(channel2, freq2, resolution2);
    digitalWrite(pinLcd, HIGH);

  // pitido inicial
    freq2=440;ledcWrite(channel2, freq2);delay(100);
    
    freq2=880;ledcWrite(channel2, freq2);delay(100);
    
    digitalWrite(pinLcd, LOW);

    freq2=1320;ledcWrite(channel2, freq2);delay(100);
  
    freq2=2200;digitalWrite(pinLcd, HIGH);ledcWrite(channel2, freq2);delay(100);



    freq2=2500;ledcWrite(channel2, freq2);digitalWrite(pinLcd, LOW);

    delay(100);
    
  //-----------------------------------Radio Si4703-----------------------------
  radio.powerOn();
  radio.setVolume(volumen);
  float value = FREQ_RDS*10.0;
  radio.setChannel(value);

  //----------------------------------Relay----------------------------------------
  pinMode(pinRelay, OUTPUT_OPEN_DRAIN);
  digitalWrite(pinRelay, HIGH);
  delay(2);
  

  //----------------------------- Semaphore ----------------------------------------
  
  i2cMutex = xSemaphoreCreateMutex();
  spiMutex = xSemaphoreCreateMutex();
  //---------------------------------- xTasks--------------------------------------
 
  xTaskCreatePinnedToCore(Task_for_sirena_function, "sirena", 5000, NULL, 3, &Task_for_sirena,0);
  vTaskSuspend(Task_for_sirena);

  xTaskCreatePinnedToCore(Task_for_display_function, "display", 4000, NULL, 2, &Task_for_display,0);
  xTaskCreatePinnedToCore(Task_for_OTAUpdate_function, "OTA_Update", 20000, NULL,5 , &Task_for_OTA,0);

  delay(200);

 
 
  xTaskCreatePinnedToCore(processBT_Function, "BT", 10000, NULL, 6, &Task_for_BT,1);
  xTaskCreatePinnedToCore(processRTC_Function, "RTC", 3000, NULL, 3, &Task_for_RTC,1);
  xTaskCreatePinnedToCore(processINT_Function, "button", 4000, NULL, 1, &Task_for_INT,1);
  xTaskCreatePinnedToCore(processRDS_Function, "RDS", 4000, NULL, 4, &Task_for_RDS,1);
 
  //gpio_install_isr_service(0);
  //gpio_isr_handler_add(INTERRUPT_PIN, my_interruption, NULL);
  attachInterrupt(GPIO2_Si, my_interruption, FALLING);
  attachInterrupt(buttonPin, button_interrupt, FALLING);

  delay(150);    
}

void loop(){}

 