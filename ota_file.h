#include "HttpsOTAUpdate.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static const char *url = "https://raw.githubusercontent.com/Armando123x/kp_IDL_py/main/sketch_230802_3a.ino.bin";
 

static const char *ssid     = "maracumango";  // your network SSID (name of wifi network)
static const char *password = "12345678"; // your network password


static const char *server_certificate = "-----BEGIN CERTIFICATE-----\n" \
"MIIDrzCCApegAwIBAgIQCDvgVpBCRrGhdWrJWZHHSjANBgkqhkiG9w0BAQUFADBh\n" \
"MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n" \
"d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBD\n" \
"QTAeFw0wNjExMTAwMDAwMDBaFw0zMTExMTAwMDAwMDBaMGExCzAJBgNVBAYTAlVT\n" \
"MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n" \
"b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IENBMIIBIjANBgkqhkiG\n" \
"9w0BAQEFAAOCAQ8AMIIBCgKCAQEA4jvhEXLeqKTTo1eqUKKPC3eQyaKl7hLOllsB\n" \
"CSDMAZOnTjC3U/dDxGkAV53ijSLdhwZAAIEJzs4bg7/fzTtxRuLWZscFs3YnFo97\n" \
"nh6Vfe63SKMI2tavegw5BmV/Sl0fvBf4q77uKNd0f3p4mVmFaG5cIzJLv07A6Fpt\n" \
"43C/dxC//AH2hdmoRBBYMql1GNXRor5H4idq9Joz+EkIYIvUX7Q6hL+hqkpMfT7P\n" \
"T19sdl6gSzeRntwi5m3OFBqOasv+zbMUZBfHWymeMr/y7vrTC0LUq7dBMtoM1O/4\n" \
"gdW7jVg/tRvoSSiicNoxBN33shbyTApOB6jtSj1etX+jkMOvJwIDAQABo2MwYTAO\n" \
"BgNVHQ8BAf8EBAMCAYYwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQUA95QNVbR\n" \
"TLtm8KPiGxvDl7I90VUwHwYDVR0jBBgwFoAUA95QNVbRTLtm8KPiGxvDl7I90VUw\n" \
"DQYJKoZIhvcNAQEFBQADggEBAMucN6pIExIK+t1EnE9SsPTfrgT1eXkIoyQY/Esr\n" \
"hMAtudXH/vTBH1jLuG2cenTnmCmrEbXjcKChzUyImZOMkXDiqw8cvpOp/2PV5Adg\n" \
"06O/nVsJ8dWO41P0jmP6P6fbtGbfYmbW0W5BjfIttep3Sp+dWOIrWcBAI+0tKIJF\n" \
"PnlUkiaY4IBIqDfv8NZ5YBberOgOzW6sRBc4L0na4UU+Krk2U886UAb3LujEV0ls\n" \
"YSEY1QSteDwsOoBrp+uvFRTp2InBuThs4pFsiv9kuXclVzDAGySj4dzp30d8tbQk\n" \
"CAUw7C29C79Fv1C5qfPrmAESrciIxpg0X40KPMbp1ZWVbd4=\n" \
"-----END CERTIFICATE-----\n" ;

static HttpsOTAStatus_t otastatus;
void HttpEvent(HttpEvent_t *event)
{
    switch(event->event_id) {
        case HTTP_EVENT_ERROR:
            //Serial.println("Http Event Error");
            break;
        case HTTP_EVENT_ON_CONNECTED:
            //Serial.println("Http Event On Connected");
            break;
        case HTTP_EVENT_HEADER_SENT:
            //Serial.println("Http Event Header Sent");
            break;
        case HTTP_EVENT_ON_HEADER:
            //Serial.printf("Http Event On Header, key=%s, value=%s\n", event->header_key, event->header_value);
            break;
        case HTTP_EVENT_ON_DATA:
            break;
        case HTTP_EVENT_ON_FINISH:
            //Serial.println("Http Event On Finish");
            break;
        case HTTP_EVENT_DISCONNECTED:
            //Serial.println("Http Event Disconnected");
            break;
    }
}



void Task_for_OTAUpdate_function(void *pvParameters)
{ 
  while (1)
  {
  ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
  
  esp_task_wdt_init(WDT_TIMEOUT, true);  

  WiFi.begin(ssid, password);
 

  // attempt to connect to Wifi network:

  bool flag_wifi = true;
  unsigned long count = millis() ;

  while (WiFi.status() != WL_CONNECTED) {
    if ((millis() - count )>7000) 
    {
      //Serial.println("La actualización falló porque no se pudo conectar al wifi");
      flag_wifi = false;
      break;
    }
    delay(50);
    }
  if (flag_wifi==true)
  {
  Serial.println("Conectado al Wifi");

    HttpsOTA.onHttpEvent(HttpEvent);
    Serial.println("Starting OTA");
    HttpsOTA.begin(url, server_certificate); 
    otastatus = HttpsOTA.status();
    Serial.println("LLegamos aqui");

    unsigned long var = millis();
    while(millis()-var < 120*1000){
    if(otastatus == HTTPS_OTA_SUCCESS) 
    { 
        Serial.println("Firmware written successfully. To reboot device, call API ESP.restart() or PUSH restart button on device");
        ESP.restart();
    } 
    else if(otastatus == HTTPS_OTA_FAIL) 
    { 
        Serial.println("Firmware Upgrade Fail");
        display_STATUS_GLOBAL = 'N'; 
        delay(10);
        WiFi.disconnect(true);
        ESP.restart();
    

    }

    esp_task_wdt_reset();
    delay(10);
    
  }
  Serial.println("Se excedió el tiempo de respuesta. Reiniciando ...");
  ESP.restart();
  }

  else
  {
    Serial.println("No se pudo conectar al wifi");
    ESP.restart();
  }
  }
  
}