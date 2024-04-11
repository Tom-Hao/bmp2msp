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
    int windowsVersion; //Windows�̃o�[�W����
    int ok; //�ϊ��������邩�ۂ����m�F
    int size; //BMP�t�@�C���̃T�C�Y
    int width; //BMP�t�@�C���̕�
    int height; //BMP�t�@�C���̍���
    int bmpBitCount; //BMP����f�Ή��r�b�g��
    int lineBytes; //��s�̃o�C�g��

    int i, j, a;

    unsigned char** bmpData; //BMP�t�@�C�����e
    unsigned char** mspData; //MSP�t�@�C�����e

    FILE* fp_bmp; //BMP�t�@�C��
    FILE* fp_msp; //MSP�t�@�C��
    BMPFILEHEADER bmpFileHeader; //BMP�t�@�C���̓�
    BMPINFOHEADER bmpInfoHeader; //BMP�t�@�C����Info��
    RGBQUAD rgbQuad; //���F��

    printf("BMP2MSP 0.5.132 BETA��\nCopyright (C) 2023-2024 ������̊J���g Tom Hao ����\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("���̃v���O�����́ABMP�摜��Windows 1.x��2.x��MS-Paint��MSP�摜�ɕϊ��o���܂��B\n 'bmp2msp bmpfilename mspfilename'�ƌ������߂ŁA�ϊ��J�n���܂��B\n");
            return 0;
        }
        else
        {
            printf("�p�����[�^�͕K�v�ł��A��F'bmp2msp test.bmp test.msp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("�p�����[�^�͕K�v�ł��A��F'bmp2msp test.bmp test.msp'\n");
        return -1;

    }

    printf("����BMP�t�@�C���� %s �ł��B\n", argv[1]);

    if((fp_bmp = fopen(argv[1], "rb")) == NULL)
    {
        printf("�G���[�̈�BMP�t�@�C���ǂݍ��߂܂���B\n");
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

    printf("Windows�o�[�W������I�����ĉ������F 1 = 1.x 2 = 2.x.\n"); //Windows�o�[�W�����̑I��
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //BMP�t�@�C�����ǂ����̌���
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //BMP�摜�̕����m�F�A�B�O�S��\�E�Z�S�l�\��f�������͕ϊ��o���܂��B
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x000F || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //BMP�摜�̍������m�F�A�B��S�E��S�l�\�E�l�S�E�l�S���\��f�̍��������͕ϊ��o���܂��B
            { 
                if(windowsVersion == 1) //Windows 1.x�̏ꍇ
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //BMP�摜���e���
                    
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
                    printf("�ϊ��J�n�B\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount);
                    printf("�������݊J�n�B\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //?MSP����

                    if(ok == 0) //���������
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP�t�@�C���쐬�����I\n");
                        return 0;
                    }
                    else //���s�����
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSP�t�@�C���쐬���s�I\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //Windows 2.x�̏ꍇ
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("�����̃v���O�����͎b��Windows 2.x��MS-Paint��MSP�t�@�C���������o���܂���B\n");
                    return 0;
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("�ԈႢ�t�@�C�����̓t�@�C���j���ł��̂ŁA�ϊ��o���܂���B\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("���̍����������o���܂���B\n");
                return 0;
            }
        }
        else
        {
            fclose(fp_bmp);
            fclose(fp_msp);
            printf("���̕��������o���܂���B\n");
            return 0;
        }
    }
    else
    {
        fclose(fp_bmp);
        fclose(fp_msp);
        printf("�ԈႢ�t�@�C�����̓t�@�C���j���ł��̂ŁA�ϊ��o���܂���B\n");
        return 0;
    }

}

