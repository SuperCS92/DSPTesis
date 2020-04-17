//
//  Robust_Convolution.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 25/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cstdint>

void robust_conv(double * pSrcA,uint32_t srcALen,double * pSrcB,  uint32_t srcBLen,double * pDst)
{
    double *pIn1;                               /* inputA apuntador */
    double *pIn2;                               /* inputB apuntador */
    double *pOut = pDst;                        /* output apuntador */
    double *px;                                 /* Intermedio inputA apuntador */
    double *py;                                 /* Intermedio inputB apuntador */
    double *pSrc1, *pSrc2;                      /* Apuntadores intermedios */
    double sum, acc0, acc1, acc2, acc3;         /* Acumuladores */
    double x0, x1, x2, x3, c0;                  /* Variables temporales */
    uint32_t j, k, count, blkCnt, blockSize1, blockSize2, blockSize3;     /* contadores de loop  */
    
    
    //If ---> srcBLen siempre debe ser el arreglo con menos muestras
    
    if(srcALen >= srcBLen)
    {
                        /* Inicializacion de apuntadores */
        pIn1 = pSrcA;
       
        pIn2 = pSrcB;
    }
    else
    {
        
        pIn1 = pSrcB;
        
        pIn2 = pSrcA;
        
        
        j = srcBLen;
        srcBLen = srcALen;
        srcALen = j;
    }
   
    
    /* conv(x,y) at n = x[n] * y[0] + x[n-1] * y[1] + x[n-2] * y[2] + ...+ x[n-N+1] * y[N -1] */
    /*
     El algoritmo esta dividido en tres etapas, de acuerdo al numero de multiplicaciones
     que hay que hacer entre InputA e InputB. En la primera etapa del algoritmo las multiplicaciones aumentan
     por un factor de 1 en cada iteracion. En la segunda etapa, se realizan srcBLen numero de multiplicaciones.
     En la tercera etapa, las multiplicaciones decrecen en un factor de 1 en cada iteracion
     
     En la primera etapa, la respuesta de impulso  esta empezando a "chocar" con mi senal de entrada
     en funcion del tiempo, por eso, a medida que avanzamos vamos haciendo mas multiplicaciones, porque mas puntos de la respuesta de impulso
     se solapan con la senal de entrada.
     
     En la segunda etapa, ya la respuesta de impulso esta inmersa dentro de la senal de entrada por eso se realizan srcBLen
     cantidad de multiplicaciones.
     
     En la tercera etapa, la senal de entrada se empieza a "alejar" de la respuesta de impulso y por eso, a medida que avanzamos vamos haciendo
     menos multiplicaciones, porque menos puntos de la respuesta de impulso se solapan con la senal de entrada .
     
     */
    
    
    blockSize1 = srcBLen - 1;
    blockSize2 = srcALen - (srcBLen - 1);
    blockSize3 = blockSize1;
    
    
    // 1era Etapa
    
    
    count = 1;
    
    px = pIn1;
    
    py = pIn2;
    
    //Aqui empieza la 1era etapa
    
    while(blockSize1 > 0)
    {
        
        sum = 0.0;      //Acumulador se iguala a 0
        
        k = count >> 2;     //Metodo Loop unrolling
                            //se divide la senal en 4 partes
        
        while(k > 0)
        {
            
            sum += *px++ * *py--;    // x[0] * y[srcBLen - 1]
            
            
            sum += *px++ * *py--;   // x[1] * y[srcBLen - 2]
            
            
            sum += *px++ * *py--;   //x[2] * y[srcBLen - 3]
            
            
            sum += *px++ * *py--;   // x[3] * y[srcBLen - 4]
            
            
            k--;                    // se resta 1 a k
        }
        
        
        /*Si la senal no es multiplo de 4 las muestras restantes se calculan en el siguiente ciclo while
         que no us el metodo loop unrolling, seran maximo 3 operaciones las necesarias*/
        
        
        k = count % 0x4;
        while(k > 0)
        {
            
            sum += *px++ * *py--;
            
            
            k--;
        }
        
        *pOut++ = sum;
        
        
        py = pIn2 + count;
        px = pIn1;
        
        
        count++;
      
        blockSize1--;
    }
    //Aqui empieza la 2da etapa
    
    /* sum = x[0] * y[srcBLen-1] + x[1] * y[srcBLen-2] +...+ x[srcBLen-1] * y[0]
     * sum = x[1] * y[srcBLen-1] + x[2] * y[srcBLen-2] +...+ x[srcBLen] * y[0]
     * ....
     * sum = x[srcALen-srcBLen-2] * y[srcBLen-1] + x[srcALen] * y[srcBLen-2] +...+ x[srcALen-1] * y[0]
     */
    
    px = pIn1; // Apuntador a inputA
    
    
    pSrc2 = pIn2 + (srcBLen - 1); // Apuntador a inputB
    py = pSrc2;
    
    
    count = 0;
    
    
    
    //Etapa 2 depende del tamano de srcBLen
    
    if(srcBLen >= 4)
    {
        
        blkCnt = blockSize2 >> 2; //srcBLen entre 4
        while(blkCnt > 0)
        {
            /* Acumuladores en 0 */
            acc0 = 0.0;
            acc1 = 0.0;
            acc2 = 0.0;
            acc3 = 0.0;
            
            /* lee x[0], x[1], x[2]  */
            
            x0 = *(px++);
            x1 = *(px++);
            x2 = *(px++);
            
            
            k = srcBLen >> 2;
            
            do
            {
                // Lee y[srcBLen - 1]
                c0 = *(py--);
                
                // Lee x[3]
                x3 = *(px);
                
                // Multiplica y luego acumula
                
                // acc0 +=  x[0] * y[srcBLen - 1]
                acc0 += x0 * c0;
                
                // acc1 +=  x[1] * y[srcBLen - 1]
                acc1 += x1 * c0;
                
                // acc2 +=  x[2] * y[srcBLen - 1]
                acc2 += x2 * c0;
                
                // acc3 +=  x[3] * y[srcBLen - 1]
                acc3 += x3 * c0;
                
                // Lee y[srcBLen - 2]
                c0 = *(py--);
                
                // Lee x[4] sample */
                x0 = *(px + 1);
                
                // Multiplica y luego acumula
                // acc0 +=  x[1] * y[srcBLen - 2]
                acc0 += x1 * c0;
                // acc1 +=  x[2] * y[srcBLen - 2]
                acc1 += x2 * c0;
                // acc2 +=  x[3] * y[srcBLen - 2]
                acc2 += x3 * c0;
                // acc3 +=  x[4] * y[srcBLen - 2]
                acc3 += x0 * c0;
                
                //Lee y[srcBLen - 3]
                c0 = *(py--);
                
                //Lee x[5]
                x1 = *(px + 2);
                
                // Multiplica y luego acumula
                // acc0 +=  x[2] * y[srcBLen - 3]
                acc0 += x2 * c0;
                // acc1 +=  x[3] * y[srcBLen - 2]
                acc1 += x3 * c0;
                // acc2 +=  x[4] * y[srcBLen - 2]
                acc2 += x0 * c0;
                // acc3 +=  x[5] * y[srcBLen - 2]
                acc3 += x1 * c0;
                
                //Lee y[srcBLen - 4]
                c0 = *(py--);
                
                //Lee x[6]
                x2 = *(px + 3);
                px += 4;
                
                // Multiplica y luego acumula
                // acc0 +=  x[3] * y[srcBLen - 4]
                acc0 += x3 * c0;
                // acc1 +=  x[4] * y[srcBLen - 4]
                acc1 += x0 * c0;
                // acc2 +=  x[5] * y[srcBLen - 4]
                acc2 += x1 * c0;
                // acc3 +=  x[6] * y[srcBLen - 4]
                acc3 += x2 * c0;
                
                
            } while(--k);
            
            
            
            //si srcBLen no es multiplo de 4, se calculan las muestras faltantes
            //no seran mas de 3 en
            k = srcBLen % 0x4;
            
            while(k > 0)
            {
               
                c0 = *(py--);
                
                
                x3 = *(px++);
                
                // Multiplica y luego acumula
                // acc0 +=  x[4] * y[srcBLen - 5]
                acc0 += x0 * c0;
                // acc1 +=  x[5] * y[srcBLen - 5]
                acc1 += x1 * c0;
                // acc2 +=  x[6] * y[srcBLen - 5]
                acc2 += x2 * c0;
                // acc3 +=  x[7] * y[srcBLen - 5]
                acc3 += x3 * c0;
                
               
                x0 = x1;
                x1 = x2;
                x2 = x3;
                
                
                k--;
            }
            
            //Acumula el resultado en pOut que variable de salida
            *pOut++ = acc0;
            *pOut++ = acc1;
            *pOut++ = acc2;
            *pOut++ = acc3;
            
            
            //incrementa el apuntador pIn1 en un factor de 4
            count += 4;
            
            
            px = pIn1 + count;
            py = pSrc2;
            
            blkCnt--;
        }
        blkCnt = blockSize2 % 0x4;
        
        while(blkCnt > 0)
        {
            
            sum = 0.0f;     //Acumulador se iguala a 0 en cada iteracion
            
            
            k = srcBLen >> 2;   //metodo loop unrolling se computaran 4 valores simultaneamente
            
            
            while(k > 0)
            {
                // Multiplica y luego acumula
                sum += *px++ * *py--;
                sum += *px++ * *py--;
                sum += *px++ * *py--;
                sum += *px++ * *py--;
                
                //decremento de contador
                k--;
            }
            
            //si srcBLen no es multiplo de 4, se calculan las muestras faltantes
            //no seran mas de 3 en
            k = srcBLen % 0x4;
            
            while(k > 0)
            {
               // Multiplica y luego acumula
                sum += *px++ * *py--;
                
                //decremento de contador
                k--;
            }
            
            //Acumula el resultado en pOut que variable de salida
            *pOut++ = sum;
            
            
            count++;
            
            
            px = pIn1 + count;
            py = pSrc2;
            
            //decremento de contador
            blkCnt--;
        }
    }
    else
    {
        
        blkCnt = blockSize2;
        
        while(blkCnt > 0)
        {
            //Acumulador se iguala a 0 en cada iteracion
            sum = 0.0;
            
           
            k = srcBLen;
            
            while(k > 0)
            {
                // Multiplica y luego acumula
                sum += *px++ * *py--;
                
                //decremento de contador
                k--;
            }
            //Acumula el resultado en pOut que variable de salida
            *pOut++ = sum;
            
           //incrementa el contador
            count++;
            
            
            px = pIn1 + count;
            py = pSrc2;
            
        
            blkCnt--;
        }
    }
    
    //Aqui empieza la etapa 3
    
    /* sum += x[srcALen-srcBLen+1] * y[srcBLen-1] + x[srcALen-srcBLen+2] * y[srcBLen-2] +...+ x[srcALen-1] * y[1]
     * sum += x[srcALen-srcBLen+2] * y[srcBLen-1] + x[srcALen-srcBLen+3] * y[srcBLen-2] +...+ x[srcALen-1] * y[2]
     * ....
     * sum +=  x[srcALen-2] * y[srcBLen-1] + x[srcALen-1] * y[srcBLen-2]
     * sum +=  x[srcALen-1] * y[srcBLen-1]
     */
    
   
    
    //En esta etapa el numero de multiplicaciones decrece en factor de 1 por cada iteracion
    
 
    pSrc1 = (pIn1 + srcALen) - (srcBLen - 1);//Apuntador a InputA
    px = pSrc1;
    
  
    pSrc2 = pIn2 + (srcBLen - 1);//Apuntador a InputB
    py = pSrc2;
    
    
    while(blockSize3 > 0)
    {
        //Acumulador se iguala a 0 en cada iteracion
        sum = 0.0;
        
        
        k = blockSize3 >> 2;    //metodo loop unrolling se divide la senal en 4
        
        
        while(k > 0)
        {
            // sum += x[srcALen - srcBLen + 1] * y[srcBLen - 1]
            sum += *px++ * *py--;
            
            // sum += x[srcALen - srcBLen + 2] * y[srcBLen - 2]
            sum += *px++ * *py--;
            
            // sum += x[srcALen - srcBLen + 3] * y[srcBLen - 3]
            sum += *px++ * *py--;
            
            // sum += x[srcALen - srcBLen + 4] * y[srcBLen - 4]
            sum += *px++ * *py--;
            
            // decremento de contador
            k--;
        }
        
        
        //si srcBLen no es multiplo de 4, se calculan las muestras faltantes
        //no seran mas de 3
        
        k = blockSize3 % 0x4;
        
        while(k > 0)
        {
            // Multiplica y luego acumula
            
            // sum +=  x[srcALen-1] * y[srcBLen-1]
            sum += *px++ * *py--;
            
            // decremento de contador
            k--;
        }
        
        //Acumula el resultado en pOut que variable de salida
        *pOut++ = sum;
        
        
        px = ++pSrc1;//Reposiciona los apuntadores
        py = pSrc2;//Reposiciona los apuntadores
        
       
        blockSize3--;//disminuye el contador
    }
}
