#include <fstream>
#include <iostream>
#include <error.h>
#include <sys/types.h>
#define READ_STR {                              \
        ifs.read(&size8, 1);                    \
        char str[size8];                        \
        ifs.read(str, size8);                   \
        std::cout << "\t-" << str << "\n";      \
    }
struct v3 {
    float x, y ,z;
};
struct obj {
    v3 pos, rot, sca, coll, col;
    unsigned long long model, flags;
};
#define readVector3(ifs, v)  \
    ifs.read((char *)&v.x, 4); \
    ifs.read((char *)&v.y, 4); \
    ifs.read((char *)&v.z, 4);
#define readObj(obj)           \
    readVector3(ifs, obj.pos)  \
    readVector3(ifs, obj.rot)  \
    readVector3(ifs, obj.sca)  \
    readVector3(ifs, obj.coll) \
    readVector3(ifs, obj.col)  \
    ifs.read((char *)&obj.model, 8);   \
    ifs.read((char *)&obj.flags, 8);

#define printV3(v, text)                                                       \
    std::cout << text << "[" << v.x << ", " << v.y << ", " << v.z << "]\n";

#define printObj(obj)                                                          \
  printV3(obj.pos, "Position: ");                                              \
  printV3(obj.rot, "Rotation: ");                                              \
  printV3(obj.sca, "Scale   : ");                                              \
  printV3(obj.coll,"Collbox : ");                                              \
  printV3(obj.col, "Color   : ");                                               \
  std::cout << "Model: " << obj.model << "\nFlags: " << obj.flags << "\n";

int main(int argc, char **argv) {
    if(argc == 1)error(1, 0, "Too few args. Provide scene file!");
    std::ifstream ifs(argv[1]);
    if(!ifs.is_open())error(1, 0, "Provide scene file!");
    char size8;
    ushort size16;
    ifs.read((char*)&size16, 2);
    
    std::cout << "Found " << (int)size16 << " models\n";
    std::cout << "Models:\n";
    for(int i =0; i < size16; i++) READ_STR;
    ifs.read((char*)&size16, 2);
    
    std::cout << "Found " << (int)size16 << " scripts\n";
    std::cout << "Scripts:\n";
    for(int i =0; i < size16; i++) READ_STR;
    ifs.read((char*)&size16, 2);
    
    std::cout << "Found " << (int)size16 << " gameobjects\n";
    obj j;
    readObj(j);
    printObj(j);
}
