//
//  DFT.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 28/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>


void calc_sig_dft (
                   double *pSrc_arr,                //apuntador a la senal de entrada
                   double *sig_dest_rex_arr,        //apuntador a la parte real de la senal
                   double *sig_dest_imx_arr,         //apuntador a la parte imaginaria de la senal
                   int sig_lenght)                  //Tamano de la senal
{
    int i,j,k;          //contadores
    
    //se inicializan ambos arreglos en 0//
    
    for ( j = 0; j<(sig_lenght / 2) + 1; j++) {
        sig_dest_rex_arr [j] = 0;
        sig_dest_imx_arr [j] = 0;
    }
    
    
    // // DFT //  //
    
    for(k = 0; k < (sig_lenght / 2) + 1; k++)                                                               // Para cada frecuencia
    {
        for (i = 0; i <sig_lenght; i++) {                                                                   //Calculamos la contribucion de todos los
            sig_dest_rex_arr[k] = sig_dest_rex_arr[k] + pSrc_arr [i] * cos(2 * M_PI * k * i/sig_lenght );
            sig_dest_imx_arr[k] = sig_dest_imx_arr[k] - pSrc_arr [i] * sin(2 * M_PI * k * i/sig_lenght);    //puntos en el dominio del tiempo
        }
    }
    
}



void get_dft_output (double *sig_dest_mag_arr, double *sig_src_rex_arr, double *sig_src_imx_arr, int sig_lenght)
{
    int x;
    for (x = 0; x < sig_lenght/2; x++) {
        sig_dest_mag_arr [x] = sqrt(pow(sig_src_rex_arr[x], 2) + pow(sig_src_imx_arr [x], 2 ));
    }
}
