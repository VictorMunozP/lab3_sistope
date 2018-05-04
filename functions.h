#include "structs.h"

//void print();
int isInt(char* numero);
char* setNameImg(int a);
char* setNameSalida(int a);

unsigned char *LoadBMP(char *filename, bmpInfoHeader *bInfoHeader, bmpFileHeader *header);
Pixeles* transformarArray(unsigned char* array, bmpInfoHeader bInfoHeader);

void mostrarPixeles(Pixeles* pixeles);

unsigned char* transformarMatriz(Pixeles* pixeles);
void guardarImagen(Pixeles* pixeles, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename);
