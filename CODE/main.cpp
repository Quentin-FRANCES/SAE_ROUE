//#include "EthernetInterface.h"
#include <stdlib.h>
#include <string.h>
#include "mbed.h"
#include "rtos.h" // need for main thread sleep
#include "html.h" // need for html patch working with web server
#include "bloc_io.h"

#define RADIUS  0.2F // wheel size
#define NBPOLES 8.0F // magnetic pole number
#define DELTA_T 0.1 // speed measurement counting period
#define PI 3.14159265358979F


Bloc_IO MyPLD(p25,p26,p5,p6,p7,p8,p9,p10,p23,p24);// instantiate object needed to communicate with PLD
    // analog input connected to mbed 
    // valid pmw mbed pin
Serial pc(USBTX, USBRX); // tx, rx
    // Top_Hall Pin


/************ persistent file parameters section *****************/
LocalFileSystem local("local");               // Create the local filesystem under the name "local"
    
/********************* web server section **********************************/
var_field_t tab_balise[10];  //une balise est présente dans le squelette
int giCounter=0;// acces counting

/*********************** can bus section  ************/
         // determine message ID used to send Gaz ref over can bus
#define _CAN_DEBUG // used to debug can bus activity
//#define USE_CAN_REF // uncomment to receive gaz ref over can_bus
CAN can_port (p30, p29); // initialisation du Bus CAN sur les broches 30 (rd) et 29(td) for lpc1768 + mbed shield
bool bCan_Active=false;
bool gaz_calibre = false, controle_poignee = false;
int cptHALLTop = 0;
float vitesse = 0, vitesse_km_h = 0;
float min_gaz, max_gaz, val_gaz, DC_gaz;

DigitalOut led1(LED1); //initialisation des Leds présentes sur le micro-controleur Mbed*/
DigitalOut led2(LED2);
DigitalOut led3(LED3); // blink when can message is sent
DigitalOut led4(LED4); // blink when can message is received 
DigitalOut VALID_PWM(p21); //broche du VALID_PWM sur le PLD ; Actif à 1
InterruptIn VALID_HALL(p22); //broche du VALID_HALL -> Un top à chaque changement de secteur
AnalogIn V_GAZ(p17);

//************ local function prototypes *******************
void init(void);
void echantillonage_100ms(void);
void cptHALL(void);
//void set_speed(int);
/**************** Read persistent data from text file located on local file system ****************/

/**************** write persitant  data to text file located on local file system ****************/

//************** calibation gaz function needed to record min_gaz and max_gaz value to persistent text file  ******************

// ************top hall counting interrupt needed for speed measurement

//********************** timer interrupt for speed measurement each 100ms  *************************
Ticker tic_speed;
//********************* Timer Interrupt for gaz ref management each 10ms   ********************

/********* main cgi function used to patch data to the web server thread **********************************/
void CGI_Function(void){ // cgi function that patch web data to empty web page
    char ma_chaine4[20]={};// needed to form html response  

    sprintf (ma_chaine4,"%d",giCounter);// convert speed as ascii string
    Html_Patch (tab_balise,0,ma_chaine4);// patch first label with dyn.string
    sprintf (ma_chaine4,"%d",2*giCounter);// convert speed as ascii string
    Html_Patch (tab_balise,1,ma_chaine4);// patch first label with dyn.string

    giCounter=giCounter+2; 
}
    
/*********************** CAN BUS SECTION  **********************/
    


void CAN_REC_THREAD(void const *args){
    int iCount,iError;

    while (bCan_Active){
    Thread::wait(100);// wait 100ms  
    // code todo
    }
}
       
//*************************** main function *****************************************
int main() {
    init();
    char cChoix=0;
    int readPLD, HALL, direction, over_current, FLTA, BRAKE;
    //***************************************** web section ********************************************/
    //Init_Web_Server(&CGI_Function); // create and initialize tcp server socket and pass function pointer to local CGI function
    //Thread WebThread(Web_Server_Thread);// create and launch web server thread
    /********* main cgi function used to patch data to the web server thread **********************************/
    //Init_Web_Server(&CGI_Function); // create and initialize tcp server socket and pass function pointer to local CGI function
    //Thread WebThread(Web_Server_Thread);// create and launch web server thread
    //Gen_HtmlCode_From_File("/local/pagecgi2.htm",tab_balise,2);// read and localise ^VARDEF[X] tag in empty html file 
    //******************************************* end web section  ************************************* / 

    //pc.printf(" programme scooter mbed \n");

    //********************* can bus section initialisation *******************************************
    //bCan_Active=true;// needed to lauchn CAN thread
    //Thread CanThread(CAN_REC_THREAD);// create and launch can receiver  thread
    //********************* end can bus section *****************************************************

    while(cChoix!='q' and cChoix!='Q'){
        pc.printf("\n\n\rBIENVENUE DANS LA FABULEUSE GRANDE ROUE (pas roux) !");
        pc.printf("\n\rVeuillez choisir votre choix : ");
        pc.printf("\n\r1 : Saisir rapport cyclique (entre 0 et 255)");
        pc.printf("\n\r2 : Etat PLD");
        pc.printf("\n\r3 : Mesure de vitesse");
        pc.printf("\n\r4 : Calibrage poignee gaz");
        pc.printf("\n\r5 : Controle de la vitesse (poignee ou IHM)");
        pc.printf("\n\rq : quitter\n\r");
        if(gaz_calibre){
            pc.printf("\n\rPoignee OK");
        }
        else{
            pc.printf("\n\r POIGNEE NON CALIBREE");
        }
        if(controle_poignee){
            pc.printf("\n\rLa vitesse est controlee par la poignee");
        }
        else{
            pc.printf("\n\rLa vitesse est controlee par l'IHM");
        }
    
    /************* multithreading : main thread need to sleep in order to allow web response */
    while (pc.readable()==0){ // determine if char availabler
        Thread::wait(10); // wait 10 until char available on serial input
    } 
    /************* end of main thread sleep  ****************/ 
    
    pc.scanf(" %c",&cChoix);
    switch (cChoix){
        case 'q':
            VALID_PWM.write(0); 
            MyPLD.write(0);
            break;

        case '1':
            int duty_cycle;
            pc.printf("\n\rEntrez la valeur du rapport cyclique :");
            pc.scanf("%d", &duty_cycle);
            if(duty_cycle >= 0 && duty_cycle <= 255){
                MyPLD.write(duty_cycle);
                VALID_PWM.write(1);
                pc.printf("\n\rPWM mis a jour a : %d", duty_cycle);
            }
            else{
                pc.printf("\n\rERROR : La valeur rentree ne correspond pas au valeurs attendues.");
            }
            break;

        case '2':
            readPLD = MyPLD.read();
            HALL = 0x07 & readPLD;
            direction = 0x01 & (readPLD >> 3);
            over_current = 0x01 & (readPLD >> 6);
            FLTA = 0x01 & (readPLD >> 4);
            BRAKE = 0x01 & (readPLD >> 5);
            pc.printf("\n\rValeur secteur %d", HALL);
            pc.printf("\n\rDirection : ");
            if(direction == 1){ 
                pc.printf("marche avant");
            }
            else{ 
                pc.printf("marche arriere");
            }
            if(over_current == 0){
                pc.printf("\n\r! OVER CURRENT !");
            }
            else{
                pc.printf("\n\rCourant normal.");
            }
            if(FLTA == 1){
                pc.printf("\n\rERROR : FLTA");
            }
            else{ 
                pc.printf("\n\rPas d'erreur");
            }
            if(BRAKE == 0){
                pc.printf("\n\rHO ca freine !");
            }
            else{ 
                pc.printf("\n\rHO ca freine pas !");
            }
            pc.printf("\n\rval poignee de gaz : %d", (int)DC_gaz);
            break;

        case '3':
            pc.printf("\n\rVitesse : %d km/h", (int)vitesse_km_h);
            break;
            
        case '4':
            int a;
            pc.printf("\n\r-----Calibrage poignee-----");
            pc.printf("\n\rMettez les gaz a 0 et appuyez sur une touche");
            pc.scanf("%d", &a);
            min_gaz = V_GAZ.read();
            pc.printf("\n\rMettez a present les gaz a fond et appuyez sur une touche");
            pc.scanf("%d", &a);
            max_gaz = V_GAZ.read();
            if(max_gaz > min_gaz){
                gaz_calibre = true;
                pc.printf("\n\r-----Poignee calibree-----");
            }
            else{
                gaz_calibre = false;
                pc.printf("\n\r-----ERROR : reessayez-----");
            }
            //pc.printf("\n\rmin : %lg", min_gaz);
            //pc.printf("\n\rmax : %lg", max_gaz);
            break;
        
        case '5':
            pc.printf("\n\rPas encore disponible");
            break;
        }

    } // end while
    
    //************** thread deinit *********************
        //DeInit_Web_Server();
        //bCan_Active=false;
        //CanThread=false;// close can received thread
        pc.printf(" fin programme scooter mbed \n");
} //end main


void init(){
    VALID_PWM.write(0);
    MyPLD.write(0);
    VALID_PWM.write(1);
    VALID_HALL.rise(&cptHALL);
    tic_speed.attach(&echantillonage_100ms, DELTA_T);
}

void echantillonage_100ms(){
    vitesse = (cptHALLTop*2*PI*RADIUS)/(DELTA_T*6*NBPOLES);
    vitesse_km_h = vitesse * 3.6;
    cptHALLTop = 0;
    if(gaz_calibre){
        DC_gaz = (val_gaz - min_gaz) / (max_gaz - min_gaz);
        if(DC_gaz < 0) DC_gaz = 0;
        else if(DC_gaz > 1) DC_gaz = 1;
        DC_gaz = DC_gaz * 255;
    }
}

void cptHALL(){
    cptHALLTop++;
}
