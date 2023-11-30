#define _CRT_SECURE_NO_WARNINGS

#include "bmpHeadReader.h"

void showBmpFileHead(BMPFILEHEADER bmpFileHeader)
{
    printf("BMP文件識別字：%x\n", bmpFileHeader.bfType);
    printf("文件大小：%ld\n", bmpFileHeader.bfSize);
    printf("圖像實際位置：%lx\n", bmpFileHeader.bfOffBits);
}

void showBmpFileInfo(BMPINFOHEADER bmpInfoHeader)
{
    printf("BMP信息頭\n");
    printf("資訊頭大小：%d\n", bmpInfoHeader.biInfoSize);
    printf("點陣圖寬度：%d\n", bmpInfoHeader.biWidth);
    printf("點陣圖高度：%d\n", bmpInfoHeader.biHeigth);
    printf("圖像的位元面數：%d\n", bmpInfoHeader.biPlanes);
    printf("每個圖元的位元數：%d\n", bmpInfoHeader.biBitCount);
    printf("壓縮方式：%d\n", bmpInfoHeader.biCompression);
    printf("圖像的大小：%d\n", bmpInfoHeader.biSizeImage);
    printf("水準方向解析度：%d\n", bmpInfoHeader.biXPelsPerMeter);
    printf("垂直方向解析度：%d\n", bmpInfoHeader.biYPelsPerMeter);
    printf("使用的顏色數：%d\n", bmpInfoHeader.biClrUsed);
    printf("重要顏色數：%d\n", bmpInfoHeader.biClrImportant);
}
