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
    if(str == "PPM3" || str == "P3")
        img.gen = 3;
    else if(str == "PPM6" || str == "P6")
        img.gen = 6;

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
    if(img.gen == 3)ReadPPM3(img, ifs);
    else if(img.gen == 6)ReadPPM6(img, ifs);
    else error(1, 0, "PPM generation not supported! %d\n", img.gen);
    image_create(img);
    delete[] img.ptr;
}
