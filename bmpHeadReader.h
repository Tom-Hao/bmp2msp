#ifndef BMP_HEAD
#define BMP_HEAD

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
//load msp head file structure
#include "bmp_head.h"


void showBmpFileHead(BMPFILEHEADER bmpFileHeader);

void showBmpFileInfo(BMPINFOHEADER bmpInfoHeader);

#endif