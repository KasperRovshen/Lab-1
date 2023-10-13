//Чарыев Бабаровшен

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include "bmph.h"

using namespace std;

int calculateNewSize(BMPHeader header) {
        int rowSize = header.width * 3;
        int paddedRowSize = rowSize;
        if (rowSize % 4 != 0) {
        paddedRowSize += 4 - (rowSize % 4); 
    }
    return paddedRowSize;
}


BMP readbmp(string filename) {
    BMP image;
    setlocale(LC_ALL, "Rus"); 

    ifstream inputFile(filename, ios::binary);
    if (!inputFile) {
        throw runtime_error("Не удается открыть BMP-файл для чтения.");
    }

    inputFile.read(reinterpret_cast<char*>(&image.header), sizeof(BMPHeader));

    int sizeRow = calculateNewSize(image.header);
    int sizePadded = sizeRow * image.header.height;

    image.pixels = new unsigned char[sizePadded];
    inputFile.read(reinterpret_cast<char*>(image.pixels), sizePadded);

    inputFile.close();
    return image;
}

void writebmp(string filename, BMP image) {
    ofstream outputFile(filename, ios::binary);
    setlocale(LC_ALL, "Rus"); 
    if (!outputFile) {
        throw runtime_error("Не удалось создать BMP-файл для записи.");
    }

 
    outputFile.write(reinterpret_cast<char*>(&image.header), sizeof(BMPHeader));

 

    int sizeRow = calculateNewSize(image.header);
    int paddedRowSize = (sizeRow + 3) / 4 * 4; 
    unsigned char* rowBuffer = new unsigned char[paddedRowSize];

   
    for (int y = 0; y < image.header.height; y++) {
        
        for (int x = 0; x < sizeRow; x++) {
            rowBuffer[x] = image.pixels[y * sizeRow + x];
        }

        
        for (int x = sizeRow; x < paddedRowSize; x++) {
            rowBuffer[x] = 0;
        }

        
        outputFile.write(reinterpret_cast<char*>(rowBuffer), paddedRowSize);
    }

    
    delete[] rowBuffer;

    outputFile.close();
}

BMP rotateRight(BMP image) {
    BMP newImage;

    int Height = image.header.height;
    int Weight = image.header.width;

    
    newImage.header = image.header;
    newImage.header.width = Height;
    newImage.header.height = Weight;

    int newSizeRow = calculateNewSize(newImage.header);
    int newPaddedSize = newSizeRow * newImage.header.height;

    newImage.header.fileSize = sizeof(BMPHeader) + newPaddedSize;
    newImage.header.dataSize = newPaddedSize;

    newImage.pixels = new unsigned char[newPaddedSize];

    for (int x = 0; x < Weight; x++) {
    for (int y = 0; y < Height; y++) {
        for (int channel = 0; channel < 3; channel++) {
            // Calculate the new coordinates for the rotated pixel
            int newX = y; // Reverse the order of y
            int newY = Height - 1 - x; // Use the original x as new y
            
            // Copy the pixel data to the rotated image
            newImage.pixels[(newY * Weight * 3 + newX * 3) + channel] = image.pixels[(x * 3 + y * Weight * 3) + channel];
        }
    }
}

    return newImage;
}


BMP rotateLeft(BMP image) {
    BMP newImage;

    int Height = image.header.height;
    int Weight = image.header.width;

    
    newImage.header = image.header;
    newImage.header.width = Height;
    newImage.header.height = Weight;

    int newSizeRow = calculateNewSize(newImage.header);
    int newPaddedSize = newSizeRow * newImage.header.height;

    newImage.header.fileSize = sizeof(BMPHeader) + newPaddedSize;
    newImage.header.dataSize = newPaddedSize;

    newImage.pixels = new unsigned char[newPaddedSize];

        for (int x = 0; x < Height; x++) {
            for (int y = 0; y < Weight; y++) {
                for (int channel = 0; channel < 3; channel++) {
                    newImage.pixels[(x * newSizeRow + (Weight - 1 - y) * 3) + channel] = image.pixels[(x * 3 + y * calculateNewSize(image.header)) + channel];
                }
            }
        }
    
    return newImage;
}



int main() {
    setlocale(LC_ALL, "Rus");
    try {
        BMP image = readbmp("Airplane.bmp");

        BMP rotatedImageRight = rotateRight(image);
        writebmp("rotatedRightAirplane.bmp", rotatedImageRight);
        cout << "Изображение повернуто на 90 градусов и сохранено как rotatedrightAirplane.bmp" << endl;

        BMP rotatedImageLeft = rotateLeft(image);
        writebmp("rotatedleftAirplane.bmp", rotatedImageLeft);
        cout << "Изображение повернуто на 90 градусов и сохранено как rotatedleftAirplane.bmp" << endl;

        

        delete[] rotatedImageRight.pixels;
        delete[] rotatedImageLeft.pixels;

    } 
    
    
    
    catch (exception const& e) {
        cerr << "Ошибка: " << e.what() << endl;
        return 1;
    }

    return 0;
}
