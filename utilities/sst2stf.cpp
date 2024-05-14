#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstring>
#include <error.h>
#include <sys/types.h>

void help() {
    std::cout
        << "-h(--help)   - Print this dialog\n"
        << "<file> <dir> - Converts the SST to normal scene file\n<dir> will be removed from each filename if its there";
    exit(0);
}
int findormake(std::vector<std::string>& models, std::string line, std::string dir) {
    unsigned i =0;
    while(dir[i] == line[i]) i++;
    line = line.substr(i);
    
    i = 0;
    while(i < models.size()){
        if(models[i] == line)
            return i;
        i++;
    }
    models.push_back(line);
    return models.size()-1;
}
struct v3 {
    float x, y, z;
};
struct i3 {
    int x, y, z;
};
struct obj {
    unsigned long long model;
    v3 pos, rot, sca;
    i3 col;
};
#define sized(equalto)                   \
    size = equalto;                      \
    ofs.write((char*)&size, sizeof(short));
#define writeV3(ofs, vec)                       \
    ofs.write((char *)&(vec.x), sizeof(float)); \
    ofs.write((char *)&(vec.y), sizeof(float)); \
    ofs.write((char *)&(vec.z), sizeof(float));

#define get(par)                                \
    std::getline(ifs, line);                    \
    ss = std::stringstream(line);               \
    ss >> o.par.x >> o.par.y >> o.par.z;
void convert(std::ifstream& ifs, std::string dir) {
    std::vector<std::string> models;
    std::vector<obj> objs;
    std::string line;
    while(std::getline(ifs, line)){
        obj o;
        o.model = findormake(models, line, dir);
        std::stringstream ss;
        get(pos);
        get(rot);
        get(sca);
        get(col);
        objs.push_back(o);
    }
    ifs.close();
    
    ushort size;
    std::ofstream ofs("scene.bin", std::ios::binary);
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
    for(unsigned i =0; i < objs.size(); i++){
        writeV3(ofs, objs[i].pos);
        writeV3(ofs, objs[i].rot);
        writeV3(ofs, objs[i].sca);
        writeV3(ofs, objs[i].sca); // Collision box
        writeV3(ofs, objs[i].col);
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
    else if(ifs.is_open()) convert(ifs, argv[2]);
    else error(1, 0, "Invalid arguments! (Use -h for help)\n");
    return 0;
}
