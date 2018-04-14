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

int main(int argc, char** argv){
  int a, numImages,umbralBin,umbralClas;
  //char* tamano;
  while ((a = getopt (argc, argv, "bc:u:n:")) != -1){
    switch(a){
      case 'c':
          if(isInt(optarg)){
          numImages=atoi(optarg);
          printf("The number of images to process are: %i\n",numImages);

        }
        else{
          printf("The input has a character not valid (only integer digits!)\n");
          return 0;
        }

        break;
      case 'u':
        umbralBin=atoi(optarg);
        printf("UMBRAL to Bin is: %i\n",umbralBin);
        break;
      case 'n':
        umbralClas=atoi(optarg);
        printf("UMBRAL to classify is:%i\n",umbralClas);
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



  return 0;
}
