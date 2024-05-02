#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <error.h>
#include <sys/types.h>
#include <vector>

void help() {
    std::cout
        << "-h(--help) - Print this dialog\n"
        << "<file>     - Converts the SST to normal scene file\n";
    exit(0);
}
int findormake(std::vector<std::string>& models, std::string file) {
    unsigned i =0;
    while(i < models.size()){
        if(models[i] == file)
            return i;
        i++;
    }
    models.push_back(file);
    return models.size()-1;
}
struct v3 {
    float x, y, z;
};
struct obj {
    unsigned long long model;
    v3 pos, rot, sca;
};
#define sized(equalto)                   \
    size = equalto;                      \
    ofs.write((char*)&size, sizeof(short));
#define writeV3(ofs, vec)                       \
    ofs.write((char *)&(vec.x), sizeof(float)); \
    ofs.write((char *)&(vec.y), sizeof(float)); \
    ofs.write((char *)&(vec.z), sizeof(float));

void convert(std::ifstream& ifs) {
    std::vector<std::string> models;
    std::vector<obj> objs;
    //models.push_back("camera");
    std::string line;
    while(std::getline(ifs, line)){
        int model = findormake(models, line);
        std::getline(ifs, line);
        std::cout << line << "\n";
        std::stringstream ss(line);
        obj o;
        o.model = model;
        ss >> o.pos.x >> o.pos.y >> o.pos.z
           >> o.rot.x >> o.rot.y >> o.rot.z
           >> o.sca.x >> o.sca.y >> o.sca.z;
        objs.push_back(o);
    }
    ifs.close();
    
    ushort size;
    std::ofstream ofs("scene.bin", std::ios::binary);
    char unload = 1;
    ofs.write(&unload, 1);
    // Models
    sized(models.size());
    for(unsigned i =0; i < models.size(); i++){
        u_char size = models[i].size();
        ofs.write((char*)&size, 1);
        ofs.write(models[i].c_str(), size);
    }
    // Scripts & GameObjects
    sized(0);
    sized(objs.size());

    unsigned long long flags = 0;
    v3 col = {255, 255, 255};
    for(unsigned i =0; i < objs.size(); i++){
        writeV3(ofs, objs[i].pos);
        writeV3(ofs, objs[i].rot);
        writeV3(ofs, objs[i].sca);
        writeV3(ofs, objs[i].sca);
        writeV3(ofs, col);
        ofs.write((char*)&objs[i].model, sizeof(long long));
        ofs.write((char*)&flags, sizeof(long long));
    }

    ofs.close();
}
int main(int argc, char **argv) {
    if(argc <= 1) error(1, 0,"Too few args! (Use -h for help)");
    std::ifstream ifs(argv[1]);
    if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        help();
    else if(ifs.is_open()) convert(ifs);
    else error(1, 0, "Invalid arguments! (Use -h for help)\n");
    return 0;
}
