#include "structs.h"
#include "functions.h"

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int main(int argc, char** argv){
  int a, numImages,umbralBin,umbralClas,i,answerNB,imprimir=0;
  bmpInfoHeader bInfoHeader;
	bmpFileHeader header;


  while ((a = getopt (argc, argv, "bc:u:n:")) != -1){
    switch(a){
      case 'c':
        if(isInt(optarg)){
          numImages=atoi(optarg);
          //printf("The number of images to process are: %i\n",numImages);
        }
        else{
          printf("The input -c has a character not valid (only integer digits!)\n");
          return 0;
        }
        break;
      case 'u':
        if(isInt(optarg)){
          umbralBin=atoi(optarg);
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
          //printf("UMBRAL to classify is:%i\n",umbralClas);
        }
        else{
          printf("The input -n has a not valid character (only integer digits!)\n");
          return 0;
        }
        break;
      case 'b':
        imprimir=1;
        break;
      case '?':
        if (optopt == 'c' || optopt == 'u' || optopt == 'n')
          fprintf (stderr, "Opcion -%c requiere un argumento.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknowing option `-%c'.\n", optopt);
        else
          fprintf (stderr,"Opcion con caracter desconocido `\\x%x'.\n",optopt);
          return 1;
      default:
        abort();
    }//fin del switch
  }//fin del while


  if(imprimir==1){
    printf("|    image   | nearly black  |\n");
    printf("|------------|---------------|\n");
  }

  for(i=1;i<numImages+1;i++){

    char* nombreEntrada=malloc(sizeof(char)*22);
    nombreEntrada=setNameInput(i);
    unsigned char* imagen = loadBMP(nombreEntrada, &bInfoHeader, &header);

    /*
    char* nombreSalida=malloc(sizeof(char)*22);
    nombreSalida=setNameOutput(i);
    saveImage(imagen, bInfoHeader, header,nombreSalida);
    */


    char* nombreSalidaGS=malloc(sizeof(char)*22);
    nombreSalidaGS=setNameOutputGS(i);
    saveImageGS(imagen, bInfoHeader, header,nombreSalidaGS);

    unsigned char* imagen2 = loadBMP(nombreEntrada, &bInfoHeader, &header);
    char* nombreSalidaBin=malloc(sizeof(char)*22);
    nombreSalidaBin=setNameOutputBin(i);
    saveImageBin(imagen2,bInfoHeader,header,nombreSalidaBin,umbralBin);

    if(imprimir==1){
      //unsigned char* imagen3 = loadBMP(nombreEntrada, &bInfoHeader, &header);
      //saveImage(imagen3,bInfoHeader,header,"guardadopreNearly.bmp");
      answerNB=nearlyBlack(imagen2,bInfoHeader,umbralClas);
      printf("|  imagen_%d  |      %d      |\n",i,answerNB);
      //free(imagen3);
    }
    free(imagen);
    free(imagen2);


  }

  return 0;
}
