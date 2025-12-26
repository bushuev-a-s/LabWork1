/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include <exception>

#ifndef BMP_HEADER
#define BMP_HEADER

#include <fstream>
#include <cstdint>
#pragma pack(push,1)

struct BITMAPFILEHEADER
{
    uint16_t bfType; // type of file - 'BM' (2 bytes)
    uint32_t bfSize; // size of the image (4 bytes)
    uint16_t bfReserved1; // for future iteretions (2 bytes)
    uint16_t bfReserved2; // for future iteretions (2 bytes)
    uint32_t bfOffset; // get towards the beggining of file (pixels) (4 bytes);
};

struct BITMAPINFOHEADER
{
    uint32_t biSize; //struct size
    int32_t  biWidth; // struct wight
    int32_t  biHeight; // struct height
    uint16_t biPlanes; // plane number (always 1)
    uint16_t biBitCount; // 8R + 8G + 8B
    uint32_t biCompression; // without compression
    uint32_t biSizeImage; // image data size
    uint32_t biXPelsPerMeter; // X resolution (4 bytes)
    uint32_t biYPelsPerMeter; // Y resolution (4 bytes)
    uint32_t biClrUsed; // colors used (4 bytes)
    uint32_t biClrImportant; // important colors (4 bytes)
};

#pragma pack(pop)

class BmpHeader
{
private:
    BITMAPFILEHEADER BmpFile;
    BITMAPINFOHEADER BmpInfo;

public:
    BmpHeader() = default;
    BmpHeader(uint32_t width, uint32_t height);

    bool read(std::ifstream& file);
    bool write(std::ofstream& file) const;
    void updateForRotation(uint32_t newW, uint32_t newH);

    uint32_t getWidth() const
    {
        return static_cast<uint32_t>(BmpInfo.biWidth);
    }
    uint32_t getHeight() const
    {
        return static_cast<uint32_t>(BmpInfo.biHeight);
    }
    uint32_t getImageSize() const
    {
        return BmpInfo.biSizeImage;
    }
    uint32_t getDataOffset() const
    {
        return BmpFile.bfOffset;
    }
    uint16_t getBitsPerPixel() const
    {
        return BmpInfo.biBitCount;
    }


    void setWidth(uint32_t width)
    {
        BmpInfo.biWidth = static_cast<uint32_t>(width);
    }
    void setHeight(uint32_t height)
    {
        BmpInfo.biHeight = static_cast<uint32_t>(height);
    }
    void setImageSize(uint32_t size)
    {
        BmpInfo.biSizeImage = size;
    }

};

#endif
