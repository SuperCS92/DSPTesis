//
//  FFT.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 10/11/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include <iostream>

using namespace std;

unsigned int bit_Reversal(int y, int sig_l );


//      FFT      //
//      REFERENCIA: DSPGUIDE---> CH 12---> SECTION 3    //
//      ALGORITMO TRANSCRITOD DEL LENGUAJE BASIC        //
//      BIT REVERSAL SORTING TOMADO DE https://www.oreilly.com/library/view/c-cookbook/0596007612/ch11s18.html//

//FALTA REVISAR SI ES NECESARIO REVERTIR EL BIT REVERSAL SORTING
//EN CUYO CASO DEBERIA BASTAR CON REPETIR EL CICLO FOR 


void FFT ( double* ReX, double* ImX, int Sig_L)
{
    
    //Declaracion de variables//
    
    int Pi =3.14129265;        //Revisar que sea el numeto Pi correcto
    int NM1 = Sig_L - 1;
    //int ND2 = Sig_L / 2;
    int m = log((double) Sig_L ) / log(2);
    //int j = ND2;
    //int k = ND2;
    
    
    cout<<"----------  m :"<< m<<endl;
    //Bit reversal sorting//
    for ( int  i = 1; i < Sig_L/2; i++)
    {
        double TR, TI; //acumulador temporal de la variable real
        int a;
        
        a = bit_Reversal(i, Sig_L); //a es el numero i invertido
        
        TR = ReX[i];        //Acumula temporalmente la variable
        TI = ImX[i];        //Acumula temporalmente la variable
        
        ReX[i]= ReX[a];     //intercambio de posicion de la parte real
        ReX[a] = TR;        //intercambio de posicion de la parte real
        
        ImX[i] = ImX[a];    //intercambio de posicion de la parte imaginaria
        ImX[a] = TI;        //intercambio de posicion de la parte imaginaria
    }
    
    
    // FFT
    for (int  L = 1; L < m; L++) {
        int LE = pow(2, L);
        int LE2 = LE/2;
        double ur = 1;
        double ui = 0;
        double sr = cos (Pi/ LE2);                      //calculo del valor del coseno
        double si = -sin(Pi/ LE2);                      //calculo del valor del seno
        double tr, ti;
        
        for (int j = 1; j < LE2; j++) {
            int jm1 = j - 1 ;
            for (int i = jm1; i < NM1; i += LE) {
                int ip = i + LE2;
                tr = ReX[ip] * ur - ImX[ip] * ui;       //Butterfly calculations
                ti = ReX[ip] * ui - ImX[ip]* ur;          //Butterfly calculations
                
                ReX[ip] = ReX[i] - tr;
                ImX[ip] = ImX[i] - ti;
                
                ReX[i] = ReX[i] + tr;
                ImX[i] = ImX[i] + ti;
            }
            
            tr = ur;
            ur = tr * sr - ui*si;
            ui = tr * si + ui*sr;
        }
        
        
        
    }
    
}



unsigned int bit_Reversal(int y, int sig_l ){
    int n = 0;                      //Output
    int t = log2((double) sig_l);   //sig_l debe ser potencia de 2
    
    //int y variable a invertir
    int x = y;
    
    for (int i = 0; i < t; ++i) {
        
        n <<= 1;    // Mutilplicacion por 2 de n
        
        /*
         Determina si el numero es par (0) o impar(1)
         (x & 1) == 0 PAR
         (x & 1) ==1 IMPAR
         */
        
        n |= (x & 1);
        
        x >>= 1;      //division entre dos
        
    }
    return n;       // n es el numero invertido
}

