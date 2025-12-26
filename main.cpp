/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include "Image.h"
#include "BMPheader.h"
#include "BMP.h"
#include <iostream>
#include <string>

int main()
{
    std::string filename;
    std::cout << "enter BMP filename: ";
    std::cin >> filename;

    BmpImage image;

    if (!image.load(filename))
    {
        std::cout << "failed to load image\n";
        return 1;
    }

    uint32_t memory = BmpImage::calculateRequiredMemory(image.getWidth(), image.getHeight());
    std::cout << "required memory for image: " << memory << " bytes\n";

    std::cout << "width: " << image.getWidth() << " pixels\n";;
    std::cout << "height: " << image.getHeight() << " pixels\n";
    std::cout << "image size: " << image.getSize() << " bytes\n";

    auto rotatedCW = image.rotateCW();
    if (rotatedCW->save("rotated_cw.bmp"))
    {
        std::cout << "saved as 'rotated_cw.bmp'\n";
    }

    auto rotatedCCW = image.rotateCCW();
    if (rotatedCCW->save("rotated_ccw.bmp"))
    {
        std::cout << "saved as 'rotated_ccw.bmp'\n";
    }

    rotatedCW->gaussianBlur();
    if (rotatedCW->save("gaussian_blurred.bmp"))
    {
        std::cout << "saved as 'gaussian_blurred.bmp'\n" << std::endl;
    }

    return 0;
}
