//
//  Signal_Statistics.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 23/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>

double calc_signal_mean (double* sig_src_arr, int sig_lenght){
    double mean = 0.0;                      //Variable que acumula el promedio
    
    for( int i = 0; i < sig_lenght; i++)    //Ciclo for que acumula todos las variables de la senal
    {
        mean += sig_src_arr [i];            //dentro de la variable mean
    }
    
    mean /= (double)sig_lenght;             //se divide mean entre el numero de muestras
    
    return mean;                            //la funcion devuelve el valor mean
}


double calc_signal_std (double* sig_src_arr, double sig_mean, int sig_lenght)
{
    double variance = 0.0;                                      //Variable que acumula cuanto varia una senal del promedio
    double std = 0.0;                                           //Variable que acumula la variacion estandar
    
    for (int i = 0; i < sig_lenght; i++) {
        variance += pow((sig_src_arr[i] - sig_mean), 2);        //variance acumula el cuadrado de la diferncia entre un punto de la senal
    }                                                           //y la media
    
    variance /= (sig_lenght - 1);                               //luego se divide entre el tamano de la senal - 1
    
    std = sqrt(variance);                                       //se saca la raiz cuadrada al acumulado total de la variance
    
    return std;                                                 //se devuelve el valor de la desviacion estandar
}


double calc_signal_rms (double* sig_src_arr, int sig_lenght)
{
    double rms = 0.0;                           //Variable que acumula el RMS
    
    for (int i = 0; i < sig_lenght; i++) {
        
        rms += sig_src_arr[i] * sig_src_arr[i];     //se acumula el cuadrado de cada punto de la senal dentro de
                                                    //la variable rms
    }
    
    rms = sqrt(rms/(double)sig_lenght);             //se divide el valor entre el tamano de la senal y luego se calcula la raiz cuadrada
    
    return rms;                                     //se devuelve el valor del RMS
}


double calc_signal_noise(double* mean, double* std_dev)
{
    double signal_noise_ratio = *mean/ *std_dev;        //El signal to noise ratio es igual al promedio entre la desviacion estandar
    
    return signal_noise_ratio;                          //se devuelve el valor del signal to noise ratio
}


double calc_signal_CV(double* mean, double* std_dev)
{
    double coefficient_variation = (*mean/ *std_dev) * 100;     //El CV es igual al promedio entre la desviacion estandar multiplicado por 100
    
    return coefficient_variation;                               //se devuelve el valor del CV
}







