#include "structs.h"

//void print();
int isInt(char* numero);
char* setNameImg(int a);
char* setNameSalida(int a);
char* setNameSalidaGS(int a);
char* setNameSalidaBin(int a);

unsigned char* LoadBMP(char *filename, bmpInfoHeader *bInfoHeader, bmpFileHeader *header);
Pixeles* transformarArray(unsigned char* array, bmpInfoHeader bInfoHeader);

void mostrarPixeles(Pixeles* pixeles);

unsigned char* transformarMatriz(Pixeles* pixeles);
void guardarImagen(Pixeles* pixeles, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename);
void guardarImagenMIA(unsigned char* array, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename);

unsigned char* rgbToGrayScale(unsigned char* array, bmpInfoHeader bInfoHeader);
void guardarImagenGS(unsigned char* array, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename);
void guardarImagenBin(unsigned char* array, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename,int umbral);
