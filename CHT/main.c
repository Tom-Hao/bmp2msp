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
    int windowsVersion; //Windows����
    int ok; //�ഫ�����T�{
    int size; //BMP�ɮפؤo
    int width; //BMP�ɮ׼e��
    int height; //BMP�ɮװ���
    int bmpBitCount; //BMP�C�ӵe��������bit��
    int lineBytes; //�@��ҥΦr���ռ�

    int i, j, a;

    unsigned char** bmpData; //BMP�ɮפ��e
    unsigned char** mspData; //MSP�ɮפ��e

    FILE* fp_bmp; //BMP�ɮ�
    FILE* fp_msp; //MSP�ɮ�
    BMPFILEHEADER bmpFileHeader; //BMP�ɮ׼��Y
    BMPINFOHEADER bmpInfoHeader; //BMP�ɮ�Info���Y
    RGBQUAD rgbQuad; //�զ�L

    printf("BMP2MSP 0.5.132 BETA��\n���v�Ҧ� (C) 2023-2024 ����n��}�o�� Tom Hao �s�@\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("�o�ӵ{���i�H�NBMP�W�椧�Ϲ��ഫ��Windows 1.x�M2.x��MS-Paint��MSP�W�椧�Ϲ��C\n �ϥ�'bmp2msp bmpfilename mspfilename'�R�O�Y�i�}�l�ഫ�C\n");
            return 0;
        }
        else
        {
            printf("�ݭn���w�ѼơA�ҡG'bmp2msp test.bmp test.msp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("�ݭn���w�ѼơA�ҡG'bmp2msp test.bmp test.msp'\n");
        return -1;

    }

    printf("��J��BMP�ɮ׬O %s �C\n", argv[1]);

    if((fp_bmp = fopen(argv[1], "rb")) == NULL)
    {
        printf("�X�{���~�ABMP�ɮ�Ū�����ѡC\n");
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

    printf("�п��Windows�����G 1 = 1.x 2 = 2.x.\n"); //Windows�������
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //�T�{�O�_��BMP�ɮ�
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //�T�{BMP�Ϲ����e�סA�u���ഫ�T�ʤG�̡����ʥ|�̵e�����Ϲ��C
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x000F || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //�T�{BMP�Ϲ������סA�u���ഫ�G�ʡ��G�ʥ|�̡��|�ʡ��|�ʤK�̵e�����Ϲ��C
            { 
                if(windowsVersion == 1) //�Y��Windows 1.x
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //BMP�Ϲ����e�w��
                    
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
                    printf("�ഫ�}�l�C\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount);
                    printf("�}�l�g�JMSP�C\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //MSP�禡

                    if(ok == 0) //�Y���\
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP�ɮ׳Ыا����I\n");
                        return 0;
                    }
                    else //�Y����
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP�ɮ׳Ыإ��ѡI\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //�Y��Windows 2.x
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("�ثe�o�ӵ{���ȮɵL�k�B�zWindows 2.x��MS-Paint��MSP�ɮסC\n");
                    return 0;
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("�]���ɮ׿��~���ɮ׷l�a�A�L�k�ഫ�C\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("�L�k�B�z�o�ذ��ת��Ϥ��C\n");
                return 0;
            }
        }
        else
        {
            fclose(fp_bmp);
            fclose(fp_msp);
            printf("�L�k�B�z�o�ؼe�ת��Ϥ��C\n");
            return 0;
        }
    }
    else
    {
        fclose(fp_bmp);
        fclose(fp_msp);
        printf("�]���ɮ׿��~���ɮ׷l�a�A�L�k�ഫ�C\n");
        return 0;
    }

}

