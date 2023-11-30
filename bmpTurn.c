#define _CRT_SECURE_NO_WARNINGS

#include "bmpTurn.h"

void bmpTurn(unsigned char** bmpData, unsigned char** mspData, int bmpSize, int width, int height, int bmpBitCount)
{
    int i, j, k;
    int lineBytes = (bmpBitCount * width) / 8; //一行字元組數
    unsigned char** tempData;

    tempData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
    for(k = 0; k < height; k++)
    {
        tempData[k] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
    }

    for(i = 0; i < height; i++) //行
    {
        for(j = 0; j < lineBytes; j++) //列
        {
            //*(mspData + i + j) = *(tempData + (height - 1 - i) + j);
            mspData[i][j] = bmpData[height - 1 - i][j];
        }
    }

    free(tempData);
}