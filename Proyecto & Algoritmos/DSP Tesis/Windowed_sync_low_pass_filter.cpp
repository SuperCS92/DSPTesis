//
//  Windowed_sync_low_pass_filter.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 07/11/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <stdio.h>
#include <cmath>
#include <cstdint>
    


void windowed_sync_low_pass (long double *filtr_kernel_dest_arr, double cutoff_freq, int filtr_lght)
{
    for (int i = 0; i < filtr_lght; i++)
    {
        if(i-filtr_lght/2==0)
        {
            filtr_kernel_dest_arr [i] = 2*M_PI*cutoff_freq;
        }
        
        
        if (i - filtr_lght/2 !=0) {
            filtr_kernel_dest_arr[i] = sin(2*M_PI*cutoff_freq* (i - filtr_lght/2))/(i - filtr_lght/2);
            //fltr_kernel_dest_arr[i] = sin(2*M_PI*cutoff_freq*(i - filter_length/2))/(i-filter_length/2);
            
            filtr_kernel_dest_arr [ i] = filtr_kernel_dest_arr [ i] * (0.54-0.46* cos( 2 * M_PI * i/ filtr_lght));
            //fltr_kernel_dest_arr[i] = fltr_kernel_dest_arr[i]     * (0.54-0.46* cos( 2 * M_PI *   i/filter_length));
        }
        
    }
}


void windowed_convolution(
                 double *sig_src_arr,
                 double *sig_dest_arr,
                 long double *imp_response_arr,
                 int  sig_src_length,
                 int  imp_response_length
                 )
{
    
    int i,j;
    for(i =0;i<(sig_src_length+ imp_response_length);i++)
    {
        
        sig_dest_arr[i] = 0;
    }
    
    for(i =0;i<sig_src_length;i++)
    {
        
        for(j =0;j<imp_response_length;j++)
        {
            sig_dest_arr[i+j] =  sig_dest_arr[i+j] + sig_src_arr[i]*imp_response_arr[j];
            
        }
    }
}






void windowed_robust_conv( long double * pSrcA,uint32_t srcALen, long double * pSrcB,  uint32_t srcBLen,long double * pDst)
{
    long double *pIn1;
    long double *pIn2;
    long double *pOut = pDst;
    long double *px;
    long double *py;
    long double *pSrc1, *pSrc2;
    double sum, acc0, acc1, acc2, acc3;
    double x0, x1, x2, x3, c0;
    uint32_t j, k, count, blkCnt, blockSize1, blockSize2, blockSize3;
    
    if(srcALen >= srcBLen)
    {
        
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
    
    blockSize1 = srcBLen - 1;
    blockSize2 = srcALen - (srcBLen - 1);
    blockSize3 = blockSize1;
    
    
    count = 1;
    
    px = pIn1;
    
    py = pIn2;
    
    while(blockSize1 > 0)
    {
        
        sum = 0.0;
        
        k = count >> 2;
        
        while(k > 0)
        {
            
            sum += *px++ * *py--;
            
            
            sum += *px++ * *py--;
            
            
            sum += *px++ * *py--;
            
            
            sum += *px++ * *py--;
            
           
            k--;
        }
        
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
    
    px = pIn1;
    
    
    pSrc2 = pIn2 + (srcBLen - 1);
    py = pSrc2;
    
    
    count = 0;
    
    if(srcBLen >= 4)
    {
        
        blkCnt = blockSize2 >> 2;
        while(blkCnt > 0)
        {
            
            acc0 = 0.0;
            acc1 = 0.0;
            acc2 = 0.0;
            acc3 = 0.0;
            
            
            x0 = *(px++);
            x1 = *(px++);
            x2 = *(px++);
            
            
            k = srcBLen >> 2;
            
            do
            {
                
                c0 = *(py--);
                
                
                x3 = *(px);
                
                
                acc0 += x0 * c0;
                
                
                acc1 += x1 * c0;
                
                
                acc2 += x2 * c0;
                
                
                acc3 += x3 * c0;
                
                
                c0 = *(py--);
                
                
                x0 = *(px + 1);
                
                
                acc0 += x1 * c0;
                
                acc1 += x2 * c0;
                
                acc2 += x3 * c0;
               
                acc3 += x0 * c0;
                
                
                c0 = *(py--);
                
                
                x1 = *(px + 2);
                
               
                acc0 += x2 * c0;
                
                acc1 += x3 * c0;
               
                acc2 += x0 * c0;
                
                acc3 += x1 * c0;
                
                
                c0 = *(py--);
                
               
                x2 = *(px + 3);
                px += 4;
                
                
                acc0 += x3 * c0;
                
                acc1 += x0 * c0;
               
                acc2 += x1 * c0;
                
                acc3 += x2 * c0;
                
                
            } while(--k);
            
            
            k = srcBLen % 0x4;
            
            while(k > 0)
            {
                
                c0 = *(py--);
                
                
                x3 = *(px++);
                
                
                acc0 += x0 * c0;
                
                acc1 += x1 * c0;
                
                acc2 += x2 * c0;
          
                acc3 += x3 * c0;
                
                
                x0 = x1;
                x1 = x2;
                x2 = x3;
                
               
                k--;
            }
            
            *pOut++ = acc0;
            *pOut++ = acc1;
            *pOut++ = acc2;
            *pOut++ = acc3;
            
          
            count += 4;
            
           
            px = pIn1 + count;
            py = pSrc2;
            
            blkCnt--;
        }
        blkCnt = blockSize2 % 0x4;
        
        while(blkCnt > 0)
        {
            
            sum = 0.0f;
            
            
            k = srcBLen >> 2;
            
            
            while(k > 0)
            {
                
                sum += *px++ * *py--;
                sum += *px++ * *py--;
                sum += *px++ * *py--;
                sum += *px++ * *py--;
                
                
                k--;
            }
            
            
            k = srcBLen % 0x4;
            
            while(k > 0)
            {
               
                sum += *px++ * *py--;
                
                
                k--;
            }
            
            
            *pOut++ = sum;
            
           
            count++;
            
            
            px = pIn1 + count;
            py = pSrc2;
            
           
            blkCnt--;
        }
    }
    else
    {
        
        blkCnt = blockSize2;
        
        while(blkCnt > 0)
        {
           
            sum = 0.0;
            
            
            k = srcBLen;
            
            while(k > 0)
            {
                
                sum += *px++ * *py--;
                
                
                k--;
            }
            
            *pOut++ = sum;
            
           
            count++;
            
            
            px = pIn1 + count;
            py = pSrc2;
            
            
            blkCnt--;
        }
    }
    
    
    pSrc1 = (pIn1 + srcALen) - (srcBLen - 1);
    px = pSrc1;
    
   
    pSrc2 = pIn2 + (srcBLen - 1);
    py = pSrc2;
    
    
    while(blockSize3 > 0)
    {
        
        sum = 0.0;
        
        
        k = blockSize3 >> 2;
        
        
        while(k > 0)
        {
           
            sum += *px++ * *py--;
            
            
            sum += *px++ * *py--;
            
            
            sum += *px++ * *py--;
            
            
            sum += *px++ * *py--;
            
            
            k--;
        }
        
        k = blockSize3 % 0x4;
        
        while(k > 0)
        {
            
            sum += *px++ * *py--;
            
           
            k--;
        }
        
       
        *pOut++ = sum;
        
        
        px = ++pSrc1;
        py = pSrc2;
        
        
        blockSize3--;
    }
}




















