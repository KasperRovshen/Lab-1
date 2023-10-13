#pragma pack(push, 1)
struct BMPHeader {
    unsigned short signature;
    unsigned int fileSize;
    unsigned int reserved;
    unsigned int dataOffset;
    unsigned int headerSize;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitsPerPixel;
    unsigned int compression;
    unsigned int dataSize;
    unsigned int horizontalResolution;
    unsigned int verticalResolution;
    unsigned int colors;
    unsigned int importantColors;
    unsigned char *pixels;
};

#pragma pack(pop)

struct BMP {
    BMPHeader header;
    unsigned char *pixels;
};