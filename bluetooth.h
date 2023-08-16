#ifndef MYLIB1_H
#define MYLIB1_H
#include <Arduino.h>
#include "BluetoothSerial.h"
//BluetoothSerial SerialBT;
BluetoothSerial SerialBT;


String device_name = "SASPe_ROJ";
String bluetoothReadLine(){
    String text_received = "";
    while(SerialBT.available()){
        byte r = SerialBT.read();
        if(r!=13 && r!=10 && char(r)!='\0')
          text_received = text_received + char(r);
    }return text_received;      
}

void writeSerialBT(String respuesta){
  SerialBT.print(respuesta);
  SerialBT.println();
  SerialBT.flush();
}

#endif
