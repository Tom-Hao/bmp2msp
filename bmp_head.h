#ifndef BMP_H
#define BMP_H

#pragma pack(1)

typedef struct BMP_FILE_HEADER
{
    unsigned short bfType; //文件識別字
    unsigned long  bfSize; //文件尺寸
    unsigned short bfReserved1; //保留字，固定為0
    unsigned short bfReserved2; //保留字，固定為0
    unsigned long  bfOffBits; //影像檔起始位址
}BMPFILEHEADER;

typedef struct BMP_INFO_HEADER
{
    unsigned long  biInfoSize; //資訊頭大小
    long           biWidth; //圖像寬度
    long           biHeigth; //圖像高度
    unsigned short biPlanes; //圖像位元面數
    unsigned short biBitCount; //每個圖元的位元數
    unsigned long  biCompression; //壓縮類型
    unsigned long  biSizeImage; //圖像大小，位元組為單位
    long           biXPelsPerMeter; //水準解析度
    long           biYPelsPerMeter; //垂直解析度
    unsigned long  biClrUsed; //使用色彩數
    unsigned long  biClrImportant; //重要顏色數
}BMPINFOHEADER;

typedef struct RGB_QUAD
{
    unsigned long  rgbWhite; //白色
    unsigned long  rgbBlack; //黑色
}RGBQUAD;

#endif

