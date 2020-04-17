//
//  main.cpp
//  DSP Tesis
//
//  Created by Carlos Segovia on 23/10/19.
//  Copyright © 2019 Carlos Segovia. All rights reserved.
//

#include <iostream>         //Incluye la libreria iostream para permitir la salida en pantalla
#include <ctime>            //Incluye la libreria ctime  que permitara calcular el tiempo de ejecucion de cada funcion
#include <fstream>          //Incluye la libreria fstream que permite la creacion, escritura y lectura de archivos


#define SIG_L 320           //define una constante que representara el tamano de la senal de entrada de un valor de 320
#define IMP_SIG_L 30        //define una constante que representara el tamano de la respuesta de impulso de un valor de 30
#define FFT_SIG_L 256       //define una constante que representara el tamano de la senal de entrada para la FFT de un valor de 256
#define FILTER_L 29         //define una constante que representara el tamano del filtro de un valor de 29

using namespace std;        //evita tener que escribir std:: antes de cada cout, cin, vector


//SEÑALES DE ENTRADA
extern double Input_signal [SIG_L];     //Senal de entrada que se encuentra en el archivo waveform.cpp es una senal de 1KHz y 15KHz
extern long double LONG_Input_signal [SIG_L];
extern double imp_resp [IMP_SIG_L];     //Respuesta de impulso que representa un filtro pasabajas de 10KHz
double Sig_1Khz [SIG_L];                //Senal de 1KHz generada con la funcion sintesis
double Sig_10KHz [SIG_L];               //Senal de 15KHz generada con la funcion sintesis
double Sig_1KHz_10KHz [SIG_L];          //Senal resultante de sumar 1KHz mas 15KHz


//SEÑALES DE SALIDA
double convolution_output_signal [ SIG_L + IMP_SIG_L ];     //Senal de salida del algoritmo de convolucion seccion 3.2.2
double robust_convolution_output_signal [SIG_L + IMP_SIG_L];//Senal de salida del algoritmo de convolucion seccion 3.2.3
double DFT_REX[(SIG_L / 2) + 1];                  //Senal de salida del algoritmo DFT, seccion 3.3, representa la parte Real
double DFT_IMX[(SIG_L / 2) + 1];                  //Senal de salida del algoritmo DFT, seccion 3.3, representa la parte Imaginaria
double DFT_MAG[SIG_L];                            //Senal de salida del algoritmo DFT, seccion 3.3, representa la magnitud
double IDFT[SIG_L];                             //Senal de salida del algoritmo IDFT, seccion 3.4,
double FFT_Input_Signal_ReX [FFT_SIG_L];        //Copia de la senal de entrada Parte real
double FFT_Input_Signal_ImX [FFT_SIG_L];        //Copia de la senal de entrada Parte imaginaria
double FFT_MAG [FFT_SIG_L];                     //Senal de salida del algoritmo FFT, seccion 3.5, representa la magnitud
double FFT_IDFT[SIG_L];                         //Senal de salida del algoritmo IDFT, seccion 3.4, usado con la FFT
double FILTRO_Output_Signal[SIG_L];             //Senal de salida del algoritmo moving_average seccion 3.6.1
double RECURSIVE_Output_Signal[SIG_L];          //Senal de salida del algoritmo recursive_moving_average seccion 3.6.2
long double Filter_arr [FILTER_L];              //Senal de salida del Windowed sync low pass, seccion 3.6.3, representa un filtro pasabjas de 10KHz
double WINDOWED_Output_Sig [FILTER_L + SIG_L];  //Senal de salida despues de convolucion entre Filter_arr y Input_signal
                                                //no debe haber contenido frecuencial por encima de 10KHz
long double WINDOWED_Output_Sig_Robust_Conv [FILTER_L + SIG_L];  //Senal de salida despues de convolucion entre Filter_arr y Input_signal
                                                            //no debe haber contenido frecuencial por encima de 10KHz



//      FUNCIONES       //
extern void windowed_sync_low_pass ( long double *filtr_kernel_dest_arr, double cutoff_freq, int filtr_lght);
extern void windowed_convolution(double *sig_src_arr,double *sig_dest_arr,long double *imp_response_arr,int  sig_src_length,int  imp_response_length);
extern void windowed_robust_conv( long double * pSrcA,uint32_t srcALen, long double * pSrcB,  uint32_t srcBLen, long double * pDst);
extern double calc_signal_mean (double* sig_src_arr, int sig_lenght);
extern double calc_signal_std (double* sig_src_arr, double sig_mean, int sig_lenght);
extern double calc_signal_rms (double* sig_src_arr, int sig_lenght);
extern double calc_signal_noise(double* mean, double* std_dev);
extern double calc_signal_CV(double* mean, double* std_dev);
extern void convolution (double *sig_src_arr,double *sig_dest_arr,double *imp_resp_arr,int src_sig_lght,int imp_resp_lght);
extern void robust_conv(double * pSrcA,uint32_t srcALen,double * pSrcB,  uint32_t srcBLen,double * pDst );
extern void calc_sig_dft (double *pSrc_arr,double *sig_dest_rex_arr,double *sig_dest_imx_arr,int sig_lenght);
extern void get_dft_output (double *sig_dest_mag_arr, double *sig_src_rex_arr, double *sig_src_imx_arr, int sig_lenght);
extern void calc_idft (double *idft_out_arr, double *sig_src_rex_arr, double *sig_src_imx_arr, int idft_lenght);
extern void Sig_std (double *pSig_src_arr, int Blocksize, double *pResult );
extern void robust_mean(double *sig_src_arr,int Blocksize, double *result );
extern void moving_average( double * pSig_src_arr, double *pSig_out_arr, int signal_lenght, int filter_pts);
extern void recursive_moving_average( double * pSig_src_arr, double *pSig_out_arr, int signal_lenght, int filter_pts);
extern void sig_rms (double *pSig_src_arr, int Blocksize, double *pResult );
extern void FFT ( double* ReX, double* ImX, int Sig_L);
extern void sig_gen (double * output_sig,int sig_lenght, int frecuency );
extern void sintesis(double * output_sig,int sig_lenght, double *inputA, double *inputB );





int main() {
    //          Generacion de senales y sintesis            //
    sig_gen(Sig_1Khz, SIG_L, 1000);
    sig_gen(Sig_10KHz, SIG_L, 10000);
    sintesis(Sig_1KHz_10KHz, SIG_L, Sig_1Khz, Sig_10KHz);
    
    ofstream file24, file22, file23;
    
    file24.open("Sig_1Khz.dat");
    file22.open("Sig_10KHz.dat");
    file23.open("Sig_1KHz_10KHz.dat");
    
    for (int i = 0; i <SIG_L; i++) {
        file24<<Sig_1Khz[i]<<endl;
        file22<<Sig_10KHz[i]<<endl;
        file23<<Sig_1KHz_10KHz[i]<<endl;
    }
    
    file24.close();
    file22.close();
    file23.close();
    
    
    // PRUEBA DE ALGORITMOS MEAN //
    
    clock_t t;          //t es una variable de tipo clock_t que hace referencia al tiempo clicks
    double f;           //f es una variable de tipo double que almacenara el valor de la media
    
    t = clock();        //la funcion clock() devuelve la cantidad de segundos que han transcurrido desde el 00:00 hours, Jan 1, 1970 UTC
                        //se almacena ese valor en t
    
    cout <<"Calculating mean\n";        //Imprime en pantalla "Calculating mean"
    f = calc_signal_mean(Input_signal, SIG_L);      //Se llama a la funcion calc_signal_mean() que devuelve un valor double y se almacena en f
    cout<< "The mean of the signal is:"<< f<<endl;  //Imprime en pantalla el valor de la media calculado por el algoritmo
    t = clock() - t;                        //Se vuelve a llamar a la funcion clock() y se resta con t para saber cuanto tiempo transcurrio
                                            //ese es el tiempo que le tomo a nuestro algoritmo procesar la senal
    
    cout<< "It took me "<< t <<" clicks ("          //se imprime en pantalla el tiempo en clicks
    << ((float) t / CLOCKS_PER_SEC)<< "seconds)"    //se imprime en pantalla el tiempo en segundos
    <<endl<<endl;                                   //doble salto de linea en pantalla
    

    clock_t y;                  //y es una variable de tipo clock_t que hace referencia al tiempo clicks
    double g;                   //g es una variable de tipo double que almacenara el valor de la media
    y = clock();                //la funcion clock() devuelve la cantidad de segundos que han transcurrido desde el 00:00 hours, Jan 1, 1970 UTC
    
    cout<<"Calculating robust mean\n";  //Imprime en pantalla "Calculating robust mean\n"
    robust_mean(Input_signal, SIG_L, &g);   //Se llama a la funcion robust_mean() que almacena el resultado en la variable g
    cout<< "The mean of the signal is:"<< g<<endl;//Imprime en pantalla el valor de la media calculado por el algoritmo
    y = clock() - y;            //Se vuelve a llamar a la funcion clock() y se resta con y para saber cuanto tiempo transcurrio
                                //se almacena ese valor en t
    
    cout<< "It took me "<< y <<" clicks ("          //se imprime en pantalla el tiempo en clicks
    << ((float) y / CLOCKS_PER_SEC)<< "seconds)"    //se imprime en pantalla el tiempo en segundo
    <<endl<<endl;                                   //doble salto de linea en pantalla
    
    
    //  PRUEBA DE STADANRD DEVIATION  //
    
    clock_t q;                          //q es una variable de tipo clock_t que hace referencia al tiempo clicks
    double h;                           //h es una variable de tipo double que almacenara el valor de la desviacion estandar
    q = clock();                     //la funcion clock() devuelve la cantidad de segundos que han transcurrido desde el 00:00 hours, Jan 1, 1970 UTC
                                    //se almacena en q
    
    cout <<"Calculating standard deviation\n";      //Imprime en pantalla "Calculating standard deviation\n"
    h = calc_signal_std(Input_signal, f, SIG_L);    //Se llama a la funcion calc_signal_std() que devuelve un valor double y se almacena en h
    cout<< "The Standard Deviation of the signal is:"<< h<<endl;//Imprime en pantalla el resultado
    q = clock() - q;                 //Se vuelve a llamar a la funcion clock() y se resta con q para saber cuanto tiempo transcurrio
                                    //se almacena ese valor en t
    
    cout<< "It took me "<< q <<" clicks ("      //se imprime en pantalla el tiempo en clicks
    << ((float) q / CLOCKS_PER_SEC)<< "seconds)"//se imprime en pantalla el tiempo en segundo
    <<endl<<endl;                               //doble salto de linea en pantalla
    
    
    clock_t a;                  //a es una variable de tipo clock_t que hace referencia al tiempo clicks
    double w;                   //w es una variable de tipo double que almacenara el valor de la desviacion estandar
    a = clock();                //la funcion clock() devuelve la cantidad de segundos que han transcurrido desde el 00:00 hours, Jan 1, 1970 UTC
                                //se almacena en a
    
    cout<<"Calculating robust standard Deviation\n";        //Imprime en pantalla "Calculating robust standard Deviation"
    Sig_std(Input_signal, SIG_L, &w);                       //Se llama a la funcion Sig_std() que almacena el resultado en la variable w
    
    cout<< "The Standard Deviation of the signal is:"<< w <<endl;//Imprime en pantalla el resultado
    a = clock() - a;                            //Se vuelve a llamar a la funcion clock() y se resta con a para saber cuanto tiempo transcurrio
    
    cout<< "It took me "<< a <<" clicks (" //se imprime en pantalla el tiempo en clicks
    << ((float) a / CLOCKS_PER_SEC)<< "seconds)"//se imprime en pantalla el tiempo en segundo
    <<endl<<endl;                           //doble salto de linea en pantalla
    
    
    
    // PRUEBA DE RMS //
    

    clock_t TIME_RMS;            //TIME_RMS es una variable de tipo clock_t que hace referencia al tiempo clicks
    double RMS;                 //RMS es una variable de tipo double que almacenara el valor del RMS
    TIME_RMS = clock();         //la funcion clock() devuelve la cantidad de segundos que han transcurrido desde el 00:00 hours, Jan 1, 1970 UTC
                                //se almacena en TIME_RMS
    
    cout <<"Calculating RMS\n"; //Imprime en pantalla "Calculating RMS"
    RMS = calc_signal_rms(Input_signal, SIG_L); //Se llama a la funcion calc_signal_rms que devuelve un valor double y se almacena en RMS
    cout<< "The RMS of the signal is:"<< RMS<<endl;//Imprime en pantalla el resultado
    TIME_RMS = clock() - TIME_RMS;      //Se vuelve a llamar a la funcion clock() y se resta con TIME_RMS para saber cuanto tiempo transcurrio
    
    cout<< "It took me "<< TIME_RMS <<" clicks ("   //se imprime en pantalla el tiempo en clicks
    << ((float) TIME_RMS / CLOCKS_PER_SEC)<< "seconds)"//se imprime en pantalla el tiempo en segundo
    <<endl<<endl;                                    //doble salto de linea en pantalla
    
    
    clock_t TIME_ROBUST_RMS;        //TIME_ROBUST_RMS es una variable de tipo clock_t que hace referencia al tiempo clicks
    double ROBUST_RMS;              //RMS es una variable de tipo double que almacenara el valor del RMS
    TIME_ROBUST_RMS = clock();      //la funcion clock() devuelve la cantidad de segundos que han transcurrido desde el 00:00 hours, Jan 1, 1970 UTC
                                    //se almacena en TIME_ROBUST_RMS
    
    cout <<"Calculating the Robust RMS\n"; //Imprime en pantalla "Calculating the Robust RMS"
    sig_rms(Input_signal , SIG_L, &ROBUST_RMS);//Se llama a la funcion sig_rms() que almacena el resultado en la variable ROBUST_RMS
    cout<< "The Robust RMS of the signal is:"<< ROBUST_RMS<<endl;//Imprime en pantalla el resultado
    TIME_ROBUST_RMS = clock() - TIME_ROBUST_RMS;//Se vuelve a llamar a la funcion clock() y se resta con TIME_RMS para saber cuanto tiempo transcurrio
    
    cout<< "It took me "<< TIME_ROBUST_RMS <<" clicks ("//se imprime en pantalla el tiempo en clicks
    << ((float) TIME_ROBUST_RMS / CLOCKS_PER_SEC)<< "seconds)"//se imprime en pantalla el tiempo en segundo
    <<endl<<endl;                                       //doble salto de linea en pantalla
    
    
    
    // ////              PRUEBA DE SIGNAL TO NOISE RATIO ////////
    
    //CALCULAR  SNR CON ALGORITMOS BASICOS
    double snr_mean, snr_std_dev, snr_result;
    
    cout <<"Calculating the Signal to Noise Ratio\n";
    
    clock_t TIME_SNR;
    TIME_SNR = clock();
    
    snr_mean = calc_signal_mean(Input_signal, SIG_L);
    snr_std_dev = calc_signal_std(Input_signal, snr_mean, SIG_L);
    snr_result = calc_signal_noise(&snr_mean, &snr_std_dev);
    
    
    TIME_SNR = clock() - TIME_SNR;
    
    
    cout<< "The Signal to Noise Ratio of the signal is:"<< snr_result<<endl;
    cout<< "It took me "<< TIME_RMS <<" clicks ("<< ((float) TIME_SNR / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    //CALCULAR SNR CON ALGORITMOS AVANZADOS
    
    cout <<"Calculating the Robust Signal to Noise Ratio\n";
    
    double robust_snr_mean, robust_snr_std_dev, robust_snr_result;
    clock_t ROBUST_TIME_SNR;
    ROBUST_TIME_SNR = clock();
    
    robust_mean(Input_signal, SIG_L, &robust_snr_mean);
    Sig_std(Input_signal, SIG_L, &robust_snr_std_dev);
    robust_snr_result = calc_signal_noise(&robust_snr_mean, &robust_snr_std_dev);
    
    ROBUST_TIME_SNR = clock() - ROBUST_TIME_SNR;
    
    
    cout<< "The Signal to Noise Ratio of the signal is:"<< robust_snr_result<<endl;
    cout<< "It took me "<< ROBUST_TIME_SNR <<" clicks ("<< ((float) ROBUST_TIME_SNR / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    
    /// CONVOLUCION DE SENALES  ///
    
    ofstream file1, file2, file3, file21;
    
    //      CONVOLUCION   //
    
    clock_t CONV_TIME;
    CONV_TIME = clock();
    
    convolution(&Input_signal[0], &convolution_output_signal[0], &imp_resp[0], SIG_L, IMP_SIG_L);
    
    CONV_TIME = clock() - CONV_TIME;
    
    
    
    //      Escribir arreglo en archivo     //
    file1.open("Convolution_Output_Signal.dat");
    file3.open("Input_Signal.dat");
    file21.open("Impulse_Response.dat");
    
    for (int i = 0; i< SIG_L + IMP_SIG_L; i++) {
        file1<<convolution_output_signal[i]<<endl;
        
        if (i<IMP_SIG_L) {
            file21<<imp_resp[i]<<endl;
        }
        if (i<SIG_L) {
            file3<<Input_signal[i]<<endl;
        }
    }
    
    file1.close();
    file3.close();
    
    // Output //
    cout<< "Convolution successfully executed"<<endl;
    cout<< "It took me "<< CONV_TIME <<" clicks ("<< ((float) CONV_TIME / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    
    //  CONVOLUCION ALGORITMO 2   //
    
    clock_t ROBUST_CONV_TIME;
    ROBUST_CONV_TIME = clock();
    
    robust_conv(&Input_signal[0], SIG_L, &imp_resp[0], IMP_SIG_L, &robust_convolution_output_signal[0]);
    
    ROBUST_CONV_TIME = clock() - ROBUST_CONV_TIME;
    
    //      Escribir arreglo en archivo     //
    file2.open("Robust_Convolution_Output_Signal.dat");
    
    for (int i = 0; i< SIG_L + IMP_SIG_L; i++) {
        file2<<robust_convolution_output_signal[i]<<endl;
    }
    
    file2.close();
    
    
    
    // Output //
    cout<< "Robust convolution successfully executed"<<endl;
    cout<< "It took me "<< ROBUST_CONV_TIME <<" clicks ("<< ((float) ROBUST_CONV_TIME / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    
    //          DFT         //
    clock_t DFT_TIME;
    DFT_TIME = clock();
    
    calc_sig_dft((double *)&Input_signal[0], (double *)&DFT_REX[0],(double *) &DFT_IMX[0], (int)SIG_L);
    
    
    DFT_TIME = clock() - DFT_TIME;
    
    get_dft_output(&DFT_MAG[0], &DFT_REX[0], &DFT_IMX[0], SIG_L);
    
    //          IDFT            //
    
    calc_idft((double *)&IDFT[0], (double *) &DFT_REX[0],(double *) &DFT_IMX[0],(int) SIG_L);
    
    // Output //
    cout<< "DFT successfully executed"<<endl;
    cout<< "It took me "<< DFT_TIME <<" clicks ("<< ((float) DFT_TIME / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    //      ESCRIBIR ARCHIVOS       //
    ofstream file4, file5,file6, file7, file8;
    
    file4.open("DFT_INPUT.dat");
    file5.open("DFT_REX.dat");
    file6.open("DFT_IMX.dat");
    file7.open("DFT_MAG.dat");
    file8.open("IDFT_OUTPUT.dat");
    
    
   for (int i = 0; i < SIG_L; i++)
   {
       file4<<Input_signal[i]<<endl;
       file5<<DFT_REX[i]<<endl;
       file6<<DFT_IMX[i]<<endl;
       file7<<DFT_MAG[i]<<endl;
       file8<<IDFT[i]<<endl;
   }
    
    file4.close();
    file5.close();
    file6.close();
    file7.close();
    file8.close();
    
    
    
    
    //          FFT             //
    /*
     COMO LA FUNCION FFT ALMACENA LA SENAL DE SALIDA DENTRO DEL ARREGLO DE LA SENAL DE ENTRADA
     SE USARA UNA COPIA DE LA SENAL DE ENTRADA PARA NO SOBREESCRIBIR LA SENAL QUE PLANEAMOS USAR
     EN MAS EXPERIMENTOS.
     
     la senal se recortara al numero mas cercano a una potencia de 2
     se usara solo 256 muestras
     
     hacer prueba con arreglos de 512 a ver que sucede
     */
    
    //          NO FUNCIONO                                         //
    //DEBO HABER HECHO LA TRANSCRIPCION DEL CODIGO BASIC MAL        //
    //SI ES UN ALGORITMO POPULAR ALGUIEN YA LO DEBIO PASADO A C++   //
    //      SIGUE CON LOS FILTROS, LO IMPORTANTE ES TERMINAR DE ESCRIBIR LAS PRUEBAS        //
    
    
    for(int i = 0; i < FFT_SIG_L;i++)           //Proceso de copia de la senal de entrada
    {
        FFT_Input_Signal_ReX[i] = Input_signal [i];     //la parte real contiene los valores de la senal
        FFT_Input_Signal_ImX[i] = 0;                   //la parte imaginaria contiene 0
        
    }
    
    
    clock_t FFT_TIME;                   //Creacion de variable que contara la duracion de aa funcion FFT
    FFT_TIME = clock();                 //Inicializacion de la variable
    
    FFT ( (double*) &FFT_Input_Signal_ReX [0], (double*) &FFT_Input_Signal_ImX [0], FFT_SIG_L);     //FFT
    
    FFT_TIME = clock() - FFT_TIME;      //Calculo de la duracion de la funcion
    
    get_dft_output(&FFT_MAG[0], &FFT_Input_Signal_ReX[0], &FFT_Input_Signal_ImX[0], FFT_SIG_L);
    
    //          IDFT            //
    calc_idft((double *)&FFT_IDFT[0], (double *) &FFT_Input_Signal_ReX[0],(double *) &FFT_Input_Signal_ImX[0],(int) FFT_SIG_L);         //reconstruye la senal deberia ser igual a la senal de salida
    
    
    // Output //
    cout<< "FFT successfully executed"<<endl;
    cout<< "It took me "<< FFT_TIME <<" clicks ("<< ((float) FFT_TIME / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    //      ESCRIBIR ARCHIVOS        //
    
    ofstream file9, file10,file11, file12, file13;
    
    file9.open("FFT_INPUT.dat");
    file10.open("FFT_REX.dat");
    file11.open("FFT_IMX.dat");
    file12.open("FFT_MAG.dat");
    file13.open("FFT_IDFT_OUTPUT.dat");
    
    
    for (int i = 0; i < FFT_SIG_L; i++)
    {
        file9<<Input_signal[i]<<endl;
        file10<<FFT_Input_Signal_ReX[i]<<endl;
        file11<<FFT_Input_Signal_ImX[i]<<endl;
        file12<<FFT_MAG[i]<<endl;
        file13<<FFT_IDFT[i]<<endl;
    }
    
    file9.close();
    file10.close();
    file11.close();
    file12.close();
    file13.close();
    
    
    
    
    
    
    //          DISENO DE FILTROS             //
    /*
     SE COMPARARA LA VELOCIDAD DEL AVERAGE CONTRA LA DEL RECURSIVE MOVING AVERAGE
     Y LUEGO CON EL WINDOWED
     
     PENSAR EN MAS PRUEBAS
     */
    
    //      MOVING AVERAGE FILTER       //
    
    //RELOJ
    clock_t FILTRO_TIME;
    FILTRO_TIME = clock();
    
    //FILTRAR
    
    moving_average( (double *) &Input_signal[0], (double *)&FILTRO_Output_Signal[0], (int) SIG_L,10);
    
    //RELOJ
    
    FILTRO_TIME = clock() - FILTRO_TIME;
    
    //OUTPUT
    
    cout<< "Moving average filter successfully executed"<<endl;
    cout<< "It took me "<< FILTRO_TIME <<" clicks ("<< ((float) FILTRO_TIME / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    //ESCRIBIR ARCHIVO
    
    
    ofstream file14, file15;
    
    file14.open("MAF_OUTPUT.dat");
    file15.open("MAF_INPUT.dat");
    
    
    for (int i = 0; i < SIG_L; i++)
    {
        file14<<FILTRO_Output_Signal[i]<<endl;
        file15<<Input_signal[i]<<endl;
        
    }
    
    file14.close();
    file15.close();
    
    
    
    //          RECURSIVE MOVING AVERAGE FILTER            //
    
    
    //RELOJ
    clock_t RECURSIVE_FILTER_TIME;
    RECURSIVE_FILTER_TIME = clock();
    
    
    //FILTRAR
    
    
    recursive_moving_average( (double *) &Input_signal[0], (double *)&RECURSIVE_Output_Signal[0], (int) SIG_L, 10);
    
    
    //RELOJ
    
    RECURSIVE_FILTER_TIME = clock() - RECURSIVE_FILTER_TIME;
    
    
    //OUTPUT
    
    cout<< "Recursive moving average filter successfully executed"<<endl;
    cout<< "It took me "<< RECURSIVE_FILTER_TIME <<" clicks ("<< ((float) RECURSIVE_FILTER_TIME / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    //ESCRIBIR ARCHIVO
    
    
    ofstream file16, file17;
    
    file16.open("RECURSIVE_OUTPUT.dat");
    file17.open("RECURSIVE_INPUT.dat");
    
    
    for (int i = 0; i < SIG_L; i++)
    {
        file16<<RECURSIVE_Output_Signal[i]<<endl;
        file17<<Input_signal[i]<<endl;
        
    }
    
    file16.close();
    file17.close();
    
    
    
    
    /*
     
     SE COMPARARA LA CANTIDA DE RUIDO QUE TIENE CADA SENAL
     PARA CONOCER MAS SOBRE LA EFICIENCIA DE CADA ALGORITMO
     USANDO LAS FUNCIONES:
     
     1-. calc_signal_noise(double* mean, double* std_dev);
     2-. calc_signal_CV(double* mean, double* std_dev);
     
     
     */
    
    //          Signal to Noise Ratio para Input Signal
    
    // declaracion de variables
    double mean_Input;
    double std_Input;
    
    double Input_SNR = 0.0;
    double Input_CV = 0.0;
    
    //Funciones
    robust_mean((double *)&Input_signal[0],(int) SIG_L, &mean_Input );
    Sig_std ((double *)&Input_signal[0], (int) SIG_L, &std_Input );
    
    Input_SNR = calc_signal_noise((double*) &mean_Input, (double*) &std_Input);
    Input_CV = calc_signal_CV((double*) &mean_Input, (double*) &std_Input);
    
    
    //Output
    cout<<"Input Signal SNR: "<<Input_SNR<<endl;
    cout<<"Input Signal CV: "<<Input_CV<<endl;
    cout<<endl<<endl;
    
    
    //          Signal to Noise Ratio para Recursive Moving Average Filter Signal
    
    
    // declaracion de variables
    double mean_RECURSIVE_Output;
    double std_RECURSIVE_Output;
    
    double RECURSIVE_SNR = 0.0;
    double RECURSIVE_CV = 0.0;
    
    //Funciones
    robust_mean((double *)&RECURSIVE_Output_Signal[0],(int) SIG_L, &mean_RECURSIVE_Output );
    Sig_std ((double *)&RECURSIVE_Output_Signal[0], (int) SIG_L, &std_RECURSIVE_Output );
    
    RECURSIVE_SNR = calc_signal_noise((double*) &mean_RECURSIVE_Output, (double*) &std_RECURSIVE_Output);
    RECURSIVE_CV = calc_signal_CV((double*) &mean_RECURSIVE_Output, (double*) &std_RECURSIVE_Output);
    
    
    //Output        //esto da un numero negativo y no se por que
    cout<<"Recursive Moving Average Filter Output Signal SNR: "<<RECURSIVE_SNR<<endl;
    cout<<"Recursive Moving Average Filter Output Signal CV: "<<RECURSIVE_CV<<endl;
    cout<<endl<<endl;
    
    
    //          Signal to Noise Ratio para Moving Average Filter Signal
    
    // declaracion de variables
    double mean_MAF_Output;
    double std_MAF_Output;
    
    double MAF_SNR = 0.0;
    double MAF_CV = 0.0;
    
    //Funciones
    robust_mean((double *)&FILTRO_Output_Signal[0],(int) SIG_L, &mean_MAF_Output );
    Sig_std ((double *)&FILTRO_Output_Signal[0], (int) SIG_L, &std_MAF_Output );
    
    MAF_SNR = calc_signal_noise((double*) &mean_MAF_Output, (double*) &std_MAF_Output);
    MAF_CV = calc_signal_CV((double*) &mean_MAF_Output, (double*) &std_MAF_Output);
    
    
    //Output
    cout<<"Moving Average Filter Output Signal SNR: "<<MAF_SNR<<endl;
    cout<<"Moving Average Filter Output Signal CV: "<<MAF_CV<<endl;
    cout<<endl<<endl;
    
    
    
    /*
    
                WINDOWED-SYNC FILTER
     
     */
    
    //RELOJ
    clock_t WINDOWED_FILTER_TIME;
    clock_t WINDOWED_FILTER_CONV;
    clock_t WINDOWED_FILTER_ROBUST_CONV;
    WINDOWED_FILTER_TIME = clock();
    
    
    //FILTRAR
    
    
   
    
    /*
     
        El segundo valor de la funcion windowed_sync_low_pass "cutoff_freq", debe estar normalizado, esto quiere decir
        nuestra frecuencia de corte debe estar en funcion a la tasa de sampleo de nuestra senal de entrada, en este
        caso 48KHz,recordemos que por teorema de nyquist nuestra frecuencia no debe exceder la mitad de la tasa de
        sampleo en este caso 24KHz. Como queremos cortar en 10Khz, usaremos regla de tres para ver cual es valor de
        nuestra frecuencia de corte.
     
     24000-----0.5
     10000-----0.2
     
     */
    
     windowed_sync_low_pass ( Filter_arr, 0.2, FILTER_L);
    
    //RELOJ
    
    WINDOWED_FILTER_TIME = clock() - WINDOWED_FILTER_TIME;
    
    //OUTPUT
    
    cout<< "Windowed Sync filter successfully generated"<<endl;
    cout<< "It took me "<< WINDOWED_FILTER_TIME <<" clicks ("<< ((float) WINDOWED_FILTER_TIME / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    
    /*
     
    Ahora se usara la funcion de convolucion para que el filtro disenado actue sobre la senal de entrada
     
     */
    
    WINDOWED_FILTER_CONV = clock();                             //RELOJ
    windowed_convolution(Input_signal, WINDOWED_Output_Sig ,(long double*) Filter_arr, SIG_L, FILTER_L);
    WINDOWED_FILTER_CONV = clock() - WINDOWED_FILTER_CONV;      //RELOJ
    
    
    WINDOWED_FILTER_ROBUST_CONV = clock();                                      //RELOJ
    windowed_robust_conv(LONG_Input_signal, SIG_L, (long double *)Filter_arr, FILTER_L, WINDOWED_Output_Sig_Robust_Conv);
    WINDOWED_FILTER_ROBUST_CONV = clock() - WINDOWED_FILTER_ROBUST_CONV;        //RELOJ
    
    
    //OUTPUT
    
    cout<< "Convolution between Input signal and Windowed sync filter successfully executed"<<endl;
    cout<< "It took me "<< WINDOWED_FILTER_CONV <<" clicks ("<< ((float) WINDOWED_FILTER_CONV / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    cout<<"The total time is: "<< WINDOWED_FILTER_TIME + WINDOWED_FILTER_CONV<<" clicks ("<< ((float) (WINDOWED_FILTER_TIME + WINDOWED_FILTER_CONV) / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    
    
    cout<< "Robust Convolution between Input signal and Windowed sync filter successfully executed"<<endl;
    cout<< "It took me "<< WINDOWED_FILTER_ROBUST_CONV <<" clicks ("<< ((float) WINDOWED_FILTER_ROBUST_CONV / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    cout<<"The total time is: "<< WINDOWED_FILTER_TIME + WINDOWED_FILTER_ROBUST_CONV<<" clicks ("<< ((float) (WINDOWED_FILTER_TIME + WINDOWED_FILTER_ROBUST_CONV) / CLOCKS_PER_SEC)<< "seconds)"<<endl<<endl;
    
    //ESCRIBIR ARCHIVO
    
    
    ofstream file18, file19,file20, file25;
    
    file18.open("WINDOWED_INPUT.dat");
    file19.open("WINDOWED_FILTER.dat");
    file20.open("WINDOWED_OUTPUT.dat");
    file25.open("WINDOWED_OUTPUT_ROBUST_CONV.dat");
    
    
    for (int i = 0; i < SIG_L + FILTER_L; i++)
    {
        file20<<WINDOWED_Output_Sig[i]<<endl;
        file25<<WINDOWED_Output_Sig_Robust_Conv[i]<<endl;
        
        if(i<FILTER_L){
            file19<<Filter_arr[i]<<endl;
        }
        if(i<SIG_L)
        {
        file18<<Input_signal[i]<<endl;
        }
        
        
    }
    
    file18.close();
    file19.close();
    file20.close();
    file25.close();
    
    
    
    
    
    /*
     
     
    
    /// CONVOLUCION DE SENALES  ///
    
    ofstream file1, file2, file3;
    
    //      CONVOLUCION   //
    
    clock_t CONV_TIME;
    CONV_TIME = clock();
    
    convolution(&Input_signal[0], &convolution_output_signal[0], &imp_resp[0], SIG_L, IMP_SIG_L);
    
    CONV_TIME = clock() - CONV_TIME;
    
    
    
    //      Escribir arreglo en archivo     //
    file1.open("Convolution_Output_Signal.dat");
    file3.open("Input_Signal.dat");
    
    for (int i = 0; i< SIG_L + IMP_SIG_L; i++) {
        file1<<convolution_output_signal[i]<<endl;
        
        if (i<SIG_L) {
            file3<<Input_signal[i]<<endl;
        }
    }
    
    file1.close();
    file3.close();
    
    
     Ahora se usara la funcion de convolucion para que el filtro disenado actue sobre la senal de entrada
     
     */
    return 0;
}
