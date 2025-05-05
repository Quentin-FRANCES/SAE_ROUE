//#include "EthernetInterface.h"
#include <stdlib.h>
#include <string.h>
#include "mbed.h"
#include "rtos.h" // need for main thread sleep
#include "html.h" // need for html patch working with web server
#include "bloc_io.h"

#define RADIUS  0.2F // wheel size
#define NBPOLES 8 // magnetic pole number
#define DELTA_T 0.1F // speed measurement counting period

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

DigitalOut led1(LED1); //initialisation des Leds présentes sur le micro-controleur Mbed*/
DigitalOut led2(LED2);
DigitalOut led3(LED3); // blink when can message is sent
DigitalOut led4(LED4); // blink when can message is received 
DigitalOut VALID_PWM(p21); //broche du VALID_PWM sur le PLD ; Actif à 1

//************ local function prototypes *******************

/**************** Read persistent data from text file located on local file system ****************/

/**************** write persitant  data to text file located on local file system ****************/

//************** calibation gaz function needed to record min_gaz and max_gaz value to persistent text file  ******************

// ************top hall counting interrupt needed for speed measurement

//********************** timer interrupt for speed measurement each 100ms  *************************

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
    char cChoix=0;

    //***************************************** web section ********************************************/
    //Init_Web_Server(&CGI_Function); // create and initialize tcp server socket and pass function pointer to local CGI function
    //Thread WebThread(Web_Server_Thread);// create and launch web server thread
    /********* main cgi function used to patch data to the web server thread **********************************/
    Init_Web_Server(&CGI_Function); // create and initialize tcp server socket and pass function pointer to local CGI function
    Thread WebThread(Web_Server_Thread);// create and launch web server thread
    Gen_HtmlCode_From_File("/local/pagecgi2.htm",tab_balise,2);// read and localise ^VARDEF[X] tag in empty html file 
    //******************************************* end web section  ************************************* / 

    pc.printf(" programme scooter mbed \n");

    //********************* can bus section initialisation *******************************************
    //bCan_Active=true;// needed to lauchn CAN thread
    //Thread CanThread(CAN_REC_THREAD);// create and launch can receiver  thread
    //********************* end can bus section *****************************************************

    while(cChoix!='q' and cChoix!='Q'){
        pc.printf("BIENVENUE DANS LA FABULEUSE ROUE (pas roux) ! \n\r");
        pc.printf("Veuillez choisir votre choix : \n\r");
        pc.printf("1 : Lecture poignee de gaz\n\r");
        pc.printf("2 : Lecture courant\n\r");
        pc.printf("3 : Lecture Vbat\n\r");
        pc.printf("4 : Vitesse km/h\n\r");
        pc.printf("5 : Lecture Pe\n\r");
        pc.printf("6 : Offset poignee min\n\r");
        pc.printf("7 : Offset poignee max\n\r");
        pc.printf("8 : Saisir rapport cyclique (entre 0 et 100)\n\r");
        pc.printf("9 : valeur de HALL : \n\r");
        pc.printf("q : quitter\n\r");
    
    /************* multithreading : main thread need to sleep in order to allow web response */
    while (pc.readable()==0){ // determine if char availabler
        Thread::wait(10); // wait 10 until char available on serial input
    } 
    /************* end of main thread sleep  ****************/ 
    
    pc.scanf(" %c",&cChoix);
    switch (cChoix){
        case 'a': 
            break;
        case 'q':
            VALID_PWM.write(0); 
            break;
        case '8':
            int duty_cycle;
            pc.printf("\n\rEntrez la valeur du rapport cyclique :");
            pc.scanf("%d", &duty_cycle);
            if(duty_cycle >= 0 && duty_cycle <= 100){
                //////////////////////////
                /// FAIRE LA CONVERTION///
                //////////////////////////
                MyPLD.write(duty_cycle);
                pc.printf("\n\rPWM mis à jour à : %d", duty_cycle);
                VALID_PWM.write(1);
            }
            else{
                pc.printf("\n\rERROR : La valeur rentree ne correspond pas au valeurs attendues.");
                VALID_PWM.write(0);
            }
            break;
        
        }
    } // end while
    
    //************** thread deinit *********************
        DeInit_Web_Server();
        //bCan_Active=false;
        //CanThread=false;// close can received thread
        pc.printf(" fin programme scooter mbed \n");
} //end main