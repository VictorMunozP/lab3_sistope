#include "functions.h"
#include "structs.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <pthread.h>


/**
* Funcion principal
* @param argc cantidad de parametros recibidos por consola
* @param argv arreglo de parametros recibidos
* @return entero
*/
int main(int argc, char** argv){


  int a,cantHebras,numImages,umbralBin,umbralClas,i,j,k,imprimir=0;
  bmpFileHeader header;
  bmpInfoHeader bInfoHeader;
  //Ciclo while para uso de funcion getopt
  while ((a = getopt (argc, argv, "bc:u:n:h:")) != -1){
    switch(a){
      case 'c':
        if(isInt(optarg)){
          numImages=atoi(optarg);
          //printf("The number of images to process are: %i\n",numImages);
        }
        else{
          printf("Not valid input -c (only integer digits!)\n");
          return 0;
        }
        break;
      case 'h':
        if(isInt(optarg)){
          cantHebras=atoi(optarg);
          //printf("The number of threads to create are: %i\n",cantHebras);
        }
        else{
          printf("Not valid input -h (only integer digits!)\n");
          return 0;
        }
        break;
      case 'u':
        if(isInt(optarg)){
          umbralBin=atoi(optarg);
          if(umbralBin<0 || umbralBin>255){
            printf("Not valid input -u (only integer [0,255] numbers!)\n");
            return 0;
          }
          //printf("UMBRAL to Bin is: %i\n",umbralBin);
        }
        else{
          printf("Not valid input -u (only integer digits!)\n");
          return 0;
        }
        break;
      case 'n':
        if(isInt(optarg)){
          umbralClas=atoi(optarg);
          if(umbralClas<0 || umbralClas>100){
            printf("Not valid input -n (only integer [0,100] numbers!)\n");
            return 0;
          }
          //printf("UMBRAL to classify is:%i\n",umbralClas);
        }
        else{
          printf("Not valid input -n (only integer digits!)\n");
          return 0;
        }
        break;
      case 'b':
        imprimir=1;
        break;
      case '?':
        if (optopt == 'c' || optopt == 'u' || optopt == 'n')
          fprintf (stderr, "Option -%c requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknow option `-%c'.\n", optopt);
        else{
          fprintf (stderr,"Option with unknown character `\\x%x'.\n",optopt);
          return 1;
        }
      default:
        abort();
    }//fin del switch
  }//fin del while

  if(imprimir==1){//la variable imprimir es seteada en 1 segun la bandera -b
    printf("|    image   | nearly black  |\n");
    printf("|------------|---------------|\n");
  }

  //ciclo for para procesar las imagenes, una a la vez
  for(i=1;i<numImages+1;i++){

    //Se construye nombre de entrada de las imagenes segun enunciado
    char* nombreEntrada=malloc(sizeof(char)*22);
    nombreEntrada=setNameInput(i);

    //Si no existe la imagen a procesar el programa termina  $$$$$$$$$$$$$-
    FILE* fp=fopen(nombreEntrada,"r");
    if(fp){
      fclose(fp);
    }else{
      printf("File '%s' not found, execution canceled.\n",nombreEntrada);
      return 0;}
    //$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$-

    //GS= GrayScale
    pthread_barrier_init(&mybarrier, NULL, cantHebras+1);

    //-----------------MULTI-THREADING----------------------------------
    pthread_t threadsGS[cantHebras];
    //unsigned char* imagen = loadImage(nombreEntrada, &bInfoHeader, &header);
    threadData* parameters=(threadData*)malloc(sizeof(threadData));


    parameters->imagen=loadImage(nombreEntrada,&bInfoHeader, &header);
    parameters->b=bInfoHeader;
    parameters->barrierc=i;
    //printf("parameters->b.height es: %d \n",parameters->b.height);


    parameters->filaFinal=0;
    for(j=0;j<cantHebras;j++){
        parameters->filaInicial=parameters->filaFinal;
        printf("main: thread %d parameters->filaInicial es: %d \n",j,parameters->filaInicial);

        parameters->filaFinal= parameters->filaFinal+ (parameters->b.height/cantHebras)-1;
        printf("main: thread %d parameters->filaFinal es: %d \n\n",j,parameters->filaFinal);

        pthread_create(&threadsGS[j],NULL,rgbToGrayScaleByRow2,(void*) &parameters);
        parameters->filaFinal=parameters->filaFinal+1;
    }
    //printf("Filas no procesadas: %d \n",parameters->b.height%cantHebras);

    pthread_barrier_wait(&mybarrier);

    char* nombreSalidaGS=malloc(sizeof(char)*22);
    nombreSalidaGS=setNameOutputGS(i);
    //rgbToGrayScale(imagen,bInfoHeader);

    for(k=0;k<cantHebras;k++){
      pthread_join(threadsGS[k],NULL);
    }
    saveImage(parameters->imagen, bInfoHeader, header,nombreSalidaGS);


    pthread_barrier_destroy(&mybarrier);


    /*/Bin= Binarize
    unsigned char* imagen2 = loadImage(nombreEntrada, &bInfoHeader, &header);
    char* nombreSalidaBin=malloc(sizeof(char)*22);
    nombreSalidaBin=setNameOutputBin(i);
    binarizeImage(imagen2,bInfoHeader,umbralBin);
    saveImage(imagen2, bInfoHeader, header,nombreSalidaBin);
    */

    if(imprimir==1){
      char* answerNB=malloc(sizeof(char)*4);
      answerNB=nearlyBlack(parameters->imagen,bInfoHeader,umbralClas);
      printf("|  imagen_%d  |      %-3s      |\n",i,answerNB);
    }
    //free(imagen);
    //free(imagen2);
    //pthread_mutex_destroy(&mutex);
  }//fin del ciclo for para procesar las imagenes

  return 0;
}
