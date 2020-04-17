//
//  Robust_rms.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 24/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>


void sig_rms (double *pSig_src_arr, int Blocksize, double *pResult ){
    
    //*pSig_src_arr---- Apuntador a la variable a la senal de entrada
    //Blocksize-------- Entero que representa el tamano de la senal
    //*pResult--------- Apuntador a la variable de la senal de salida
    
    double sum = 0.0;       //Acumulador del cuadrado de todos los valores de la senal
    int blkCnt;             //Contador
    double in;              // valor que almacenara el valor de entrada
    
    
    //blkCnt = Blocksize>>2;
    blkCnt = Blocksize/4;   //se divide el tamano  de la senal en 4
    
    while (blkCnt > 0) {    //   Metodo Loop unrolling
        
                                    //X [0] = 1
        in = *pSig_src_arr++;       //in = X [0]
        sum += in * in;             //sum = X [0] x X [0]
                                    //sum = sum + ( 1 x 1)
        
                                    //X [1] = 0.5
        in = *pSig_src_arr++;       //in = X [1]
        sum += in * in;              //sum = X [1] x X [1]
                                    //sum =sum + ( 0.5 x 0.5)
        
        
        in = *pSig_src_arr++;
        sum += in * in;
        
        in = *pSig_src_arr++;
        sum += in * in;
        
        blkCnt--;                     //Contador menos 1
        
    }
    
    blkCnt = Blocksize%4;       //BlkCnt es igual a 0 al llegar a esta linea, se iguala al modulo de la senal dividida entre 4
    
    while (blkCnt>0) {          //Si el modulo es diferente de 0, se activara este ciclo
        in = *pSig_src_arr++;   //para evitar que quede alguna muestra de la senal sin calcular
        sum += in * in;
        
        blkCnt--;
    }
    
    *pResult = sqrt(sum/(double)Blocksize);// se realiza calcula la raiz cuadrada del acumulador
                                            //dividido entre la cantidad de muestras de la senal
                                            //y se almacena en la variable de salida
    
}
