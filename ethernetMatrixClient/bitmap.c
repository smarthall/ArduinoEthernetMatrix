#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct tagBITMAPFILEHEADER {
    uint16_t type;
    uint32_t size;
    uint16_t reserved1;
    uint16_t reserved2;
    uint32_t dataOffset;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    uint32_t size;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitCount;
    uint32_t compression;
    uint32_t fileSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colourUsed;
    uint32_t colourImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
    uint8_t blue;
    uint8_t green;
    uint8_t red;
    uint8_t reserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER header;
    RGBQUAD colours[];
} BITMAPINFO;

