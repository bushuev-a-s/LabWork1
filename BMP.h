/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include "BMPheader.h"
//#include "Pixel.h"
#include <vector>

#ifndef BMP_H
#define BMP_H
class Bmp: public Image {
public:
    Bmp();
    ~Bmp();

    //void getMemoryAmount(const std::string &filename) override;
    void load(const std::string &filename) override;
    void save(const std::string &filename) override;
    void rotateRight() override;
    void rotateLeft() override;
    void applyGaussianFilter() override;
private:
    FileHeader _fileheader;
    InfoHeader _infoheader;
    std::vector<uint8_t> _pixels;
}
#endif
