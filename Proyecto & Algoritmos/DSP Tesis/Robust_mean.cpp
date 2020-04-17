//
//  Robust_mean.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 23/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>

void robust_mean(double *sig_src_arr,int Blocksize, double *result )
{
    
    double sum = 0.0;           // acumulador
    
    int blkCnt;                 // Loop counter
    
    double in1, in2, in3, in4;
    
    blkCnt = Blocksize / 4;     //Se divide el arreglo en cuatro partes
    
    while (blkCnt>0) {
        
        in1 = *sig_src_arr++;         //in1 = sig_src_arr [1]
        in2 = *sig_src_arr++;         //in2 = sig_src_arr [2]
        in3 = *sig_src_arr++;         //in3 = sig_src_arr [3]
        in4 = *sig_src_arr++;         //in4 = sig_src_arr [4]
        
        sum += in1;                 //sum = sum + sig_src_arr [0]
        sum += in2;                 //sum = sum + sig_src_arr [1]
        sum += in3;                 //sum = sum + sig_src_arr [2]
        sum += in4;                 //sum = sum + sig_src_arr [3]
        
        blkCnt--;                   //blkCnt = bckCnt - 1
    }
    
    
    blkCnt = Blocksize%4;           //BlkCnt es igual a 0 al llegar a esta linea, se iguala al modulo de la senal dividida entre 4
    
    while (blkCnt > 0) {            //Si el modulo es diferente de 0, se activara este ciclo
        sum += *sig_src_arr++;      //para evitar que quede alguna muestra de la senal sin calcular
        blkCnt--;
    }
    
    *result = sum/(double)Blocksize;    //se divide el total acumulado entre la cantidad de muestras esto nos dara el promedio
    
}
