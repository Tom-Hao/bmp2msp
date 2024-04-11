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
    int windowsVersion; //Windowsのバージョン
    int ok; //変換完了するか否かを確認
    int size; //BMPファイルのサイズ
    int width; //BMPファイルの幅
    int height; //BMPファイルの高さ
    int bmpBitCount; //BMP毎画素対応ビット数
    int lineBytes; //一行のバイト数

    int i, j, a;

    unsigned char** bmpData; //BMPファイル内容
    unsigned char** mspData; //MSPファイル内容

    FILE* fp_bmp; //BMPファイル
    FILE* fp_msp; //MSPファイル
    BMPFILEHEADER bmpFileHeader; //BMPファイルの頭
    BMPINFOHEADER bmpInfoHeader; //BMPファイルのInfo頭
    RGBQUAD rgbQuad; //調色盤

    printf("BMP2MSP 0.5.132 BETA版\nCopyright (C) 2023-2024 中国軟体開発組 Tom Hao 製作\n");

    if(argc <= 2 || argc > 3)
    {
        //if(argv[1] == "/?")
        if(strcmp(argv[1], "/?") == 0)
        {
            printf("此のプログラムは、BMP画像をWindows 1.xと2.xのMS-PaintのMSP画像に変換出来ます。\n 'bmp2msp bmpfilename mspfilename'と言う命令で、変換開始します。\n");
            return 0;
        }
        else
        {
            printf("パラメータは必要です、例：'bmp2msp test.bmp test.msp'\n");
            return -1;
        }
    }

    if(argc == 0 || argc == 1)
    {
        printf("パラメータは必要です、例：'bmp2msp test.bmp test.msp'\n");
        return -1;

    }

    printf("此のBMPファイルは %s です。\n", argv[1]);

    if((fp_bmp = fopen(argv[1], "rb")) == NULL)
    {
        printf("エラーの為BMPファイル読み込めません。\n");
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

    printf("Windowsバージョンを選択して下さい： 1 = 1.x 2 = 2.x.\n"); //Windowsバージョンの選択
    scanf("%d", &windowsVersion);
    fflush(stdin);
    if(bmpFileHeader.bfType == 0x4d42) //BMPファイルかどうかの検査
    {
        if(bmpInfoHeader.biWidth == 0x0140 || bmpInfoHeader.biWidth == 0x0280) //BMP画像の幅を確認、唯三百二十・六百四十画素幅だけは変換出来ます。
        {
            if(bmpInfoHeader.biHeigth == 0x00C8 || bmpInfoHeader.biHeigth == 0x000F || bmpInfoHeader.biHeigth == 0x0190 || bmpInfoHeader.biHeigth == 0x01E0) //BMP画像の高さを確認、唯二百・二百四十・四百・四百八十画素の高さだけは変換出来ます。
            { 
                if(windowsVersion == 1) //Windows 1.xの場合
                {
                    fseek(fp_bmp, bmpFileHeader.bfOffBits, SEEK_SET); //BMP画像内容定位
                    
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
                    printf("変換開始。\n");
                    bmpTurn(bmpData, mspData, size, width, height, bmpBitCount);
                    printf("書き込み開始。\n");
                    ok = mspFiller(fp_msp, windowsVersion, width, height, mspData, width, height, lineBytes); //?MSP函式

                    if(ok == 0) //成功すれば
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSPファイル作成完了！\n");
                        return 0;
                    }
                    else //失敗すれば
                    {
                        free(bmpData);
                        free(mspData);
                        fclose(fp_bmp);
                        fclose(fp_msp);
                        printf("MSPファイル作成失敗！\n");
                        return 0;
                    }
                }
                else if(windowsVersion == 2) //Windows 2.xの場合
                {
                    
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("今此のプログラムは暫くWindows 2.xのMS-PaintのMSPファイルを処理出来ません。\n");
                    return 0;
                }
                else
                {
                    fclose(fp_bmp);
                    fclose(fp_msp);
                    printf("間違いファイル又はファイル破損ですので、変換出来ません。\n");
                    return 0;
                }
            }
            else
            {
                fclose(fp_bmp);
                fclose(fp_msp);
                printf("此の高さを処理出来ません。\n");
                return 0;
            }
        }
        else
        {
            fclose(fp_bmp);
            fclose(fp_msp);
            printf("此の幅を処理出来ません。\n");
            return 0;
        }
    }
    else
    {
        fclose(fp_bmp);
        fclose(fp_msp);
        printf("間違いファイル又はファイル破損ですので、変換出来ません。\n");
        return 0;
    }

}

