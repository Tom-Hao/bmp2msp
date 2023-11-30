#ifndef MSPFILLER
#define MSPFILLER
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

int mspFiller(FILE* fp, int windowsVersion, long biWidth, long biHeigth, unsigned char** mspData, int width, int height, int lineBytes);

#endif