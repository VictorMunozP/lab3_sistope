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
  int a, numImages,umbralBin,umbralClas;

  while ((a = getopt (argc, argv, "bc:u:n:")) != -1){
    switch(a){
      case 'c':
        if(isInt(optarg)){
          numImages=atoi(optarg);
          printf("The number of images to process are: %i\n",numImages);
        }
        else{
          printf("The input -c has a character not valid (only integer digits!)\n");
          return 0;
        }
        break;
      case 'u':
        if(isInt(optarg)){
          umbralBin=atoi(optarg);
          printf("UMBRAL to Bin is: %i\n",umbralBin);
        }
        else{
          printf("Not valid input -u (only integer digits!)\n");
          return 0;
        }
        break;
      case 'n':
        if(isInt(optarg)){
          umbralClas=atoi(optarg);
          printf("UMBRAL to classify is:%i\n",umbralClas);
        }
        else{
          printf("The input -n has a not valid character (only integer digits!)\n");
          return 0;
        }
        break;
      case 'b':
        printf("Printing...\n");
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

  bmpInfoHeader bInfoHeader;
	bmpFileHeader header;

  for(int i=1;i<numImages+1;i++){
    char* nombreEntrada=malloc(sizeof(char)*22);
    nombreEntrada=setNameImg(i);
    //printf("Nombre de entrada:%s\n",nombreDef);
    unsigned char* imagen = LoadBMP(nombreEntrada, &bInfoHeader, &header);

    //Pixeles* pixeles = transformarArray(imagen, bInfoHeader);

    char* nombreSalida=malloc(sizeof(char)*22);
    nombreSalida=setNameSalida(i);
    guardarImagenMIA(imagen, bInfoHeader, header,nombreSalida);



    unsigned char* imagenGS = LoadBMP(nombreEntrada, &bInfoHeader, &header);
    char* nombreSalidaGS=malloc(sizeof(char)*22);
    nombreSalidaGS=setNameSalidaGS(i);
    guardarImagenGS(imagenGS, bInfoHeader, header,nombreSalidaGS);
    //printf("Nombre de salida: %s\n",salida);

  }
  return 0;
}
