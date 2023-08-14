//***********************************************************//
//****************************Library************************//
//***********************************************************//

//Nokia 5110 Display
#include <Arduino.h>
#include <U8x8lib.h>

#ifdef U8X8_HAVE_HW_SPI
#endif
 
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

#include "rtc.h"
 

// #if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
// #error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
// #endif
// #if !defined(CONFIG_BT_SPP_ENABLED)
// #error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
// #endif


//--------------------------------- Define constanst ---------------------------------
#define WDT_TIMEOUT 100


//*************************************************************************************
//----------------------------------- Constants --------------------------------------
//*************************************************************************************
  String string_date_sistema = "";
//------------------------------------   LCD -------------------------------------------
U8X8_PCD8544_84X48_4W_HW_SPI u8x8(2, 4, 15); //spi (cs, dc, reset)  
const int pinLcd = 26;
int ledState = LOW; 

String Longitud = "Lon.";  
String Latitud = "Lat."; 
String Radio = "RDS";  
String Frecuencia = "Freq:"; 
String Senal = "         Senal:";  
String Mensaje = "Mensaje:";

//------------------------------------ Sirena ------------------------------------------
//sirena
//#include "audio.h"
 
const int pinAudio2 = 14; 
int freq2 = 2200;
int channel2 = 0;
int resolution2 = 8;

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
 
TaskHandle_t Task_for_display;


//------------------------------ Parametros SASPe ---------------------------------//

float latitud_center = 0;
float longitud_center = 0;
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

}

void read_EEPROM(char letter)
{
  if (letter=='A')
  {
    //Leemos valores SASPE
    // - Latitud
    // - Longitud
    // - Frecuencia_RDS
    return;
  }

  else if (letter=='B')
  {
    //leemos un historico de datos

    return;
  }
  else 
  {
    return ;
  }
}



bool check_event(float R, float lat, float lon){  
 
  float diff_lat = abs(latitud_center-lat);
  float diff_lon = abs(longitud_center-lon);
  diff_lat = diff_lat*dlat;
  diff_lon = diff_lon*dlon;
  double diff = sqrt(pow(diff_lat,2)+pow(diff_lon,2));
  
  if(diff<=R) return true;
  else return false;
} 


 
 

//*************************************************************************************
//------------------------------ Interrupciones ----------------------------------------
//*************************************************************************************




void IRAM_ATTR read_RDS() {
  //Ejecución de la interrupción
  radio.getBlocks(blocks,1);
  process_rds.set_block(blocks);
 

 
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

volatile unsigned long time0_ALARM = 0;
void Task_for_sirena_function(void* parameter)
{
   // define your code for sirena
  // ACTIVAMOS EL RELÉ
  // probamos la sirena por un minuto 
  digitalWrite(pinRelay, HIGH);

  
  while(1)
  {
    
  if (!(millis() - time0_ALARM > 1000*60))
  {
    // for (int i=0; i<33452; ++i)
    // {
    // dacWrite(pinAudio2, rawData2[i]);
    // esp_task_wdt_reset();
    // delayMicroseconds(14);
    // }

    // for (int r=0;r<1000;r++)
    // {
    // for (int i=0; i<35756; ++i)
    // {
    // dacWrite(pinAudio2, rawData1[i]);
    // esp_task_wdt_reset();
    // delayMicroseconds(14);
    // }
    // }


    freq2=440;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));

    freq2=880;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));

    freq2=1320;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));
    
    freq2=2200;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));

    freq2=2500;
    ledcWrite(channel2, freq2);
    vTaskDelay(pdMS_TO_TICKS(100));

  }
  else
  {
    digitalWrite(pinRelay, LOW);
    esp_task_wdt_reset();
    vTaskDelay(pdMS_TO_TICKS(10));
    // suspendemos la tarea
    vTaskSuspend(NULL);
  }
}
}
SemaphoreHandle_t spiMutex;


volatile char display_STATUS_GLOBAL = 'F';
void Task_for_display_function(void* parameter)
{
  char ultimate = 'N';
  //presentacion 
  xSemaphoreTake(spiMutex, portMAX_DELAY);

  {
  esp_task_wdt_reset();

  u8x8.setFont(u8x8_font_victoriabold8_r  );
  //fondo blanco
  esp_task_wdt_reset();

  u8x8.drawString(0,0,""); 
  esp_task_wdt_reset();

  u8x8.setInverseFont(0);
  esp_task_wdt_reset();


  //fondo oscuro
  u8x8.drawString(0,1,"╔ ═ ═ ═ ═ ╗");
  esp_task_wdt_reset();

  u8x8.setInverseFont(1);
  esp_task_wdt_reset();

  u8x8.drawString(0,2, "----SASPe----"); 
  esp_task_wdt_reset();

  u8x8.setInverseFont(1);
  esp_task_wdt_reset();

  u8x8.drawString(0,3,"╚ ═ ═ ═ ═ ╝");
  esp_task_wdt_reset();

  u8x8.setInverseFont(1);
  esp_task_wdt_reset();


  //fondo blanco
  u8x8.drawString(0,4,"");
  u8x8.setInverseFont(0);
  esp_task_wdt_reset();

  
  u8x8.refreshDisplay();   
  xSemaphoreGive(spiMutex);

  vTaskDelay(pdMS_TO_TICKS(2500));
  esp_task_wdt_reset();
   


  }
 
  while(1)
  {
    xSemaphoreTake(spiMutex, portMAX_DELAY);
    if (ultimate == 'N')
    {
      // cargamos pantalla normal;
       
 
      esp_task_wdt_reset();


      String sLongitud = Longitud + longitud_center;  
      esp_task_wdt_reset();

      String sLatitud = Latitud + latitud_center; 
      String sRadio = "Rds:" + String(process_rds.signal_RDS);  
      String sFrecuencia = Frecuencia + String(FREQ_RDS); 
      esp_task_wdt_reset();

      String sHoraSistema =   string_date_sistema;
      //String sSenal = Senal + vSenal;
      //String sMensaje = Mensaje + vMensaje;
      
      

      u8x8.drawString(0,0,"------"); //Sistema de Alerta Sísmico Peruano - SASPe
      u8x8.setInverseFont(0);//fondo oscuro
      esp_task_wdt_reset();


      u8x8.drawString(0,1,"");
      // u8x8.setInverseFont(0);//fondo blanco
      esp_task_wdt_reset();

      
      u8x8.drawString(0,1,sLongitud.c_str());
      // u8x8.setInverseFont(1);//fondo blanco
      esp_task_wdt_reset();

      
      u8x8.drawString(0,2,sLatitud.c_str());
      // u8x8.setInverseFont(0);//fondo blanco
      esp_task_wdt_reset();

      
      u8x8.drawString(0,3,sRadio.c_str());
      // u8x8.setInverseFont(0);//fondo blanco
      esp_task_wdt_reset();


      u8x8.drawString(0,4,sFrecuencia.c_str());
      // u8x8.setInverseFont(0);//fondo blanco
      esp_task_wdt_reset();

      
      u8x8.drawString(0,5,sHoraSistema.c_str());
      // u8x8.setInverseFont(0);//fondo blanco
      esp_task_wdt_reset();



      u8x8.refreshDisplay();   
      esp_task_wdt_reset();
    }

    if (ultimate == 'E')
    {
      //emergencia
      
    }
    xSemaphoreGive(spiMutex);
    // vTaskDelay(pdMS_TO_TICKS(1000*3));
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
 
    xSemaphoreTake(i2cMutex, portMAX_DELAY);
    bool flag_radio = radio.getBlocks(blocks,1);
    xSemaphoreGive(i2cMutex);

    if (flag_radio==true)

    {  
        esp_task_wdt_reset();

        process_rds.set_block(blocks);
        esp_task_wdt_reset();

        process_rds.check_timeout(true);
    }
    else
    {
      process_rds.check_timeout(false);
    esp_task_wdt_reset();

    }

    //reset watchdog
    esp_task_wdt_reset();


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
        vTaskSuspend(Task_for_sirena);
        flag_send = false;
      }      

      else if (data.indexOf("RELE_test_ON")!=-1)
      {
        // vTaskResume(Task_for_rele);
        flag_send = false;
      }

      else if (data.indexOf("RELE_test_OFF")!=-1)
      {
        // vTaskSuspend(Task_for_rele);
        time0_RELE = 0;
        //digitalWrite(pinRelay, LOW);
        delay(10);

        flag_send = false;
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
  if (process_rds.FLAG_BLOCKS == true)
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

  if (process_rds.FLAG_BLOCKS == true)
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
 

  // year = String(year);
  // month = String(month);
  // day  = String(day);

  // hour = String(hour);
  // minute = String(minute);
  // second = String(second);

  // if (minute<10) minute = "0"+String((int)hour);
  // if (second<10) second = "0"+String((int)second);

    string_date_sistema = String(year) + "/"+String(month) + "/" + String(dayOfMonth) + "  " + String(hour) + ":" + String(minute) + ":"+String((int)second);;
  

    // vTaskDelay(pdMS_TO_TICKS(4*1000));
    vTaskDelay(pdMS_TO_TICKS(0.5*1000));
    esp_task_wdt_reset();
}
}
//----------------------------------- Principal function Core 2 ----------------------------------------
void Core2(void * pvParameters)
{ 

    //---------------------------------- Interruption -------------------------
    //attachInterrupt(GPIO2_Si, read_RDS, FALLING);
    //------------------------------- Tareas--------------------------------------
    //xTaskCreate(processRDS_Function, "SubTask", 1000, NULL, 1, &Task_for_RDS);
   
    //Process BT

    while(1)
    {
      delay(2);
    esp_task_wdt_reset();




    }

     
}


//*************************************************************************************
//-------------------------------------- Setup ----------------------------------------
//*************************************************************************************
void setup()
{ 
  //------------------------------- Lectura de variables SASPe----------------------
 
  //read_EEPROM('A');
  
  //--------------------------------- Bluetooth ----------------------------------
  
  SerialBT.begin(device_name); //Bluetooth device name
  
  //---------------------------------Nokia 5110 Display---------------------
  u8x8.begin();
  u8x8.setPowerSave(0);
  pinMode(pinLcd,OUTPUT);

  //Nokia 5110 Display            
  //---------------------------------------Sirena--------------------------------
  ledcAttachPin(pinAudio2, channel2);
  ledcSetup(channel2, freq2, resolution2);
 
  //-----------------------------------Radio Si4703-----------------------------
  radio.powerOn();
  radio.setVolume(volumen);
  radio.setChannel(FREQ_RDS*10);

  //----------------------------------Relay----------------------------------------
  pinMode(pinRelay, OUTPUT_OPEN_DRAIN);
  //digitalWrite(pinRelay, HIGH);
  delay(2);
  

  
  i2cMutex = xSemaphoreCreateMutex();
  spiMutex = xSemaphoreCreateMutex();
  //---------------------------------- xTasks--------------------------------------
  //xTaskCreatePinnedToCore (Core1,"Task1",12000,NULL,2,&Task1,0);
  xTaskCreatePinnedToCore(Task_for_sirena_function, "sirena", 6500, NULL, 3, &Task_for_sirena,0);
  vTaskSuspend(Task_for_sirena);



  xTaskCreatePinnedToCore(Task_for_display_function, "display", 4000, NULL, 4, &Task_for_display,0);

  delay(200);

 
  // xTaskCreatePinnedToCore (Core2,"Task2",12000,NULL,5,&Task2,1);    
  xTaskCreatePinnedToCore(processBT_Function, "BT", 10000, NULL, 6, &Task_for_BT,1);
 

  xTaskCreatePinnedToCore(processRTC_Function, "RTC", 4000, NULL, 1, &Task_for_RTC,1);

  delay(200);    
}

void loop(){}

 