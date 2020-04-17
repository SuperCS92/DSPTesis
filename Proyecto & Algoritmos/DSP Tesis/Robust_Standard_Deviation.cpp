//
//  Robust_Standard_Deviation.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 24/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>


void Sig_std (double *pSig_src_arr, int Blocksize, double *pResult )
{
    //*pSig_src_arr---- Apuntador a la variable a la senal de entrada
    //Blocksize-------- Entero que representa el tamano de la senal
    //*pResult--------- Apuntador a la variable de la senal de salida
    
    double sum = 0.0;       //Acumulador de todos los valores de la senal
    double sumofSquare = 0.0;//Acumulador del cuadrado de todos los valores de la senal
    double in;              // valor que almacenara el valor de entrada
    
    
    int blkCnt;             //Contador
    
    double meanOfSquare, mean, squareOfMean;
    //double SquareOfSum = 0.0;
    //double var;
    
    if(Blocksize == 1){
        *pResult = 0;
        return;
    }
    
    //blkCnt = Blocksize>>2;//
    blkCnt = Blocksize/4;       //se divide el tamano  de la senal en 4
    
    while (blkCnt>0) {
        
                                //X [0] = 1
        in = *pSig_src_arr++;   //in = X [0]
        sum += in;              //sum = sum + X [0]
        sumofSquare += in * in; //sumofSquare = X [0] x X [0]
                                //sumofSquare = sum + ( 1 x 1)
        
                                    //X [1] = 0.5
        in = *pSig_src_arr++;       //in = X [1]
        sum += in;                  //sum = sum + X [1]
        sumofSquare += in * in;     //sumofSquare = sumofSquare +  ( X[1] x X[1] )
                                    //sumofSquare = sumofSquare + ( 0.5 x 0.5)
        
        in = *pSig_src_arr++;
        sum += in;
        sumofSquare += in * in;
        
        in = *pSig_src_arr++;
        sum += in;
        sumofSquare += in * in;
        
        blkCnt--;           //Contador menos 1
    }
    
    //Lo ideal es que el arreglo siempre sea de un tamano divisible entre 4
    //para que esta parte del codigo nunca se ejecute
    
    blkCnt = Blocksize%4;   //BlkCnt es igual a 0 al llegar a esta linea, se iguala al modulo de la senal dividida entre 4
                            //Si la senal es multiplo de 4, blkCnt seguira siendo 0 y el bloque while siguiente sera ignorado
    
    while (blkCnt>0) {
        in = *pSig_src_arr++;
        sum += in;
        sumofSquare += in * in;
        
        blkCnt--;
    }
    
    meanOfSquare = sumofSquare / ((double)Blocksize - 1.0); //se saca el promedio de la suma de los cuadrados
    
    mean = sum /(double)Blocksize;                          //se saca el promedio  de la suma de la senales
    
    squareOfMean = (mean * mean ) * ((double)Blocksize/ (double)Blocksize - 1.0);// se saca el cuadrado del promedio
    
    *pResult = sqrt((meanOfSquare - squareOfMean));     // se calcula la raiz de la diferencia entre  meanOfSquare y squareOfMean
    
}
