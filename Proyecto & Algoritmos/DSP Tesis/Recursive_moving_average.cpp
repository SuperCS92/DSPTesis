//
//  Recursive_moving_average.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 05/11/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <math.h>

void recursive_moving_average( double * pSig_src_arr, double *pSig_out_arr, int signal_lenght, int filter_pts)
{
    int i;              //contador
    double acc;         //acumulador
    acc = 0;            //se iguala a 0 el acumulador
    
    for(i = 0; i <filter_pts - 1; i++)      //se calcula el primer valor de la misma manera que el
    {                                       //moving average filter
        acc = acc + pSig_src_arr [ i] ;
    }
    
    pSig_out_arr [(filter_pts- 1 / 2)] = acc / filter_pts;  //se divide el valor acumulado entre la cantidad de puntos
    
    for (i = (int)floor(filter_pts/2); i <(int) (signal_lenght -  floor(filter_pts/2) - 1); i++) {                  //paso recursivo
        acc = acc + pSig_src_arr [ i + ((filter_pts - 1) / 2)] - pSig_src_arr [ i - (int) floor(filter_pts/2)];     //se usa el valor acumulado
                                                                                                                    //y ahora solo se realizan dos operacion
        pSig_out_arr [ i] = acc/filter_pts;                                                                        //se divide el valor acumulado
    }                                                                                                               //entre la cantidad de puntos
}
