//
//  Moving_average_filter.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 05/11/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <math.h>




void moving_average( double * pSig_src_arr, double *pSig_out_arr, int signal_lenght, int filter_pts)
{
    //* pSig_src_arr---- Apuntador a la variable a la senal de entrada
    //*pSig_out_arr---- Apuntador a la variable a la senal de salida
    //signal_lenght--- Entero que representa el tamano de la senal
    //filter_pts---cantidad de puntos que acumulara el filtro

    int i, j; //contadores
    
    for(i =(int) floor(filter_pts/2) ; i <(int) (signal_lenght - floor(filter_pts/2) - 1); i++  )
    {
        pSig_out_arr [ i ] = 0;         //inicializacion del arreglo de salida en 0
        
        for ( j =(int) -floor(filter_pts/2); j < (int) floor(filter_pts/2); j++) {
            pSig_out_arr [i] =  pSig_out_arr [i]+  pSig_src_arr [ i + j];// suma los puntos de la senal de acuerdo al numero de filter_pts
                                                                        //si filter_pts = 3
                                                                //pSig_out_arr [0] =  pSig_src_arr [0] +  pSig_src_arr [1] +  pSig_src_arr [2]
        }
        pSig_out_arr [i] = pSig_out_arr [ i]/filter_pts;        //pSig_out_arr [0] = pSig_out_arr [0] / 3
    }
}
