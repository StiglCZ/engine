#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/stat.h>
#include <fstream>
#include <sys/types.h>

#include "file.hh"
#include "math.hh"
#include "types.hh"

u64 uIDc = random(); // ID counter

const u32 SIGNATURE = 0xAABB;

[[deprecated("Use std::find_first_of or substr")]]
inline bool begins(std::string str, std::string bgn) {
    if(bgn.size() > str.size())return false;
    for(u32 i =0; i < bgn.size();i++)
        if(str[i] != bgn[i])return false;
    return true;
}

// Uselless, for now
std::vector<std::string> split(std::string str, char c) {
    std::string tmp;
    std::vector<std::string> result;
    for(u32 i =0; i < str.size();i++){
        if(str[i] == c){
            result.push_back(tmp);
        tmp = "";
        }else tmp += str[i];
    }
    result.push_back(tmp);
    return result;
}

std::vector<std::string> read_filelines(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string line;
    if(!file.is_open())return lines;
    while (std::getline(file, line)) 
        lines.push_back(line);
    file.close();
    return lines;
}

#include <dirent.h>
// 0 = Nothing/Not found
// 1 = File
// 2 = Directory
u8 eDirectory(char* dir) {
    struct stat info;
    if(!stat(dir, &info))return 0;
    return S_ISDIR(info.st_mode) + 1;
}

std::vector<std::string> getFiles(std::string dir) {
    std::vector<std::string> strs;
    DIR* directory = opendir(dir.c_str());
    // Doesn't exist
    if (directory == nullptr)
        return strs;
    dirent* entry;
    while ((entry = readdir(directory)) != nullptr) {
        std::string fileName = entry->d_name;
        if (entry->d_type == 4)
            continue; // Skip if subdirectory
        strs.push_back(fileName);
    }
    closedir(directory);
    return strs;
}

/*
  Avoid comments in the obj file begining
  with letter folowed by space

  Auto-scaling to size (1,1,1) not implemented
 */
Model loadOBJ(std::vector<std::string> file) {
    Model m;
    for(u32 i =0; i < file.size();i++){
        if(file[i][0] == 'v' && file[i][1] == ' '){
            std::vector<std::string> parts0 = split(file[i], ' ');
            m.verticies.push_back((Vector3){
                    (fx)atof(parts0[1].c_str()),
                    (fx)atof(parts0[2].c_str()),
                    (fx)atof(parts0[3].c_str())
                });
        }
        if(file[i][0] == 'f' && file[i][1] == ' '){
            std::vector<std::string> parts0 = split(file[i], ' ');
            u16* n = new u16[parts0.size()-1];
            for(u32 i =1; i < parts0.size(); i++){
                std::string n1 = split(parts0[i], '/')[0];
                n[i-1] = (ushort)(atoi(n1.c_str()) -1);
            }
            m.faces.push_back(
                (Face){
                    .verts = n,
                    .count = (u32)(parts0.size() - 1),
                });
        }
    }
    return m;
}

fx biggest(Vector3 v) {
    fx big = v.X;
    if(v.Y > big) big = v.Y;
    if(v.Z > big) big = v.Z;
    return big;
}

void optimizeModel(std::string fileName) {
    std::vector<Vector3> vecs;
    std::vector<std::vector<u16>> faces;
    std::ifstream ifs(fileName);
    if(!ifs.is_open())return;
    std::string line;
    Vector3 min, max;
    while(std::getline(ifs, line)){
        std::vector<std::string> parts0 = split(line, ' ');
        if(line[1] != ' ')continue;
        if(line[0] == 'v'){
            Vector3 vec;
            vec.X = atof(parts0[1].c_str());
            vec.Y = atof(parts0[2].c_str());
            vec.Z = atof(parts0[3].c_str());
            vecs.push_back(vec);
            {
                if(vec.X < min.X) min.X = vec.X;
                if(vec.Y < min.Y) min.Y = vec.Y;
                if(vec.Z < min.Z) min.Z = vec.X;

                if(vec.X > max.X) max.X = vec.X;
                if(vec.Y > max.Y) max.Y = vec.Y;
                if(vec.Z > max.Z) max.Z = vec.X;
            }
        }else if(line[0] == 'f'){
            std::vector<u16> face;
            for(u32 i =1; i < parts0.size(); i++){
                std::string n1 = split(parts0[i], '/')[0];
                face.push_back((ushort)(atoi(n1.c_str()) -1));
            }
            faces.push_back(face);
        }
    }
    // Centers the object(o0, o0, o0 = 0, 0, 0) and
    // scales them to idealy size (1, 1, 1)
    fx r2 = biggest(max - min);
    for(u32 i = 0; i < vecs.size(); i++){
        vecs[i] -= min;
        vecs[i] /= r2;
    }
    ifs.close();
    std::ofstream ofs(fileName, std::ios_base::binary | std::ios_base::out);
    if(!ofs.is_open())return;
    ofs.write((i8*)&SIGNATURE, sizeof(u32));
    u32 size = vecs.size();
    ofs.write((i8*)&size, sizeof(u32));
    for(u32 i =0; i < size; i++){
        ofs.write((i8*)&vecs[i].X, sizeof(fx));
        ofs.write((i8*)&vecs[i].Y, sizeof(fx));
        ofs.write((i8*)&vecs[i].Z, sizeof(fx));
    }
    size = faces.size();
    ofs.write((i8*)&size, sizeof(u32));
    for(u32 i =0; i < size; i++){
        u8 size3 = faces[i].size();
        ofs.write((i8*)&size3, sizeof(u8));
        for(u32 j =0; j < faces[i].size();j++)
            ofs.write((i8*)&faces[i][j], sizeof(u16));
    }
    ofs.write((i8*)&uIDc, sizeof(u64));
    uIDc++;
    ofs.close();
}

Model readOptimizedModel(std::string fileName) {
    retry:
    std::ifstream ifs(fileName, std::ios_base::binary);
    u32 signature;
    ifs.read((i8*)&signature, sizeof(u32));
    if(signature != SIGNATURE){
        optimizeModel(fileName);
        // Proper use of goto!
        // Do or while cycle would make this in this case extreemly unreadable
        goto retry;
    }
    Model m = {};
    u32 size;
    u8 str_size;
    ifs.read((i8*)&size, sizeof(u32));
    for(u32 i =0; i < size; i++){
        fx X, Y, Z;
        ifs.read((i8*)&X, sizeof(fx));
        ifs.read((i8*)&Y, sizeof(fx));
        ifs.read((i8*)&Z, sizeof(fx));
        m.verticies.push_back({X, Y, Z});
    }
    
    ifs.read((i8*)&size, sizeof(u32));
    for(u32 i =0; i < size; i++){
        ifs.read((i8*)&str_size, sizeof(u8));
        u16* data = new u16[str_size];
        for(int j =0; j < str_size; j++)
            ifs.read((i8*)&data[j], sizeof(u16));
        Face f = {
            .verts = data,
            .count = str_size,
        };
        m.faces.push_back(f);
    }
    ifs.read((i8*)&m.uId, sizeof(u64));
    m.freed = 0;
    ifs.close();
    return m;
}

Model loadModel(std::string filename) {
    // Wrapper cuz the objload is also possible to add
    return readOptimizedModel(filename);
}

// Called from native.hh
void SaveScreenshot(u8* data, Point size, const char* filename) {
    std::ofstream ofs(filename, std::ios::binary);
    std::string str = "P6\n"
        + std::to_string(size.X) + " "
        + std::to_string(size.Y) + "\n255\n";
    ofs.write(str.c_str(), str.size());
    for(int i =0; i < size.X * size.Y * 3; i++)
        ofs.write((char*)data++, 1);
    ofs.close();
}
