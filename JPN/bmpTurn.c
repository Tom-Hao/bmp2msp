#define _CRT_SECURE_NO_WARNINGS

#include "bmpTurn.h"

void bmpTurn(unsigned char** bmpData, unsigned char** mspData, int bmpSize, int width, int height, int bmpBitCount, int bmpWhite, int bmpBlack)
{
    int i, j, k;
    int lineBytes = (bmpBitCount * width) / 8;
    unsigned char** tempData;

    tempData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
    for(k = 0; k < height; k++)
    {
        tempData[k] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
    }

    if(bmpWhite == 0x0 && bmpBlack == 0xFFFFFF)
    {
        for(i = 0; i < height; i++) //s
        {
            for(j = 0; j < lineBytes; j++) //—ñ
            {
                //*(mspData + i + j) = *(tempData + (height - 1 - i) + j);
                mspData[i][j] = bmpData[height - 1 - i][j];
            }
        }
    }
    else if(bmpWhite == 0xFFFFFF && bmpBlack == 0x0)
    {
        for(i = 0; i < height; i++) //s
        {
            for(j = 0; j < lineBytes; j++) //—ñ
            {
                //*(mspData + i + j) = *(tempData + (height - 1 - i) + j);
                mspData[i][j] = 0xFF - bmpData[height - 1 - i][j];
            }
        }
    }

    free(tempData);
}