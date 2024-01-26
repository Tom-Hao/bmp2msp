#ifndef BMP_H
#define BMP_H

#pragma pack(1)

typedef struct BMP_FILE_HEADER
{
    unsigned short bfType;
    unsigned long  bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned long  bfOffBits;
}BMPFILEHEADER;

typedef struct BMP_INFO_HEADER
{
    unsigned long  biInfoSize;
    long           biWidth;
    long           biHeigth;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned long  biCompression;
    unsigned long  biSizeImage;
    long           biXPelsPerMeter;
    long           biYPelsPerMeter;
    unsigned long  biClrUsed;
    unsigned long  biClrImportant;
}BMPINFOHEADER;

typedef struct RGB_QUAD
{
    unsigned long  rgbWhite;
    unsigned long  rgbBlack;
}RGBQUAD;

#endif

