

#include "Arduino.h"
#include "string.h"


#ifndef myfunction_h
#define myfunction_h

class process_read
{
    private:
        const int IDENTIFIER_TYPE=31;
        
        bool completed[4] = {false, false, false, false};
       
        bool radiotext_B[32]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
        bool radiotext_A[64]={false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false,false};
        

        //--------------------------- update variables for detect event -------------------------------------------------//
        bool emergency_signal[10]   = {false,false,false,false,false,false,false,false,false,false};
        float latitud_array [10]    = {0,0,0,0,0,0,0,0,0,0};
        float longitud_array [10]   = {0,0,0,0,0,0,0,0,0,0};
        float radio_array [10]      = {0,0,0,0,0,0,0,0,0,0};
        
        uint16_t BUFF_BLOCK_C_0[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
        uint16_t BUFF_BLOCK_C_1[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

        uint16_t BUFF_BLOCK_D_0[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
        uint16_t BUFF_BLOCK_D_1[10] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

        bool TRAMA_0 = false;
        bool TRAMA_1 = false;

        unsigned long timeout_emergency = 0;
        float MAX_SECONDS_BETWEEN_EMERGENCY = 1.5;
        
        
    
        //--------------------------------------------------------------------------------------------------------------//
        int completedCount = 0;
 
        int countFailedRDS = 0;
        int countSucessRDS = 0;

       //milliseconds
       //change is more precise 
        long timeout = 1300;
        long timeout_radiotext = 45000;
        long second = 1000;

        long set_point=millis() + timeout;
        long set_point_radiotext=millis() + timeout_radiotext;
        long set_point_RDS= millis()+second*3;

        const float DEFINE_RDS_PER_SECOND = 9;


    public:
        char type_text  ='C';
        char buffer_ [16];
        char radio_textA[64];
        char radio_textB[32];

        bool  FLAG_MS_NEW = false;

        int BLOCKA =0;
        int BLOCKB =1;
        int BLOCKC =2;
        int BLOCKD =3;

        int minutes = 0;
   
        int seconds = 0;

        long ums = -1;


        double lat = 0;
        double lon = 0;
        double radio = 0 ;

        int completedTextA = 0;
        int completedTextB = 0;

        float signal_RDS=0;

        long ms_total=-1;

        long min1; long sec1;long ms0; long ms1; long mseconds;

        int secuencia_total= -1;
        
        bool FLAG_BLOCKS =false;
        bool FLAG_NAME   =false;
        bool FLAG_RADIO_TEXT = false;
        bool FLAG_RDS_STRENGTH = false;
        bool FLAG_AFTER = false;

        bool FLAG_LONLAT = false;
        bool FLAG_HOUR = false;
        bool FLAG_MS = false;
        bool FLAG_DL = false;
        
        uint16_t BLOCK_A;
        uint16_t BLOCK_B;
        uint16_t BLOCK_C;
        uint16_t BLOCK_D;

        int identificador;


        int secuencia0 = -1;
        int secuencia1 = -1;
        int usecuencia = -10;

        //-----------------------------------------
        // functions
        void set_block(uint16_t* block);
  
        int get_type(void);
        void get_name (void);

        void check_timeout(bool flag);

        void reset(void);

        void check_RDS(void);
        void get_radiotext(void);

        int get_pty(void);
        

        //-------------------- Method that verify event -----------------------------//
        void verify_emergency(void);
        void reset_emergency(void);
        void get_error_bit_per_bit(float array[]);

        double latitud_emergency;
        double longitud_emergency;
        double radio_emergency;

        bool EMERGENCY_SIGNAL = false;

};

#endif