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
    int ok; //转换完成确认
    int size; //BMP文件尺寸
    int width; //BMP文件宽度
    int height; //BMP文件高度
    int bmpBitCount; //BMP每个像素对应的bit数
    int lineBytes; //一行字节数
    int bmpWhite;  //BMP图片白黑对应
    int bmpBlack;  //BMP图片白黑对应

    int i, j, a;

    unsigned char** bmpData; //BMP文件内容
    unsigned char** mspData; //MSP文件内容

    FILE* fp_bmp; //BMP文件
    FILE* fp_msp; //MSP文件
    BMPFILEHEADER bmpFileHeader; //BMP文件头
    BMPINFOHEADER bmpInfoHeader; //BMP文件Info头
    RGBQUAD rgbQuad; //调色盘

    printf("BMP2MSP 0.5.191 BETA版\n版权所有 (C) 2023-2024 Tom Hao 制作\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("这个程序可以将BMP格式图像转换为Windows 1.x和2.x的MS-Paint的MSP格式图像。\n 使用'bmp2msp bmpfilename mspfilename'命令即可开始转换。\n");
            return 0;
        }
        else
        {
            printf("需要指定参数，例：'bmp2msp test.bmp test.msp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("需要指定参数，例：'bmp2msp test.bmp test.msp'\n");
        return -1;

    }

    printf("输入的BMP文件是 %s 。\n", argv[1]);

    if((fp_bmp = fopen(argv[1], "rb")) == NULL)
    {
        printf("出现错误，BMP文件读取失败。\n");
        return -1;
    }

    if ((fp_msp = fopen(argv[2], "wb+")) == NULL)
    {
        return -1;
    }

    fseek(fp_bmp, 0, SEEK_SET);

    fread(&bmpFileHeader, 14, 1, fp_bmp);
    fread(&bmpInfoHeader, 40, 1, fp_bmp);
    fread(&rgbQuad, 8, 1, fp_bmp);
/*
    showBmpFileHead(bmpFileHeader);
    showBmpFileInfo(bmpInfoHeader);
    showBmpRgbQuad(rgbQuad);
*/

    printf("请选择Windows版本： 1 = 1.x 2 = 2.x.\n"); //Windows版本选择
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //确认是否为BMP文件
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //确认BMP图像的宽度，只能转换三百二十、六百四十像素的图片。
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x00F0 || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //确认BMP图像的高度，只能转换二百、二百四十、四百、四百八十像素的图片。
            { 
                if(windowsVersion == 1) //若为Windows 1.x
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //BMP图像内容定位
                    
                    size = bmpInfoHeader.biSizeImage;
                    width = bmpInfoHeader.biWidth;
                    height = bmpInfoHeader.biHeigth;
                    bmpBitCount = bmpInfoHeader.biBitCount;
                    bmpWhite = rgbQuad.rgbWhite;
                    bmpBlack = rgbQuad.rgbBlack;
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
                    printf("转换开始。\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount, bmpWhite, bmpBlack);
                    printf("开始写入MSP。\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //MSP函数

                    if(ok == 0) //若成功
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP文件创建完成！\n");
                        return 0;
                    }
                    else //若失败
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP文件创建失败！\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //若为Windows 2.x
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("目前这个程序暂时无法处理Windows 2.x的MS-Paint的MSP文件。\n");
                    return 0;
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("因为文件错误或文件损坏，无法转换。\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("无法处理这种高度的图片。\n");
                return 0;
            }
        }
        else
        {
            fclose(fp_bmp);
            fclose(fp_msp);
            printf("无法处理这种宽度的图片。\n");
            return 0;
        }
    }
    else
    {
        fclose(fp_bmp);
        fclose(fp_msp);
        printf("因为文件错误或文件损坏，无法转换。\n");
        return 0;
    }

}

