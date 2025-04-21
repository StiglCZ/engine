#include <cstring>
#include <fstream>
#include <iostream>
#include <png.h>
#include <error.h>
#include <sstream>

typedef unsigned char byte;

struct Image {
    char gen;            // PPM generation
    int sizeX, sizeY;    // Dimensions
    byte* ptr;           // Pointer on the (rgb) image data
    unsigned size;       // Size of    the (rgb) image data
    short maxval;        // Uselless at this point(if not higher than 255)
};
void image_create(Image& img) {
    FILE *fp = fopen("output.png", "wb");
    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    setjmp(png_jmpbuf(png));
    png_init_io(png, fp);
    png_set_IHDR(
        png, info,
        img.sizeX, img.sizeY, 8,
        PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT
    );
    png_write_info(png, info);
    for (int y = 0; y < img.sizeY; y++) {
        int index = img.sizeX * 3 * y;
        png_bytep row = (png_bytep)&img.ptr[index];
        png_write_row(png, row);
    }  
    png_write_end(png, NULL);
    fclose(fp);
}

// First 3 lines
Image readHead(std::ifstream& file) {
    Image img;
    std::string str;
    std::getline(file, str);
    if(str.empty()) return img;
    if(str == "PPM1" || str == "P1") img.gen = 1;
    else if(str == "PPM2" || str == "P2") img.gen = 2;
    else if(str == "PPM3" || str == "P3") img.gen = 3;
    else if(str == "PPM3" || str == "P4") img.gen = 4;
    else if(str == "PPM3" || str == "P5") img.gen = 5;
    else if(str == "PPM6" || str == "P6") img.gen = 6;

    // Fetch 2 lines, detect the maxval and dimensions
    std::getline(file, str);
    std::stringstream ss(str);
    ss >> img.sizeX >> img.sizeY;
    std::getline(file, str);
    ss >> img.maxval;
    img.size = 3 * img.sizeX * img.sizeY;;
    img.ptr = new byte[img.size];
    return img;
}

// PPM1 - text bit format
void ReadPPM1(Image &img, std::ifstream& file) {
    byte* b = img.ptr;
    std::string str;
    for(int i =0; i < img.sizeX * img.sizeY; i++){
        std::getline(file, str);
        std::stringstream ss(str);
        int A, R = 0, G = 0, B = 0;
        ss >> A;
        if(A) R = G = B = 255;
        *b++ = R;
        *b++ = G;
        *b++ = B;
    }
}

// PPM2 - text gray format
void ReadPPM2(Image &img, std::ifstream& file) {
    byte* b = img.ptr;
    std::string str;
    for(int i =0; i < img.sizeX * img.sizeY; i++){
        std::getline(file, str);
        std::stringstream ss(str);
        int R = 0, G = 0, B = 0;
        ss >> B;
        R = G = B;
        *b++ = R;
        *b++ = G;
        *b++ = B;
    }
}

// PPM3 - text format
void ReadPPM3(Image &img, std::ifstream& file) {
    byte* b = img.ptr;
    std::string str;
    for(int i =0; i < img.sizeX * img.sizeY; i++){
        std::getline(file, str);
        std::stringstream ss(str);
        int R, G, B;
        ss >> R >> G >> B;
        *b++ = R;
        *b++ = G;
        *b++ = B;
    }
}

// PPM4 - binary gray format
void ReadPPM4(Image& img, std::ifstream& ifs) {
    byte* b = img.ptr;
    std::string str;
    for(int i =0; i < img.sizeX * img.sizeY / 8; i++){
        byte B;
        ifs.read((char*)&B, 1);
        for(int i =0; i <= 0x80; i *= 2)
            *b++ = *b++ = *b++ = B & i;
    }
}

// PPM5 - binary gray format
void ReadPPM5(Image& img, std::ifstream& ifs) {
    byte* b = img.ptr;
    std::string str;
    for(int i =0; i < img.sizeX * img.sizeY; i++){
        byte R, G, B;
        ifs.read((char*)&B, 1);
        R = G = B;
        *b++ = R;
        *b++ = G;
        *b++ = B;
    }
}

// PPM6 - binary format
void ReadPPM6(Image& img, std::ifstream& ifs) {
    byte* b = img.ptr;
    std::string str;
    for(int i =0; i < img.sizeX * img.sizeY; i++){
        byte R, G, B;
        ifs.read((char*)&R, 1);
        ifs.read((char*)&G, 1);
        ifs.read((char*)&B, 1);
        *b++ = R;
        *b++ = G;
        *b++ = B;
    }
}

int main(int argc, char** argv) {
    if(argc < 2)
        error(1, 0, "Too few args");
    if(!strcmp(argv[1], "-h")){
        std::cout << "./ppm2png <file> - Decodes PPM format to png\n";
        return 0;
    }
    std::ifstream ifs(argv[1]);
    if(!ifs.is_open())
        error(1, 0, "Cannot open file %s", argv[1]);
    Image img = readHead(ifs);
    switch(img.gen) {
        case 1:
            ReadPPM1(img, ifs);
            break;
        case 2:
            ReadPPM2(img, ifs);
            break;
        case 3:
            ReadPPM3(img, ifs);
            break;
        case 4:
            ReadPPM4(img, ifs);
            break;
        case 5:
            ReadPPM5(img, ifs);
            break;
        case 6:
            ReadPPM6(img, ifs);
            break;
        default:
            error(1, 0, "PPM generation not supported! %d\n", img.gen);
    }
    image_create(img);
    delete[] img.ptr;
}
