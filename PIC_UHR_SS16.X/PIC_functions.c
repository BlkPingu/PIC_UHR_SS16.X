/*
 * File:   PIC_functions.c
 * Author: Tobias
 *
 * Created on July 5, 2016, 12:17 PM
 */


#include <xc.h>
#include "globals.h"
#include "mcc_generated_files/mcc.h"
#include "mcc_generated_files/pin_manager.h"

void siebensegdek(unsigned int zahl)
{
    if (DP==0)
    {
    switch (zahl)
        {
            case 0:  LATB=0xC0; //192 = 0  Normale Anzeige
            break;
            case 1:  LATB=0xF9; //249 = 1
            break;
            case 2:  LATB=0xA4; //164 = 2
            break;
            case 3: LATB=0xB0; //176 = 3 
            break;
            case 4: LATB=0x99; //153 = 4 
            break;
            case 5: LATB=0x92; //146 = 5 
            break;
            case 6: LATB=0x82; //130 = 6 
            break;
            case 7: LATB=0xF8; //248 =7  
            break;
            case 8: LATB=0x80; //128 = 8   
            break;
            case 9: LATB=0x90; // 144 = 9 
            break;
            default: 1;
        } 
    }
    if (DP>0) //Neu: Zahl plus Dezimalpunkt
    
    {
        switch (zahl)
        {
            case 0:  LATB=0x40; //192-128  = 64  Die Zahl mit Punkt
            break;
            case 1:  LATB=0x79; //249 -128 = 121
            break;
            case 2:  LATB=0x24; //164 -128 = 36
            break;
            case 3: LATB=0x30; //176 -128 = 48
            break;
            case 4: LATB=0x19; //153 -128 = 25
            break;
            case 5: LATB=0x12; //146 -128 = 18
            break;
            case 6: LATB=0x02; //130 -128 = 2
            break;
            case 7: LATB=0x78; //248 -128 = 120
            break;
            case 8: LATB=0x00; //128 -128= 0  
            break;
            case 9: LATB=0x10; // 144 -128 = 16
            break; 
            default:1;
        }
    }
}
void anzeigewechseln () //wie im PAP
{
    if (mux < 5)
    {
        mux = 5;
    }
    else mux = 1;
}

void digitstellen ()
{        
        
        
    while (tasterpos==1)
    {
        tasterpos = IO_RA4_GetValue() ;
        
        tasterzahl= IO_RA5_GetValue() ;
        
        
        if (tasterzahl == 0)  //entpuffern der Schalter
        { __delay_ms(100);
            do
                {
                    tasterzahl=IO_RA5_GetValue() ;
                    __delay_ms(100); 
                }
            while (tasterzahl==0);
                

               
            switch (digit) //digitstellen aus dem PAP - erklärung siehe Protokoll
            {
                case 1: ME=ME+1; 
                    if (ME>9) {ME=0;}
                    
                break;

                case 2: MZ=MZ+1;
                    if (MZ>5) {MZ=0;}
                    
                break;

                case 3: HE=HE+1;
                    if ((HZ>1)&&(HE>3)) {HE=0;}
                    else if (HE>9) {HE=0;}
                    
                break;    
                case 4: HZ=HZ+1;
                    if ((HE>3 && HZ >1)|| (HZ>2))
                        { 
                        HZ = 0;
                    }
                break;
            }
        }
    } 
    tasterzahl = IO_RA5_GetValue();  //erneutes entpuffern des Schalters
    if (tasterpos==0)
            {                    
            __delay_ms(100);
                do
                {
                    tasterpos=IO_RA4_GetValue();
                    __delay_ms(100);

                }
            while(tasterpos ==0);
            }
}


void uhrstellen ()
{
    mux=1;
    digit = 1;
    digitstellen ();
    digit = 2;
    digitstellen ();
    digit = 3;
    digitstellen ();
    digit = 4;
    digitstellen ();
    SE=0;
    SZ=0;
    digit=0; //neu: Wir setzen digit =  0 damit es nicht wieder zurück zu den Minute einern springt
}

void uhrzeitanzeigen (void)
{
    
    IO_RA0_SetHigh();
    IO_RA1_SetHigh();
    IO_RA2_SetHigh();
    IO_RA3_SetHigh();
    
    switch(mux)  //alle 6 Faelle wie im Pap, plus LOW setzen der RA0-RA3 nach jedem Fall
    {
        case 1: 
            
                zahl = ME;
                siebensegdek(zahl);
                if (digit ==1) {IO_RB7_SetLow();}
                IO_RA0_SetLow();
                mux=2;

          
            break;


        case 2:
            
                zahl = MZ;
                siebensegdek(zahl);
                if (digit ==2) {IO_RB7_SetLow();}
                IO_RA1_SetLow();
                mux=3;
            
            break;

        case 3:
            
                zahl = HE;
                siebensegdek(zahl);
                if (digit ==3) {IO_RB7_SetLow();}
                IO_RA2_SetLow();
                mux=4;
               
            
            break;

         case 4:
            
                zahl = HZ;
                siebensegdek(zahl);
                if (digit ==4) {IO_RB7_SetLow();}
                IO_RA3_SetLow();
                mux=1;
            
            break;
        case 5:
            
                zahl = SE;
                siebensegdek(zahl);
                IO_RA0_SetLow();
                mux=6;
            
            break;
        case 6:
            
                zahl = SZ;
                siebensegdek(zahl);
                IO_RA1_SetLow();
                mux=5;
            
            break;

        default: 
            break;
    }
}

void uhrzweivier () //Uhr 24 wie im PAP
{
    if (SE>9) 
    {
        SE=0;
        SZ=SZ+1;
        if (SZ>5) 
        {
            SZ=0;
            ME=ME+1;
            if (ME>9)
            {
                ME=0;
                MZ=MZ+1;
                if (MZ>5)
                {
                    MZ=0;
                    HE=HE+1;
                    if ((HZ<2)&&(HE>9))
                    {
                        HE=0;
                        HZ=HZ+1;
                    }
                    else if (HE>3)
                    {
                        HE=0;
                        HZ=0;
                    }  
                }
            }
        }
    }
}