#ifndef MYLIB2_H
#define MYLIB2_H
#include <Arduino.h> 

String identifier_pty(int result){ 
  if(result==0){
    return String("No programme type or undefinied.");
  }
  else if (result==1){
    return String("News.");
  }
  else if (result==2){
    return String("Current Afairs.");
  }
  else if (result==3){
    return String("Information.");
  }
  else if (result==4){
    return String("Sport");
  }
  else if (result==5){
    return String("Education");
  }
  else if (result==6){
    return String("Drama");
  }
  else if (result==7){
    return String("Culture");
  }
  else if (result==8)
  {
    return String("Science");
  }
  else if (result==9){
    return String("Varied");
  }
  else if (result==10){
    return String("Pop Music");
  }
  else if (result==11){
    return String("Rock Music");
  }
  else if (result==12){
    return String("Easy Listening Music");
  }
  else if (result==13){
    return String("Light Classical");
  }
  else if (result==14){
    return String("Serious Classical");
  }
  else if (result==15){
    return String("Other Music");
  }
  else if (result==16){
    return String("Weather");
  }
  else if (result==17){
    return String("Finance");
  }
  else if (result==18){
    return String("Children's programme");
  }
  else if (result==19){
    return String("Social Affairs");
  }
  else if (result==20){
    return String("Religion");
  }
  else if (result==21){
    return String("Phone In");
  }
  else if (result==22){
    return String("Travel");
  }
  else if (result==23){
    return String("Leisure");
  }
  else if (result==24){
    return String("Jazz Music");
  }
  else if (result==25){
    return String("Country Music");
  }
  else if (result==26){
    return String("National Music");
  }
  else if (result==27){
    return String("Oldies Music");
  }
  else if (result==28){
    return String("Folk Music");
  }
  else if (result==29){
    return String("Documentary");
  }
  else if (result==30){
    return String("Alarm Test");
  }
  else if (result ==31){
    return String("Alarm");
  }
  else{
    return String("none");
  }
}
#endif
