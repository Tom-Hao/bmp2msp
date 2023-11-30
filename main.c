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
    int windowsVersion; //Windows �汾�x��
    int ok; //�z��MSP�n���Ƿ�?��
    int size; //BMP�DƬ�ߴ�
    int width; //BMP�DƬ?��
    int height; //BMP�DƬ�߶�
    int bmpBitCount; //BMP�DƬÿ?���c����λԪ��
    int lineBytes; //һ����Ԫ�M��

    int i, j, a;

    unsigned char** bmpData; //BMP�n������
    unsigned char** mspData; //MSP�n������

    FILE* fp_bmp; //BMP�n��
    FILE* fp_msp; //MSP�n��
    BMPFILEHEADER bmpFileHeader; //BMP�n���^
    BMPINFOHEADER bmpInfoHeader; //BMP�Y?�^
    RGBQUAD rgbQuad; //�{ɫ�P

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

    printf("Please choose your Windows Version: 1 = 1.x 2 = 2.x.\n"); //�x��Windows�汾
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //�z���Ƿ��BMP�n��
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //�_�JBMP�DƬ?�ȣ��@?��ʽ�H֧Ԯ���ٶ�ʮ?���Լ�������ʮ?��֮�DƬ
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x000F || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //�_�JBMP�DƬ�߶ȣ��@?��ʽ�H֧Ԯ����?�أ�������ʮ?�أ��İ�?���Լ��İٰ�ʮ?��֮�DƬ
            { 
                if(windowsVersion == 1) //����Windows 1.x
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //��λ�DƬ������ʼλ��
                    
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
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount); //�D���D��
                    printf("Start writing.\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //?MSP��ʽ

                    if(ok == 0) //���ɹ�
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP write complete!\n");
                        return 0;
                    }
                    else //ʧ��
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP write fail!\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //����Windows 2.x
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("Sorry, cannot support this version temporarily.\n");
                    return 0;
                    
                   /*
                   fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //��λ�DƬ������ʼλ��
                    
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
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount); //�D���D��
                    printf("Start writing.\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //?MSP��ʽ

                    if(ok == 0) //���ɹ�
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP write complete!\n");
                        return 0;
                    }
                    else //ʧ��
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

