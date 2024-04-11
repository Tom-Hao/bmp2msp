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
    int windowsVersion; //Windows版本
    int ok; //轉換完成確認
    int size; //BMP檔案尺寸
    int width; //BMP檔案寬度
    int height; //BMP檔案高度
    int bmpBitCount; //BMP每個畫素對應的bit數
    int lineBytes; //一行所用字元組數

    int i, j, a;

    unsigned char** bmpData; //BMP檔案內容
    unsigned char** mspData; //MSP檔案內容

    FILE* fp_bmp; //BMP檔案
    FILE* fp_msp; //MSP檔案
    BMPFILEHEADER bmpFileHeader; //BMP檔案標頭
    BMPINFOHEADER bmpInfoHeader; //BMP檔案Info標頭
    RGBQUAD rgbQuad; //調色盤

    printf("BMP2MSP 0.5.132 BETA版\n版權所有 (C) 2023-2024 中國軟體開發組 Tom Hao 製作\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("這個程式可以將BMP規格之圖像轉換為Windows 1.x和2.x的MS-Paint的MSP規格之圖像。\n 使用'bmp2msp bmpfilename mspfilename'命令即可開始轉換。\n");
            return 0;
        }
        else
        {
            printf("需要指定參數，例：'bmp2msp test.bmp test.msp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("需要指定參數，例：'bmp2msp test.bmp test.msp'\n");
        return -1;

    }

    printf("輸入之BMP檔案是 %s 。\n", argv[1]);

    if((fp_bmp = fopen(argv[1], "rb")) == NULL)
    {
        printf("出現錯誤，BMP檔案讀取失敗。\n");
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

    printf("請選擇Windows版本： 1 = 1.x 2 = 2.x.\n"); //Windows版本選擇
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //確認是否為BMP檔案
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //確認BMP圖像的寬度，只能轉換三百二十／六百四十畫素之圖像。
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x000F || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //確認BMP圖像的高度，只能轉換二百／二百四十／四百／四百八十畫素之圖像。
            { 
                if(windowsVersion == 1) //若為Windows 1.x
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //BMP圖像內容定位
                    
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
                    printf("轉換開始。\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount);
                    printf("開始寫入MSP。\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //MSP函式

                    if(ok == 0) //若成功
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP檔案創建完成！\n");
                        return 0;
                    }
                    else //若失敗
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP檔案創建失敗！\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //若為Windows 2.x
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("目前這個程式暫時無法處理Windows 2.x的MS-Paint的MSP檔案。\n");
                    return 0;
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("因為檔案錯誤或檔案損壞，無法轉換。\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("無法處理這種高度的圖片。\n");
                return 0;
            }
        }
        else
        {
            fclose(fp_bmp);
            fclose(fp_msp);
            printf("無法處理這種寬度的圖片。\n");
            return 0;
        }
    }
    else
    {
        fclose(fp_bmp);
        fclose(fp_msp);
        printf("因為檔案錯誤或檔案損壞，無法轉換。\n");
        return 0;
    }

}

