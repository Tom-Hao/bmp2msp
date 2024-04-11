#define _CRT_SECURE_NO_WARNINGS

#include "mspFiller.h"

int mspFiller(FILE* fp, int windowsVersion, long biWidth, long biHeigth, unsigned char** mspData, int width, int height, int lineBytes)
{
    int i, j;
    short mspHead[16] = {0x00};
    /*
    if((fp = fopen("output.msp", "wb+")) == NULL)
    {
        return -1;
    }
    */

    //MSPÀÉ®×¼ÐÀY
    /*
    WORD  Key1;              Magic number    
    WORD  Key2;              Magic number    
    WORD  Width;             Width of the bitmap in pixels   
    WORD  Height;            Height of the bitmap in pixels   
    WORD  XARBitmap;         X Aspect ratio of the bitmap   
    WORD  YARBitmap;         Y Aspect ratio of the bitmap   
    WORD  XARPrinter;        X Aspect ratio of the printer   
    WORD  YARPrinter;        Y Aspect ratio of the printer   
    WORD  PrinterWidth;      Width of the printer in pixels   
    WORD  PrinterHeight;     Height of the printer in pixels   
    WORD  XAspectCorr;       X aspect correction (unused)     
    WORD  YAspectCorr;       Y aspect correction (unused)     
    WORD  Checksum;          Checksum of previous 24 bytes   
    WORD  Padding[3];        Unused padding    
    */
    if(windowsVersion == 1)
    {
        mspHead[0] = 0x6144;
        mspHead[1] = 0x4d6e;

        mspHead[12] = 0x2c2a;

    }
    else if(windowsVersion == 2)
    {
        mspHead[0] = 0x694c;
        mspHead[1] = 0x536e;

        mspHead[12] = 0x3a22;
    }

    mspHead[2] = biWidth;
    mspHead[3] = biHeigth;

    mspHead[4] = 0x000a;
    mspHead[5] = 0x000a;
    mspHead[6] = 0x000a;
    mspHead[7] = 0x000a;

    mspHead[8] = biWidth;
    mspHead[9] = biHeigth;

    mspHead[10] = 0x0000;
    mspHead[11] = 0x0000;

    mspHead[13] = 0x0000;
    mspHead[14] = 0x0000;
    mspHead[15] = 0x0000;

    fwrite(mspHead, 2, 16, fp);


    for(i = 0; i < height; i++)
    {
        for(j = 0; j < lineBytes; j++)
        {
            fwrite(&mspData[i][j], sizeof(unsigned char), 1, fp);
        }
    }
    
    //fclose(fp);
    return 0;
}