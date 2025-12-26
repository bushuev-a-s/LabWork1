/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#include <exception>
#include <string>

#ifndef IMAGE_H
#define IMAGE_H
class Image {
public:
    virtual ~Image() = default;

    //virtual void getMemoryAmount(const std::string &filename) = 0;
    virtual void load(const std::string &filename) = 0;
    virtual void save(const std::string &filename) = 0;
    virtual void rotateRight() = 0;
    virtual void rotateLeft() = 0;
    virtual void applyGaussianFilter() = 0;
};
#endif
