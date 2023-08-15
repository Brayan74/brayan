#include "myfunction.h"

#include "esp_task_wdt.h"

uint16_t operation(uint16_t A, uint16_t B, uint16_t C)
{
    return ((A&B) | (B)&(A^C) );
}

 
void process_read::check_timeout(bool flag){

    long current_point=millis();

    if (flag==false){
        if (current_point>set_point){
            FLAG_BLOCKS = false; 
            countFailedRDS++;
            }    
    }
    else{
        set_point=millis() + timeout;
        FLAG_BLOCKS = true;
        countSucessRDS++;
    }
    esp_task_wdt_reset();

    

}


void process_read:: check_RDS(void){
    
    long diff =  millis() - set_point_RDS;

    if (diff>0){
        signal_RDS = (countSucessRDS)/(DEFINE_RDS_PER_SECOND*3)*100;
        
        set_point_RDS = millis() + second*3;
        countSucessRDS=0;
    }

     esp_task_wdt_reset();
   

    
}

void process_read :: reset(void){

        completedCount=0;

        for (int i=0;i<4;i++)
        {
            completed[i]=false;
        }  

        FLAG_NAME=false;
        FLAG_BLOCKS=false;
        FLAG_RADIO_TEXT = false; 

        for (int ind=0;ind<64;ind++){
                radiotext_A[ind]=false;
                }
        for (int ind=0;ind<32;ind++){
                radiotext_B[ind]=false;
                }
        
        completedTextA=0;
        completedTextB=0;

        type_text='C';

        memset(radio_textA,0,sizeof(radio_textA));
        memset(radio_textB,0,sizeof(radio_textB));
    esp_task_wdt_reset();

}


void process_read :: get_name(void){
    int index = BLOCK_B & 0x03;

    if (! completed[index] && BLOCK_B < 500){
        completed[index] = true;
        completedCount ++;
	  	char Dh = (BLOCK_D & 0xFF00) >> 8;
      	char Dl = (BLOCK_D & 0x00FF);


		buffer_[index * 2] = Dh;
		buffer_[index * 2 +1] = Dl;

        
    }
    if (completedCount==4){

        FLAG_NAME = true;
 
        //reset 
        completedCount=0;

        for (int i=0;i<4;i++)
        {
            completed[i]=false;
        }  

        return ;
    }
    else{
        return;

    }
}


void process_read:: set_block(uint16_t* block){


    BLOCK_A =   block[BLOCKA];
    BLOCK_B =   block[BLOCKB];
    BLOCK_C =   block[BLOCKC];
    BLOCK_D =   block[BLOCKD];

    get_name();
    esp_task_wdt_reset();

    get_radiotext();
    esp_task_wdt_reset();

    check_RDS();
    esp_task_wdt_reset();

     

}

int process_read:: get_type(void){

    if(FLAG_BLOCKS==true){
    uint16_t number = (IDENTIFIER_TYPE << 11) & (BLOCK_B);

    number  = number >> 11;

    return int(number);
    }
    else 
    {
        return int(100);
    }
    
}

int delay(int value){

    if (value==162) return 95;
    if (value==160) return 96;
    if (value==180) return 97;
    if (value==178) return 98;
    if (value==179) return 99;
    if ((value>=32) && (value<=126))
    {
        return value-32;
    }    

}

// 0-9 a-z A-Z = 63 valores
int decodificador_to_3662 (int value){
    if ((value>=65) &&(value<=90)){
        //valores  mayusculas [A-Z]
        return value - 55;  
          }

    if ((value>=48) && (value <=57))
    {   // valores numero [0-9]
        return value - 48;
    }
    if  ((value>=97) && (value <=122))
    {   //valores minusculas [a-z]
        return value - 61;
    }
}

//retorna un decimal en base 10 -- radio de 0 36[2]-1
float todecimal36(int value0,int value1){
    // number <-> (value0)(value1) en base 36
    return value0*36 + value1;

} 

//retorna un decimal en base 10
float todecimal62(int value0,int value1){
    // number <-> (value0)(value1) en base 62
    return value0*62 + value1;
}
//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//------------------------------------ others func -------------------------------------------------//
//--------------------------------------------------------------------------------------------------//
//--------------------------------------------------------------------------------------------------//

int return_index(bool array[],int size_array)

{
    for (int i=0; i<size_array;i++)
    {
        if (!array[i]) return i;
    }
}

float calcularModa(float array[], int size) {
  int maxCount = 0;
  float moda = 0;

  for (int i = 0; i < size; ++i) {
    int count = 0;
    for (int j = 0; j < size; ++j) {
      if (array[j] == array[i]) {
        ++count;
      }
    }

    if (count > maxCount) {
      maxCount = count;
      moda = array[i];
    }
  }

  if ((maxCount==1) or (maxCount< 0.5*size)) return -1;
  

  return moda;
}


void process_read :: reset_emergency(void)
{
    for (int r=0; r<10; r++)
    {
        emergency_signal[r] = false;

        latitud_array[r] = 0;
        longitud_array[r] = 0;
        radio_array[r] = 0;

        BUFF_BLOCK_C_0[r] = 0x00;
        BUFF_BLOCK_D_0[r] = 0x00;
        BUFF_BLOCK_C_0[r] = 0x00;
        BUFF_BLOCK_C_1[r] = 0x00;
        
        
    }
}
void process_read :: get_error_bit_per_bit(float array[])
{
    int idx = return_index(emergency_signal,10);
    float lati,loni,radi;
    uint16_t BC0,BC1,BD0,BD1;

    if (idx==3)
    {
        BC0 = operation(BUFF_BLOCK_C_0[0],BUFF_BLOCK_C_0[1],BUFF_BLOCK_C_0[2]);
        BC1 = operation(BUFF_BLOCK_C_1[0],BUFF_BLOCK_C_1[1],BUFF_BLOCK_C_1[2]);
        BD0 = operation(BUFF_BLOCK_D_0[0],BUFF_BLOCK_D_0[1],BUFF_BLOCK_D_0[2]);
        BD1 = operation(BUFF_BLOCK_D_1[0],BUFF_BLOCK_D_1[1],BUFF_BLOCK_D_1[2]);

        //-------------------------- Operation with BLOCK 0 --------------------------//
        //latitud 
        int C0_ = (BC0 & (0xFF00))>> 8;
        int C1_ = BC0 & (0x00FF);
        
        int C0 = decodificador_to_3662(C0_);
        int C1 = decodificador_to_3662(C1_);

        lati = todecimal62(C0,C1);
        lati = lati*-1.0/100;

        //longitud
        int D0_ = (BD0 & (0xFF00))>> 8;
        int D1_ = BD0 & (0x00FF);

        int D0 = decodificador_to_3662(D0_);
        int D1 = decodificador_to_3662(D1_);

        loni = todecimal62(D0,D1);
        loni  = loni*-1.0/100-68;


        //-------------------------- Operation with BLOCK 1 --------------------------//
        // radio 
        C0 = (BC1 & (0xFF00))>> 8;
        C1 = BC1 & (0x00FF);

        C0 = decodificador_to_3662(C0);
        C1 = decodificador_to_3662(C1);

        radi = todecimal36(C0,C1);

        // identificador
         D0 = (BD1 & (0xFF00))>> 8;
         D1 = BD1 & (0x00FF);

        D0 = decodificador_to_3662(D0);
        D1 = decodificador_to_3662(D1);  


    }
    array[0] = lati;
    array[1] = loni;
    array[2] = radi;
     

}
void process_read :: verify_emergency(void)
{
    /*
    Esta aplicación verifica si hay una emergencia
    -> Si no se vuelve a recibir otra señal en emergencia en 1 seg, la emergencia es invalida solo si:
        -> Solo hay una señal de emergencia
        -> Hay dos señales de emergencia pero con valores diferentes

    */  

   int index = return_index(emergency_signal,10);

    latitud_array[index] = lat;
    longitud_array[index] =lon;
    radio_array[index]  = radio;


    //-------------------- logica--------------------
   if (index==0)
   {
    //solo hay una señal de emergencia
    // se espera 1.5 segundos para confirmar emergencia 

    timeout_emergency = millis() + MAX_SECONDS_BETWEEN_EMERGENCY*1000;
    return;

   }
   if (index==1)
   {   
    //verificamos si el timeout venció 
    if (millis()-timeout_emergency>0)
    {
        // reseteamos todo
        reset_emergency();
        //volvemos a insertar los nuevos valores y esperamos otro timeout
        latitud_array[0] = lat;
        longitud_array[0] = lon;
        radio_array[0]  = radio;       

        timeout_emergency = millis() + MAX_SECONDS_BETWEEN_EMERGENCY*1000;

        return;
    }
    
    //hay dos elementos
    //Verificamos si estos valores son iguales;
    bool flag_check = true;

    if (latitud_array[index-1] != latitud_array[index]) flag_check =false;
    
    else if (longitud_array[index-1] != longitud_array[index])  flag_check = false;
    
    else if (radio_array[index-1]!= radio_array[index]) flag_check = false;

    if (flag_check == true)
    {   
        //verificamos si estamos en el radio de acción

        //activamos señal de emergencia
        latitud_emergency = latitud_array[index-1];
        longitud_emergency = longitud_array[index-1];
        radio_emergency = radio_array[index-1];

        reset_emergency();
        EMERGENCY_SIGNAL = true;
        return;
    }

   }
   else if (index==2)
   {
    // hay tres elementos 
    // si no lanzó, es porque hay elementos desiguales
    // verificaremos par en par
    bool flag_check = true;

    if (!((latitud_array[index-1]==latitud_array[index]) or (latitud_array[index-2]==latitud_array[index-1]) or (latitud_array[index]==latitud_array[index-2])))       flag_check = false;
    if (!((longitud_array[index-1]==longitud_array[index]) or (longitud_array[index-2]==longitud_array[index-1]) or (longitud_array[index]==longitud_array[index-2])))        flag_check = false;
    if (!((radio_array[index-1]==radio_array[index]) or (radio_array[index-2]==radio_array[index-1]) or (radio_array[index]==radio_array[index-2])))        flag_check = false;


    if (flag_check == false)
    {
    //comprobamos el error de bit a bit
    float array[3];
     get_error_bit_per_bit(array);
     latitud_emergency = array[0];
     longitud_emergency = array[1];
     radio_emergency = array[2];
    //corregido los bits, lanzamos la emergencia

    reset_emergency();
    EMERGENCY_SIGNAL = true;

    return;

    }
    else
    {
        //hubo dos datos validos

        //latitud
        if (latitud_array[index-1]==latitud_array[index]) latitud_emergency = latitud_array[index];
        if (latitud_array[index-2]==latitud_array[index-1]) latitud_emergency = latitud_array[index-1];
        if (latitud_array[index]==latitud_array[index-2])  latitud_emergency = latitud_array[index];

        //longitud
        if (longitud_array[index-1]==longitud_array[index]) longitud_emergency = longitud_array[index-1];
        if (longitud_array[index-2]==longitud_array[index-1]) longitud_emergency = longitud_array[index-1];
        if (longitud_array[index]==longitud_array[index-2]) longitud_emergency = longitud_array[index];

        //radio

        if((radio_array[index-1]==radio_array[index])) radio_emergency = radio_array[index-1];
        if((radio_array[index-2]==radio_array[index-1])) radio_emergency = radio_array[index-1];
        if((radio_array[index-2]==radio_array[index])) radio_emergency = radio_array[index];

        reset_emergency();
        EMERGENCY_SIGNAL = true;

        return;

    }

   }

    // else if (index>2)

    // if (!(flag_check))
    // //sigue marcando mal
    // // verificaremos moda por moda en cada valor 
    // {
    //     get_moda
    // }
   
}






// carga los valores de rds si hay alguna senal
void process_read :: get_radiotext(void)
{
    long diff = millis() - set_point_radiotext;
    if(FLAG_BLOCKS==true)

    {
        int type = get_type();
        int pty  = get_pty();

        //if ((type==20) && (pty==31))



        if ((type==20) && (pty==3000))
        {

            /*
            Bloque de medición de latencia
            */


            int x = BLOCK_B & 0x01;
            //medir latencia
            if ((x==0) or (x==1))
            {   
                //minutos
                int C0 = (BLOCK_C & (0xFF00))>> 8;
                C0 = decodificador_to_3662(C0);
                min1 = todecimal62(0,C0);
                //segundos
                int C1 = BLOCK_C & (0x00FF);
                C1 = decodificador_to_3662(C1);
                sec1 = todecimal62(0,C1);   
                //miliseconds
                int D0 = (BLOCK_D & (0xFF00))>> 8;
                int D1 = BLOCK_D & (0x00FF);
                D0  = decodificador_to_3662(D0);
                D1  = decodificador_to_3662(D1);

                mseconds = todecimal36(D0,D1);

                ms_total = min1*60*1000 + sec1*1000 + mseconds;

                if ((ms_total!=ums) && (ms_total>0)){
                    FLAG_MS_NEW = true;
                    ums = ms_total;
                    ms_total = -1;
                }
            }

        }


        // generamos valores aleatoriamente de lat y rad de emergencia 

        {
            latitud_emergency = -11.97;
            lat = latitud_emergency;

            longitud_emergency = -76.97;
            lon = longitud_emergency;

            radio_emergency = 100;
            radio = radio_emergency;
            FLAG_LONLAT = true;

            EMERGENCY_SIGNAL = true;


        }




        //if ((type==20) && (pty==30))
        if ((type==20) && (pty==31))
        {
            int x = BLOCK_B & 0x01;

            /*
            Bloque de obtención de latitud y longitud
            
            */
            if (x==0)
            {
                //estructura
                //latitud 
                int C0_ = (BLOCK_C & (0xFF00))>> 8;
                int C1_ = BLOCK_C & (0x00FF);
                
                int C0 = decodificador_to_3662(C0_);
                int C1 = decodificador_to_3662(C1_);

                lat = todecimal62(C0,C1);
                lat = lat*-1.0/100;

                //longitud
                int D0_ = (BLOCK_D & (0xFF00))>> 8;
                int D1_ = BLOCK_D & (0x00FF);

                int D0 = decodificador_to_3662(D0_);
                int D1 = decodificador_to_3662(D1_);

                lon = todecimal62(D0,D1);
                lon = lon*-1.0/100-68;

                FLAG_LONLAT = true;
                TRAMA_0 = true; 

                //------------------------ guardamos los valores en el buffer -----------------------//

                int idx = return_index(emergency_signal,10);

                BUFF_BLOCK_C_0[idx] = BLOCK_C;
                BUFF_BLOCK_D_0[idx] = BLOCK_D;

                //------------------------------------------------------------------------------------//
           
               
            }

            //  dos bloques 
            if (x==1) // primera trama
            {   
                // radio 
                int C0 = (BLOCK_C & (0xFF00))>> 8;
                int C1 = BLOCK_C & (0x00FF);

                C0 = decodificador_to_3662(C0);
                C1 = decodificador_to_3662(C1);

                radio = todecimal36(C0,C1);

                // identificador
                int D0 = (BLOCK_D & (0xFF00))>> 8;
                int D1 = BLOCK_D & (0x00FF);

                D0 = decodificador_to_3662(D0);
                D1 = decodificador_to_3662(D1);     

                identificador = todecimal36(D0,D1);
                FLAG_LONLAT = true;

                TRAMA_1 = true;


                //------------------------ guardamos los valores en el buffer -----------------------//

                int idx = return_index(emergency_signal,10);

                BUFF_BLOCK_C_1[idx] = BLOCK_C;
                BUFF_BLOCK_D_1[idx] = BLOCK_D;

                //------------------------------------------------------------------------------------//


            }

            if (TRAMA_0 && TRAMA_1)
            {   
                TRAMA_0 = false;
                TRAMA_1 = false;
                //TRAMA TOTAL COMPLETA
                //PROCESAMOS LA SEÑAL DE EMERGENCIA
                //TRAMA_TOTAL = true;

                verify_emergency();
            }
        
        }


      
        if (type==4) //type A
        {
            
            
            type_text = 'A';
            int index = (BLOCK_B)&(0x0F);
            if (!radiotext_A[index*4]&!radiotext_A[index*4+1]&!radiotext_A[index*4+2]&!radiotext_A[index*4+3])
            {
                char C0 = (BLOCK_C & (0xFF00))>> 8;
                char C1 = BLOCK_C & (0x00FF);

                char D0 = (BLOCK_D & (0xFF00))>> 8;
                char D1 = BLOCK_D & (0x00FF);

                radio_textA [index*4]     = C0;
                radio_textA [index*4+1]   = C1;
                radio_textA [index*4+2]   = D0;
                radio_textA [index*4+3]   = D1;

                radiotext_A[index*4]      = true;
                radiotext_A[index*4+1]    = true;
                radiotext_A[index*4+2]    = true;
                radiotext_A[index*4+3]    = true;


                completedTextA++;
                
            }
            
            if (completedTextA>=10){

                FLAG_RADIO_TEXT=true;
                
                // reset 

                for (int ind=0;ind<64;ind++){
                    radiotext_A[ind]=false;
                }

                completedTextA = 0;
                set_point_radiotext=millis() + timeout_radiotext;
            }

            return;
        }

        else if (type==5) 
        {   
            type_text = 'B';
            
            int index = (BLOCK_B)&(0x0F);
            if (!radiotext_B[index*2]&!radiotext_B[index*2+1] )
            { 

                char D0 = (BLOCK_D & (0xFF00))>> 8;
                char D1 = BLOCK_D & (0x00FF);

                radio_textB [index*2]     = D0;
                radio_textB [index*2+1]   = D1; 

                radiotext_B[index*2] = true;
                radiotext_B[index*2+1] = true;

                completedTextB++; 
            }

            if (completedTextB>=8){
                FLAG_RADIO_TEXT = true;

                //RESET
                for (int ind=0;ind<32;ind++){
                    radiotext_B[ind]=false;
                } 

                completedTextB=0;
                set_point_radiotext=millis() + timeout_radiotext;

            }
            return;
        }
        else{
            return;
        }
    }
}


int process_read :: get_pty(void ){
if(FLAG_BLOCKS==true)
{
    uint16_t number = (IDENTIFIER_TYPE <<5) & (BLOCK_B);
    number  = number >> 5;

    return int(number);

}
else {
    return int(100);
}
}