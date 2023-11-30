#ifndef BMP_HEAD
#define BMP_HEAD

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
//引用bmp头文件结构体
#include "bmp_head.h"


void showBmpFileHead(BMPFILEHEADER bmpFileHeader);

void showBmpFileInfo(BMPINFOHEADER bmpInfoHeader);

#endif