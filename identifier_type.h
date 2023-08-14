#ifndef MYLIB3_H
#define MYLIB3_H
#include <Arduino.h>

void identifier_type(int result, String (& buff) [2]){

    // Funcion que identifica el tipo de señal RDS que se está enviando en
    // ese instante.
    // Entradas:
    // result: Valor entero.
    // buff: Cadena String que permite guardar la información.
    if (result==0 )
    {
        buff[0]= "Group Type 0A";
        buff[1]= "Basic tunning and switching information only";
    }
    
    else if (result==1 )
    {
        buff[0]= "Group Type 0B";
        buff[1]= "Basic tunning and switching information only";
    }

    else if (result==2 )
    {
        buff[0]= "Group Type 1A";
        buff[1]= "Programme Item Number";
    }

    else if (result==3 )
    {
        buff[0]= "Group Type 1B";
        buff[1]= "Programme Item Number";
    }

    else if (result==4 )
    {
        buff[0]= "Group Type 2A";
        buff[1]= "RadioText Only";
    } 

    else if (result==5 )
    {
        buff[0]= "Group Type 2B";
        buff[1]= "RadioText Only";
    }
    else if (result==6 )
    {
        buff[0]= "Group Type 3A";
        buff[1]= "Application Identification for ODA only";
    }
    else if (result==7)
    {
        buff[0]= "Group Type 3B";
        buff[1]= "Open Data Applications";
    }
    else if (result==8)
    {
        buff[0]= "Group Type 4A";
        buff[1]= "Clock-time and date only";
    }
    else if (result==9 )
    {
        buff[0]= "Group Type 4B";
        buff[1]= "Open Data Applications";
    }
    else if (result== 10)
    {
        buff[0]= "Group Type 5A";
        buff[1]= "Transparent Data Channels or ODA";
    }
    else if (result==11 )
    {
        buff[0]= "Group Type 5B";
        buff[1]= "Transparent Data Channels or ODA";
    }
    else if (result==12 )
    {
        buff[0]= "Group Type 6A";
        buff[1]= "In House application or ODA";
    }
    else if (result==13 )
    {
        buff[0]= "Group Type 6B";
        buff[1]= "In house applications or ODA";
    }
    else if (result==14)
    {
        buff[0]= "Group Type 7A";
        buff[1]= "Radio Paging or ODA";        
    }

    else if (result==15)
    {
        buff[0]= "Group Type 7B";
        buff[1]= "Open Data Applications";        
    } 


    else if (result==16)
    {
        buff[0]= "Group Type 8A";
        buff[1]= "Traffic Message Channel or ODA";        
    } 

    else if (result==17)
    {
        buff[0]= "Group Type 8B";
        buff[1]= "Open Data Applications";        
    } 

    else if (result==18)
    {
        buff[0]= "Group Type 9A";
        buff[1]= "Emergency Warning System or ODA";        
    } 

    else if (result==19)
    {
        buff[0]= "Group Type 9B";
        buff[1]= "Open Data Applications";        
    } 

    else if (result==20)
    {
        buff[0]= "Group Type 10A";
        buff[1]= "Programme Type Name";        
    } 

    else if (result==21)
    {
        buff[0]= "Group Type 10B";
        buff[1]= "Open Data Applications";        
    }

    else if (result==22)
    {
        buff[0]= "Group Type 11A";
        buff[1]= "Open Data Applications";        
    }  

    else if (result==23)
    {
        buff[0]= "Group Type 11B";
        buff[1]= "Open Data Applications";        
    }  

    else if (result==24)
    {
        buff[0]= "Group Type 12A";
        buff[1]= "Open Data Applications";        
    }  

    else if (result==25)
    {
        buff[0]= "Group Type 12B";
        buff[1]= "Open Data Applications";        
    }  

    else if (result==26)
    {
        buff[0]= "Group Type 13A";
        buff[1]= "Enhaced Radio Paging";        
    }  
    
    else if (result==27)
    {
        buff[0]= "Group Type 13B";
        buff[1]= "Open Data Applications";        
    }  

    else if (result==28)
    {
        buff[0]= "Group Type 14A";
        buff[1]= "Enhaced Other Networks";        
    }  
    else if (result==29)
    {
        buff[0]= "Group Type 14B";
        buff[1]= "Enhaced Other Networks";        
    }  
    else if (result==30)
    {
        buff[0]= "Group Type 15A";
        buff[1]= "Defined in RBDS only";        
    }  
    else if (result==31)
    {
        buff[0]= "Group Type 15B";
        buff[1]= "Fast switching information only";        
    }

    else {
        buff[0]= "No definido"; 
       buff[1]= "No definido";
    }
}
#endif
