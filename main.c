#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "bmp_head.h"
#include "bmpHeadReader.h"
#include "mspFiller.h"
#include "bmpTurn.h"

int main(int argc, char* argv[])
{
    int windowsVersion; //Windows 版本選擇
    int ok; //檢查MSP檔案是否?完
    int size; //BMP圖片尺寸
    int width; //BMP圖片?度
    int height; //BMP圖片高度
    int bmpBitCount; //BMP圖片每?素點對應位元數
    int lineBytes; //一行字元組數

    int i, j, a;

    unsigned char** bmpData; //BMP檔案內容
    unsigned char** mspData; //MSP檔案內容

    FILE* fp_bmp; //BMP檔案
    FILE* fp_msp; //MSP檔案
    BMPFILEHEADER bmpFileHeader; //BMP檔案頭
    BMPINFOHEADER bmpInfoHeader; //BMP資?頭
    RGBQUAD rgbQuad; //調色盤

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("This program can convert your bmp picture to msp picture, which is the format for MS-Paint on Windows 1.x or Windows 2.x.\n To use this program, you can type the command like this:\n 'bmp2msp bmpfilename mspfilename',\n and then the program will convert the bmpfilename to mspfilename with the name you typed.\n");
            return 0;
        }
        else
        {
            printf("Please run with parameter, e.g. 'bmp2msp test.bmp test.msp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("Please run with parameter, e.g. 'bmp2msp test.bmp test.msp'\n");
        return -1;

    }

    printf("The BMP file is %s.\n", argv[1]);

    if((fp_bmp = fopen(argv[1], "rb")) == NULL)
    {
        printf("Read bmp file error.\n");
        return -1;
    }

    if ((fp_msp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_bmp, 0, SEEK_SET);

    fread(&bmpFileHeader, 14, 1, fp_bmp);
    fread(&bmpInfoHeader, 40, 1, fp_bmp);
/*
    showBmpFileHead(bmpFileHeader);
    showBmpFileInfo(bmpInfoHeader);
*/

    printf("Please choose your Windows Version: 1 = 1.x 2 = 2.x.\n"); //選擇Windows版本
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //檢查是否為BMP檔案
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //確認BMP圖片?度，這?程式僅支援三百二十?素以及六百四十?素之圖片
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x000F || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //確認BMP圖片高度，這?程式僅支援二百?素／二百四十?素／四百?素以及四百八十?素之圖片
            { 
                if(windowsVersion == 1) //若為Windows 1.x
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //定位圖片內容起始位置
                    
                    size = bmpInfoHeader.biSizeImage;
                    width = bmpInfoHeader.biWidth;
                    height = bmpInfoHeader.biHeigth;
                    bmpBitCount = bmpInfoHeader.biBitCount;
                    lineBytes = (bmpBitCount * width) / 8;
                    bmpData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
                    for(i = 0; i < height; i++)
                    {
                        bmpData[i] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
                    }

                    mspData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
                    for(i = 0; i < height; i++)
                    {
                        mspData[i] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
                    }
                    for(i = 0; i < height; i++)
                    {
                        for(j = 0; j < lineBytes; j++)
                        {
                            fread(&bmpData[i][j], sizeof(unsigned char), 1, fp_bmp);
                        }
                    }
                    printf("Start converting.\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount); //圖像轉置
                    printf("Start writing.\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //?MSP函式

                    if(ok == 0) //若成功
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP write complete!\n");
                        return 0;
                    }
                    else //失敗
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP write fail!\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //若為Windows 2.x
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("Sorry, cannot support this version temporarily.\n");
                    return 0;
                    
                   /*
                   fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //定位圖片內容起始位置
                    
                    size = bmpInfoHeader.biSizeImage;
                    width = bmpInfoHeader.biWidth;
                    height = bmpInfoHeader.biHeigth;
                    bmpBitCount = bmpInfoHeader.biBitCount;
                    lineBytes = (bmpBitCount * width) / 8;
                    bmpData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
                    for(i = 0; i < height; i++)
                    {
                        bmpData[i] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
                    }

                    mspData = (unsigned char**)malloc(sizeof(unsigned char*) * height);
                    for(i = 0; i < height; i++)
                    {
                        mspData[i] = (unsigned char*)malloc(sizeof(unsigned char) * lineBytes);
                    }
                    for(i = 0; i < height; i++)
                    {
                        for(j = 0; j < lineBytes; j++)
                        {
                            fread(&bmpData[i][j], sizeof(unsigned char), 1, fp_bmp);
                        }
                    }
                    printf("Start converting.\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount); //圖像轉置
                    printf("Start writing.\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //?MSP函式

                    if(ok == 0) //若成功
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP write complete!\n");
                        return 0;
                    }
                    else //失敗
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP write fail!\n");
                        return 0;
                    }
                    */
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("Illegal input!\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("This program cannot support this size of the BMP file.\n");
                return 0;
            }
        }
        else
        {
            fclose(fp_bmp);
            fclose(fp_msp);
            printf("This program cannot support this size of the BMP file.\n");
            return 0;
        }
    }
    else
    {
        fclose(fp_bmp);
        fclose(fp_msp);
        printf("This is not a BMP file.\n");
        return 0;
    }

}

