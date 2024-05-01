#include <cstring>
#include <fstream>
#include <iostream>

#include <string>
#include <sys/types.h>
#include <vector>
#include <string>
#include <error.h>
ulong uIDc = random();
std::vector<std::string> split(std::string str, char c) {
    std::string tmp;
    std::vector<std::string> result;
    for(unsigned i =0; i < str.size();i++){
        if(str[i] == c){
            result.push_back(tmp);
        tmp = "";
        }else tmp += str[i];
    }result.push_back(tmp);
    return result;
}
std::vector<std::string> read_filelines(std::string filename) {
    std::ifstream file(filename);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) 
        lines.push_back(line);
    return lines;
}
struct Vector3 {
    float X, Y, Z;
};
void decompileObj(std::string fileName) {
    std::ifstream ifs(fileName, std::ios_base::binary);
    std::ofstream ofs(fileName + ".obj", std::ios_base::binary);
    uint signature;
    ifs.read((char*)&signature, sizeof(uint));
    if(signature != 0xAABB)error(1, 0, "Invalid signature");
    uint size;
    u_char size3;
    ifs.read((char*)&size, sizeof(uint));
    for(uint i =0; i < size; i++){
        float X, Y, Z;
        ifs.read((char*)&X, sizeof(float));
        ifs.read((char*)&Y, sizeof(float));
        ifs.read((char*)&Z, sizeof(float));
        std::string x = std::to_string(X);
        std::string y = std::to_string(Y);
        std::string z = std::to_string(Z);
        std::string a = "v " + x + ' ' + y + ' ' + z + '\n';
        ofs.write(a.c_str(), a.size());
    }
    ifs.read((char*)&size, sizeof(uint));
    for(uint i =0; i < size; i++){
        ifs.read((char*)&size3, sizeof(char));
        ushort data[size3];
        std::string a = "f ";
        for(int j =0; j < size3; j++){
            ifs.read((char*)&data[j], sizeof(ushort));
            a += std::to_string(data[j]) + "/0/0 ";
        }
        a += '\n';
        ofs.write(a.c_str(), a.size());
    }
    ofs.close();
}
float biggest(Vector3 v) {
    float big = v.X;
    if(v.Y > big) big = v.Y;
    if(v.Z > big) big = v.Z;
    return big;
}
void optimize2Obj(std::string fileName) {
    std::vector<Vector3> vecs;
    std::vector<std::vector<ushort>> faces;
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
            std::vector<ushort> face;
            for(uint i =1; i < parts0.size(); i++){
                std::string n1 = split(parts0[i], '/')[0];
                face.push_back((ushort)(atoi(n1.c_str()) -1));
            }
            faces.push_back(face);
        }
    }
    ifs.close();
    // Centers the object(o0, o0, o0 = 0, 0, 0) and
    // scales them to idealy size (1, 1, 1)
    float r2 = biggest({
            max.X - min.X,
            max.Y - min.Y,
            max.Z - min.Z,
        });
    for(uint i = 0; i < vecs.size(); i++){
        vecs[i] ={
            (vecs[i].X - min.X) / r2,
            (vecs[i].Y - min.Y) / r2,
            (vecs[i].Z - min.Z) / r2
        };
    }
    std::ofstream ofs(fileName, std::ios_base::binary | std::ios_base::out);
    uint sign = 0xAABB;
    ofs.write((char*)&sign, sizeof(uint));
    uint size = vecs.size();
    ofs.write((char*)&size, sizeof(uint));
    for(uint i =0; i < size; i++){
        ofs.write((char*)&vecs[i].X, sizeof(float));
        ofs.write((char*)&vecs[i].Y, sizeof(float));
        ofs.write((char*)&vecs[i].Z, sizeof(float));
    }
    uint size2 = faces.size();
    ofs.write((char*)&size2, sizeof(uint));
    for(uint i =0; i < size2; i++){
        u_char size3 = faces[i].size();
        ofs.write((char*)&size3, sizeof(char));
        for(uint j =0; j < faces[i].size();j++)
            ofs.write((char*)&faces[i][j], sizeof(short));
    }
    ofs.write((char*)&uIDc, sizeof(long));
    uIDc += 1000;
    ofs.close();
}
int main(int argc, char** argv) {
    if(argc <= 1)error(1, 0, "Too few arguments(0)");
    if(!strcmp(argv[1], "-h") || !strcmp(argv[1], "help")){
        std::cout << "Help: \n"
                  << "<file>.obj - Optimizes the file\n"
                  << "-d <file>  - de-optimizes the file, but looses some information\n"
                  << "-u <uid>   - While optimizing file, provide an UID(second arg)";
        return 0;
    }
    if(!strcmp(argv[1], "-d")){
        std::string fileName;
        for(int i =1; i < argc; i++){
            std::ifstream ifs2(argv[i]);
            if(ifs2.is_open()){
                fileName = argv[i];
                break;
            }
            ifs2.close();
        }
        std::ifstream ifs(fileName);
        if(!ifs.is_open())
            error(1, 0, "Invalid file\n");
        decompileObj(fileName);
        return 0;
    }
    std::string fileName;
    for(int i =1; i < argc; i++){
        std::ifstream ifs2(argv[i]);
        if(ifs2.is_open()){
            fileName = std::string(argv[i]);
            if(!strcmp(argv[i], "-u")){
                uIDc = atol(argv[i+1]);
                break;
            }
        }
        ifs2.close();
    }
    std::ifstream ifs(fileName);
    if(!ifs.is_open())error(1, 0, "Invalid file/args (-h for help)\n");
    uint signature;
    ifs.read((char*)&signature, sizeof(uint));
    if(signature != 0xAABB)optimize2Obj(fileName);
    return 0;
}
