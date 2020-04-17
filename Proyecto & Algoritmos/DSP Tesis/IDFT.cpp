//
//  IDFT.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 29/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>

//esta funcion va a tomar la parte real y la parte imaginaria de una senal en el dominio frecuencial y
//y va a entregar la misma senal en el dominio del tiempo,
//1 param: Apuntador al arreglo de salida
//2 param: Apuntador al arreglo de entrada, con la parte real de la senal en el dominio frecuencial
//3 param: Apuntador al arreglo de entrada, con la parte imaginaria de la senal en el dominio frecuencial
//4 param: El tamano de la senal en el dominio del tiempo

//EL tamano del arreglo con la parte real de la senal en el dominio frecuencial mas el tamano del arreglo con la parte
//imaginaria en el dominio frecuencial resultan en el tamano de la senal en el dominio del tiempo

void calc_idft (double *idft_out_arr, double *sig_src_rex_arr, double *sig_src_imx_arr, int idft_lenght)
{
    int i, k;           //contadores
    
    
    
    //Inicializa el arreglo de salida en 0
    for (k = 0; k < idft_lenght; k++) {
        idft_out_arr[k] = 0;
    }
    
    
                //          IDFT         //
    
    
    for (k= 0; k<idft_lenght/2; k++) {
        
        for (i = 0; i < idft_lenght; i++) {
            
            //      Manejo del caso especial donde     //  cuadno el valor dentro del coseno y seno puede ser 0
            
            if (i== 0 || i== idft_lenght) {
                idft_out_arr [i] =  idft_out_arr [i] + (sig_src_rex_arr[k] /(idft_lenght)) * cos(2 * M_PI * k * i/idft_lenght);
                idft_out_arr [i] =  idft_out_arr [i] + (sig_src_imx_arr[k] /(idft_lenght)) * sin(2 * M_PI * k * i/idft_lenght);
                
            }
            
            
            
            //se almacena el valor de la parte real (multiplicado por el coseno) en el arreglo de salida,
            //y luego se almacena el valor de la parte imaginaria (multiplicado por el seno)
        
            
            idft_out_arr [i] =  idft_out_arr [i] + (sig_src_rex_arr[k] /(idft_lenght/2)) * cos(2 * M_PI * k * i/idft_lenght);
            idft_out_arr [i] =  idft_out_arr [i] + (sig_src_imx_arr[k] /(idft_lenght/2)) * sin(2 * M_PI * k * i/idft_lenght);
        }
    }
    
    
    
}
