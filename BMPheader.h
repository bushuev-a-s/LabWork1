/*
 * Alexander Bushuev
 * st140044@student.spbu.ru
 * Lab work 1
 */

#ifndef BMP_HEADER
#define BMP_HEADER
#pragma pack(push, 1)

struct FileHeader {
    uint16_t bfType;          // 'BM'
    uint32_t bfSize;          // Размер файла
    uint16_t bfReserved1;     // Зарезервировано
    uint16_t bfReserved2;     // Зарезервировано
    uint32_t bfOffBits;       // Смещение до пиксельных данных
};

struct InfoHeader {
    uint32_t biSize;          // Размер структуры
    int32_t biWidth;          // Ширина изображения
    int32_t biHeight;         // Высота изображения
    uint16_t biPlanes;        // Количество плоскостей (1)
    uint16_t biBitCount;      // Битовая глубина (24 для RGB)
    uint32_t biCompression;   // Способ сжатия (BI_RGB = 0)
    uint32_t biSizeImage;     // Размер пиксельных данных
    int32_t biXPelsPerMeter;  // Горизонтальное разрешение
    int32_t biYPelsPerMeter;  // Вертикальное разрешение
    uint32_t biClrUsed;       // Количество используемых цветов
    uint32_t biClrImportant;  // Количество важных цветов
};

#pragma pack(pop)
#endif
