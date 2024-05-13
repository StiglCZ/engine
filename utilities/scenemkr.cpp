#include <fstream>
#include <ios>
#include <iostream>
#include <error.h>
#include <sys/types.h>
#include <string>
#include <vector>
typedef unsigned int u;
std::vector<std::string> split(std::string str, char c) {
    std::string tmp;
    std::vector<std::string> result;
    for(u i =0; i < str.size();i++){
        if(str[i] == c){
            result.push_back(tmp);
        tmp = "";
        }else tmp += str[i];
    }result.push_back(tmp);
    return result;
}
void print_help() {
    std::cout
        << "-h        - Prints out this message\n"
        << "-m <file> - Makes subfile for model \t\t(models.bin)\n"
        << "-s <file> - Makes subfile for scripts \t\t(scripts.bin)\n"
        << "-o <file> - Makes subfile for gameobject \t(objects.bin)\n"
        << "-c ...    - Makes scene file from subfiles \t(scene.bin)\n"
        << "... = unload, model, script and object files\nseparated by spaces\n";
}
int main(int argc, char **argv) {
    if(argc < 2)error(1, 0, "Too few arguments(-h for help)");
    int var = 0;
    switch(argv[1][1]){
        default:
            error(1, 0, "Valid file argument not provided");
            break;
        case 'h':
            print_help();
            return 0;
            break;
        case 's':
            var = 1;
        case 'm':{ // Warning: The path is not dynamic and max 255chars
            if(argc < 3)error(1, 0, "File argument not provided");
            std::string line; int counter = 0;
            std::ifstream ifs(argv[2]);
            std::string fileName = "models.bin";
            if(var) fileName = "scripts.bin";
            std::ofstream ofs(fileName, std::ios_base::binary);
            if(!ifs.is_open() || !ofs.is_open())error(1, 1, "I/O error");
            while(std::getline(ifs, line)){
                u_char c = line.size();
                ofs.write((char*)&c, 1);
                ofs.write(line.c_str(), c);
                counter++;
            }
            ofs.write((char*)&counter, 4);
            ofs.close();
            }break;
        case 'o':{
            if(argc < 3)error(1, 0, "File argument not provided");
            std::ifstream ifs(argv[2]);ushort counter = 0;
            std::ofstream ofs("objects.bin", std::ios_base::binary);
            if(!ifs.is_open() || !ofs.is_open())error(1, 1, "I/O error");
            std::string line;
            while(std::getline(ifs, line)){
                std::vector<std::string> vec =
                    split(line, ' ');
                if(vec.size() < 17)continue;
                for(int i =0; i < 15; i++){
                    float a = atof(vec[i].c_str());
                    ofs.write((char*)&a, 4);
                }
                long b = atol(vec[15].c_str());
                long c = atol(vec[16].c_str());
                ofs.write((char*)&b, 8);
                ofs.write((char*)&c, 8);
                counter++;
            }
            ofs.write((char*)&counter, 2);
            ofs.close();}
            break;
        case 'c': // Compiles all smaller parts together
            if(argc < 6)error(1, 0, "File argument not provided");
            std::ifstream ifs1(argv[2]);
            std::ifstream ifs2(argv[3]);
            std::ifstream ifs3(argv[4]);
            std::ofstream ofs("scene.bin");
            if(!ifs1.is_open() || !ifs2.is_open() ||
               !ifs3.is_open() || !ofs.is_open())
                error(1, 1, "I/O error");
            
            // Models
            ushort size;
            ifs1.seekg(2, std::ios_base::end);
            ifs1.read((char*)&size, 2);
            ofs.write((char*)&size, 2);

            int fSize = (int)ifs1.tellg() -2;
            ifs1.seekg(0, std::ios_base::beg);
            char buff[fSize];
            ifs1.read(buff, fSize);
            ofs.write(buff, fSize);
            ifs1.close();

            // Scripts
            size = 0;
            ifs2.seekg(2, std::ios_base::end);
            ifs2.read((char*)&size, 2);
            ofs.write((char*)&size, 2);

            fSize = (int)ifs2.tellg() -2;
            ifs2.seekg(0, std::ios_base::beg);
            char buff2[fSize];
            ifs2.read(buff2, fSize);
            ofs.write(buff2, fSize);
            ifs2.close();

            // GameObjects
            size = 0;
            ifs3.seekg(2, std::ios_base::end);
            ifs3.read((char*)&size, 2);
            ofs.write((char*)&size, 2);

            fSize = (int)ifs3.tellg() -2;
            ifs3.seekg(0, std::ios_base::beg);
            char buff3[fSize];
            ifs3.read(buff3, fSize);
            ofs.write(buff3, fSize);
            ifs3.close();
            
            ofs.close();
            break;
    }
}
/*
int main1(int argc, char **argv) {
    if(argc < 2)error(1, 1, "Too few arguments(-h for help)");
    u_char unload = 0, lvl = 0;
    ushort s = 0xFFFF;
    std::string modelFile, scriptFile, gameObjectFile;
    for(int i =1; i < argc; i++){
        if(!strcmp("-h", argv[i])){
            std::cout
                << "Help:\n"
                << "-h   - prints out this dialog\n"
                << "-c <1/0(unload)> <modelfile> <scriptfile> <objfile> - Creates the scene file";
            return 0;
        }
        switch(lvl){
            case 0:
                unload = !strcmp("1", argv[i]);
                break;
            case 1:
                modelFile = std::string(argv[i]);
                break;
            case 2:
                scriptFile = std::string(argv[i]);
                break;
            case 3:
                gameObjectFile = std::string(argv[i]);
                break;
        }
        lvl++;
    }
    std::ofstream ofs("scene.bin", std::ios_base::binary);
    ofs.write((char*)&unload, 1);
    ofs.write((char*)&s, 2);
    
    std::ifstream ifs0(modelFile);
    if(ifs0.is_open())error(1, 2, "Cannot open file! Check the args..");
    ushort size1 = 0;
    u_char size0 = 0;
    std::string line;
    while(std::getline(ifs0, line)){
        size0 = line.size();
        ofs.write((char*)&size0, 1);
        ofs.write(line.c_str(), size0);
        size1++;
    }
    size0 = 0;
    ifs0.close();
    
    std::ifstream ifs1(scriptFile);
    if(ifs1.is_open())error(1, 2, "Cannot open file! Check the args..");
    ushort size3 = 0;
    ushort pos1 = ofs.tellp();
    while(std::getline(ifs1, line)){
        size0 = line.size();
        ofs.write((char*)&size0, 1);
        ofs.write(line.c_str(), size0);
        size3++;
    }
    ifs0.close();

    std::ifstream ifs2(scriptFile);
    if(ifs1.is_open())error(1, 2, "Cannot open file! Check the args..");
    ushort size4 = 0;
    ushort pos2 = ofs.tellp();
    while(std::getline(ifs1, line)){
        size0 = line.size();
        ofs.write((char*)&size0, 1);
        ofs.write(line.c_str(), size0);
        size4++;
    }
    ifs0.close();
    return 0;
}
*/
