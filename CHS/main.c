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
    int windowsVersion; //Windows�汾
    int ok; //ת�����ȷ��
    int size; //BMP�ļ��ߴ�
    int width; //BMP�ļ����
    int height; //BMP�ļ��߶�
    int bmpBitCount; //BMPÿ�����ض�Ӧ��bit��
    int lineBytes; //һ���ֽ���
    int bmpWhite;  //BMPͼƬ�׺ڶ�Ӧ
    int bmpBlack;  //BMPͼƬ�׺ڶ�Ӧ

    int i, j, a;

    unsigned char** bmpData; //BMP�ļ�����
    unsigned char** mspData; //MSP�ļ�����

    FILE* fp_bmp; //BMP�ļ�
    FILE* fp_msp; //MSP�ļ�
    BMPFILEHEADER bmpFileHeader; //BMP�ļ�ͷ
    BMPINFOHEADER bmpInfoHeader; //BMP�ļ�Infoͷ
    RGBQUAD rgbQuad; //��ɫ��

    printf("BMP2MSP 0.5.191 BETA��\n��Ȩ���� (C) 2023-2024 Tom Hao ����\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("���������Խ�BMP��ʽͼ��ת��ΪWindows 1.x��2.x��MS-Paint��MSP��ʽͼ��\n ʹ��'bmp2msp bmpfilename mspfilename'����ɿ�ʼת����\n");
            return 0;
        }
        else
        {
            printf("��Ҫָ������������'bmp2msp test.bmp test.msp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("��Ҫָ������������'bmp2msp test.bmp test.msp'\n");
        return -1;

    }

    printf("�����BMP�ļ��� %s ��\n", argv[1]);

    if((fp_bmp = fopen(argv[1], "rb")) == NULL)
    {
        printf("���ִ���BMP�ļ���ȡʧ�ܡ�\n");
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

    printf("��ѡ��Windows�汾�� 1 = 1.x 2 = 2.x.\n"); //Windows�汾ѡ��
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //ȷ���Ƿ�ΪBMP�ļ�
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //ȷ��BMPͼ��Ŀ�ȣ�ֻ��ת�����ٶ�ʮ��������ʮ���ص�ͼƬ��
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x00F0 || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //ȷ��BMPͼ��ĸ߶ȣ�ֻ��ת�����١�������ʮ���İ١��İٰ�ʮ���ص�ͼƬ��
            { 
                if(windowsVersion == 1) //��ΪWindows 1.x
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //BMPͼ�����ݶ�λ
                    
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
                    printf("ת����ʼ��\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount, bmpWhite, bmpBlack);
                    printf("��ʼд��MSP��\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //MSP����

                    if(ok == 0) //���ɹ�
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP�ļ�������ɣ�\n");
                        return 0;
                    }
                    else //��ʧ��
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP�ļ�����ʧ�ܣ�\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //��ΪWindows 2.x
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("Ŀǰ���������ʱ�޷�����Windows 2.x��MS-Paint��MSP�ļ���\n");
                    return 0;
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("��Ϊ�ļ�������ļ��𻵣��޷�ת����\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("�޷��������ָ߶ȵ�ͼƬ��\n");
                return 0;
            }
        }
        else
        {
            fclose(fp_bmp);
            fclose(fp_msp);
            printf("�޷��������ֿ�ȵ�ͼƬ��\n");
            return 0;
        }
    }
    else
    {
        fclose(fp_bmp);
        fclose(fp_msp);
        printf("��Ϊ�ļ�������ļ��𻵣��޷�ת����\n");
        return 0;
    }

}

