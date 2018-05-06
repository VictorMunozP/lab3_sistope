#include "structs.h"


int isInt(char* numero);
char* setNameInput(int a);
char* setNameOutput(int a);
char* setNameOutputGS(int a);
char* setNameOutputBin(int a);

unsigned char* loadImage(char *filename, bmpInfoHeader *bInfoHeader, bmpFileHeader *header);

void saveImage(unsigned char* array, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename);

unsigned char* rgbToGrayScale(unsigned char* array, bmpInfoHeader bInfoHeader);

//void saveImageGS(unsigned char* array, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename);

unsigned char* binarizeImage(unsigned char* array, bmpInfoHeader bInfoHeader,int umbral);

//void saveImageBin(unsigned char* array, bmpInfoHeader bInfoHeader, bmpFileHeader header, char* filename,int umbral);

char* nearlyBlack(unsigned char* array, bmpInfoHeader bInfoHeader,int umbral);
