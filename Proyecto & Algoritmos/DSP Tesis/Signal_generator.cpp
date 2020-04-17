//
//  Signal_generator.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 15/11/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>

//Generador de senoidales de un tamano determinado
//Generador de un ciclo de una senoidal de una frecuencia especifica
//Sintesis de dos senoidales


//funcion generadora de senales
//toma un array de salida y almacena una senoidal de frecuencia (frecuency)

void sig_gen (double * output_sig,int sig_lenght, int frecuency )
{
    int pi =3.14159265359;
    
    for (int t = 0; t <sig_lenght; t++) {
        output_sig[t] = sin(2 * pi  * frecuency * t );
    }
    
    
}


//funcion que sintetiza dos formas de onda del mismo tamano

void sintesis(double * output_sig,int sig_lenght, double *inputA, double *inputB )
{
    
    for (int t = 0; t <sig_lenght; t++) {
        output_sig [t]= inputA[t] + inputB[t];
    }
}


void cicle_sig_gen(int frecuency)
{
    
}
