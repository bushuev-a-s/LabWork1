/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include <exception>

#ifndef BMP_H
#define BMP_H

#include "Image.h"
#include "BMPheader.h"
#include <vector>


class BmpImage : public Image
{
private:
    BmpHeader header;
    std::vector<uint8_t> pixelData;

    uint32_t calculateRowSize() const;
    uint32_t calculatePadding() const;

public:
    BmpImage() = default;

    bool load(const std::string& filename) override;
    bool save(const std::string& filename) const override;
    std::unique_ptr<Image> rotateCW() const override;
    std::unique_ptr<Image> rotateCCW() const override;
    void gaussianBlur() override;


    uint32_t getWidth() const override
    {
        return header.getWidth();
    }
    uint32_t getHeight() const override
    {
        return header.getHeight();
    }
    uint32_t getSize() const override
    {
        uint32_t rowSize = calculateRowSize();
        return rowSize * getHeight();
    }

    void setPixel(uint32_t x, uint32_t y, uint8_t r, uint8_t g, uint8_t b);
    void getPixel(uint32_t x, uint32_t y, uint8_t& r, uint8_t& g, uint8_t& b) const;
    static uint32_t calculateRequiredMemory(uint32_t width, uint32_t height);
};
#endif
