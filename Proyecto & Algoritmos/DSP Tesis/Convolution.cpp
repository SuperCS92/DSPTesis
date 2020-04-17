//
//  Convolution.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 25/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>


void convolution (double *sig_src_arr,          //Apuntador a la senal de entrada
                  double *sig_dest_arr,         //Apuntador a la senal de salida
                  double *imp_resp_arr,         //Apuntador a la respuesta de impulso
                  int src_sig_lght,             //Tamano de la senal de entrada
                  int imp_resp_lght             //Tamano de la repuesta de impulso
                  ){
    
    //Se inicializa el arreglo de destino con todos sus valores en 0
    int i, j ;
    for(int i = 0; i < (src_sig_lght + imp_resp_lght); i++)
    {
        sig_dest_arr [i] = 0;
    }
    
    
    //algoritmo de convolución
    for (i = 0; i < src_sig_lght; i++) {                                 //ciclo for del tamano de la senal de entrada
        for (j = 0; j < imp_resp_lght; j++) {                            //ciclo for del tamano de la respuesta de impulso
            sig_dest_arr [i + j] += sig_src_arr[i] * imp_resp_arr[j];    //Todos los puntos de la respuesta de impulso se multiplican por un
        }                                                                // punto de la senal de entrada y se acumulan en un punto de la
    }                                                                    //senal de salida
    
    
    
}
