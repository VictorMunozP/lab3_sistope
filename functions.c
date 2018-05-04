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
#include <stdint.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <sys/wait.h>

/*
void print(){
  printf("hello world!!\n");
}
*/
int isInt(char* numero){
  /*VERIFICACION DE QUE EL ARGUMENTO numero SEA ENTERO*/
 for(int i=0;i<strlen(numero);i++){
   if(isdigit(numero[i])==0){
     return 0;
   }
 }
 return 1;
}

char* setNameImg(int a){
  char* nombreEntrada=malloc(sizeof(char)*20);
  strcpy(nombreEntrada,"image_");

  char* buffer=malloc(sizeof(char)*12);
  sprintf(buffer,"%d",a);

  char* formato=".bmp";

  char* nombreDef=malloc(sizeof(char)*22);
  nombreDef=strcat(nombreEntrada,buffer);
  //printf("nombre parcial: %s\n",nombreDef);
  nombreDef=strcat(nombreDef,formato);
  //printf("%s\n",nombreDef);
  return nombreDef;
}

char* setNameSalida(int a){
  char* nombreEntrada=malloc(sizeof(char)*20);
  strcpy(nombreEntrada,"salida_");

  char* buffer=malloc(sizeof(char)*12);
  sprintf(buffer,"%d",a);

  char* formato=".bmp";

  char* nombreDef=malloc(sizeof(char)*22);
  nombreDef=strcat(nombreEntrada,buffer);
  //printf("nombre parcial: %s\n",nombreDef);
  nombreDef=strcat(nombreDef,formato);
  //printf("%s\n",nombreDef);
  return nombreDef;
}

unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader, bmpFileHeader *header)
{

  FILE *f;
 						     /* cabecera */
  unsigned char *imgdata;   /* datos de imagen */
  uint16_t type;        /* 2 bytes identificativos */

  f=fopen (filename, "r");
  if (!f)
    return NULL;        /* Si no podemos leer, no hay imagen*/

  /* Leemos los dos primeros bytes */
  fread(&type, sizeof(uint16_t), 1, f);
  if (type !=0x4D42)        /* Comprobamos el formato */
    {
      fclose(f);
      return NULL;
    }

  /* Leemos la cabecera de fichero completa */
  fread(header, sizeof(bmpFileHeader), 1, f);

  /* Leemos la cabecera de información completa */
  fread(bInfoHeader, sizeof(bmpInfoHeader), 1, f);

  /* Reservamos memoria para la imagen, ¿cuánta?
     Tanto como indique imgsize */
  imgdata=(unsigned char*)malloc(bInfoHeader->imgsize);

  /* Nos situamos en el sitio donde empiezan los datos de imagen,
   nos lo indica el offset de la cabecera de fichero*/
  fseek(f, header->offset, SEEK_SET);

  /* Leemos los datos de imagen, tantos bytes como imgsize */
  fread(imgdata, bInfoHeader->imgsize,1, f);

  /* Cerramos */
  fclose(f);

  /* Devolvemos la imagen */
  return imgdata;
}

//Funcion que transforme el array de unsigned char a una matriz de enteros 3d
Pixeles* transformarArray(unsigned char* array, bmpInfoHeader bInfoHeader){
	int largo = (int) bInfoHeader.height;
	int ancho = (int) bInfoHeader.width;
	int i,j;
	//Usamos un indice para recorrer el array a medida que recorremos la matriz
	int indice =0;
	//Creamos la estructura de datos
	Pixeles* pixeles = malloc(sizeof(Pixeles));
	if(pixeles==NULL){
		printf("Error de memoria en la creacion de la matriz de pixeles!!\n");
		return NULL;
	}
	pixeles->ancho= ancho;
	pixeles->largo = largo,
	//Reservamos memoria para las filas de la matriz de pixeles
	pixeles->matrizPixeles = malloc(sizeof(int**)*largo);
	//Si es  nula, se aborta la creacion
	if(pixeles->matrizPixeles==NULL){
		printf("Error en la creacion de las filas de la matriz\n");
		return NULL;
	}
	//Reservamos la memoria para las columnas de la matriz
	//Es decir, los elementos de cada fila de la matriz
	for(i=0; i < largo; i++){
		pixeles->matrizPixeles[i] = malloc(sizeof(int*)*ancho);
		//Si hay algun error, se aborta
		if(pixeles->matrizPixeles[i]==NULL){
			printf("Error en la creacion de las columnas de la matriz\n");
			return NULL;
		}
		for(j=0; j < ancho; j++){
			//Cada elemento de la matriz es un arreglo de 3 elementos
			//los componentes r,g,b de un pixel
			pixeles->matrizPixeles[i][j]= malloc(sizeof(int)*3);
			//si alguna componente del pixel resulta nula, abortamos
			if(pixeles->matrizPixeles[i][j]== NULL){
				printf("Error en la creacion de los componentes de un pixel de la matriz\n");
				return NULL;
			}
		}
	}
	//Recorremos el arreglo con las dimensiones de ancho y largo como si fuera una matriz
	for(i=0; i < largo; i++){
		for(j=0; j < ancho; j++){
			//cópiamos la componente roja
			pixeles->matrizPixeles[i][j][0] = array[indice];
			//Avanzamos a la componente verde en el array
			indice++;
			//copiamos la componente verde
			pixeles->matrizPixeles[i][j][1] = array[indice];
			//Avanzamos a la componente azul
			indice++;
			//Copiamos la componente azul
			pixeles->matrizPixeles[i][j][2] = array[indice];
			indice++;
		}
	}
	//retornamos la matriz de pixeles de 3 dimensiones
	return pixeles;
}

//procedimiento que muestra una matriz de pixeles por pantalla
// recibe como parametro una matriz de 3 dimensiones
void mostrarPixeles(Pixeles* pixeles){
	int i,j;
	for(i=0; i< pixeles->largo; i++){
		for(j=0; j< pixeles->ancho; j++){
			printf("[%d %d %d] ", pixeles->matrizPixeles[i][j][0], pixeles->matrizPixeles[i][j][1], pixeles->matrizPixeles[i][j][2]);
		}
		printf("\n");
	}
}


//Funcion que transformar una matriz de pixeles en un arreglo de unsigned char
unsigned char* transformarMatriz(Pixeles* pixeles){
	//Se debe poner cada componente de un pixel dentro del arreglo
	//por lo que la dimension del arreglo es: anchox x largo x 3
	int largo = pixeles->largo;
	int ancho = pixeles->ancho;
	int dimension = largo*ancho*3;
	unsigned char* array = malloc(sizeof(unsigned char)*dimension);
	//Recorremos la matriz de pixeles y vamos asignando componente a componente
	//cada elemento del array
	//para recorrer el array dentro del recorrido de la matriz, usamos un indice
	int indice=0;
	int i,j;
	for(i=0; i < largo; i++){
		for(j=0; j < ancho; j++){
			//cópiamos la componente roja de la matriz al arreglo
			array[indice] = pixeles->matrizPixeles[i][j][0];
			//Avanzamos a la componente verde en el arreglo
			indice++;
			//copiamos la componente verde de la matriz al arreglo
			array[indice] = pixeles->matrizPixeles[i][j][1];
			//Avanzamos a la componente azul en el arreglo
			indice++;
			//Copiamos la componente azul de la matriz al arreglo
			array[indice] = pixeles->matrizPixeles[i][j][2];
			indice++;
		}
	}
	//retornamos el array de unsigned char
	return array;
}


//Funcion que guarda una matriz de pixeles en formato bmp
void guardarImagen(Pixeles* pixeles, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename){
	//Recibimos el mismo header de informacion y de archivo de la imagen original
	//debemos cambiar el size del header.
	//---> size = tamaño total del archivo --> size of(header + bInfoHeader + datos de la imagen)
	//y en bInfoHeader debemos cambiar imgsize, width y height.
	//----_> imgsize = ancho*largo*3 (3bytes por pixel).
	int ancho = pixeles->ancho;
	int largo = pixeles->largo;
	int image_size = ancho*largo*3;
	//modificamos primero la cabecera de info de la imagen
	bInfoHeader.width = (uint32_t) ancho;
	bInfoHeader.height = (uint32_t) largo;
	bInfoHeader.imgsize = (uint32_t) image_size;
	//Luego modificamos el header del archivo
	header.size = (uint32_t) (sizeof(bInfoHeader) + sizeof(header) + sizeof(image_size));
	//Abrimos el archivo de la nueva imagen
	FILE* imagen = fopen(filename, "w");
	if(imagen==NULL){
		printf("Error de memoria en la creacion del archivo de imagen de salida.\n");
		return;
	}
	//Escribimos el tipo de archivo (BM)
	uint16_t type = 0x4D42;
	fwrite(&type, sizeof(uint16_t), 1, imagen);
	//Escribimos la cabecera del archivo completa
	fwrite(&header, sizeof(bmpFileHeader),1,imagen);
	//Escribimos la cabecera de info de la imagen completa
	fwrite(&bInfoHeader, sizeof(bmpInfoHeader),1,imagen);
	//obtenemos el array de datos de la imagen
	unsigned char* datos_imagen = transformarMatriz(pixeles);
	//Escribimos el arreglo de datos de la imagen en el archivo
	//Nos movemos a la parte dle archivo en donde deben ir los datos, segun el header del archivo
	fseek(imagen, header.offset, SEEK_SET);
	//escribimos los datos de la imagen
	fwrite(datos_imagen, bInfoHeader.imgsize,1, imagen);
	//cerramos el archivo de salida
	fclose(imagen);
}
