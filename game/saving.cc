#include "../types.hh"
#include "saving.hh"
#include "../logging.hh"
#include <string>
#include <fstream>
#include <vector>

const u32 zero = 0;
std::string filename = "save.dat";
std::vector<SaveRequest (*)()> funcs;

void RequestSaveInfo(SaveRequest (*func)()) { funcs.push_back(func); }
void FileName(char *fileName) { filename = std::string(fileName); }

void Location(u32 node, std::ifstream* ifs) {
    if(node <= 0)return;
    u32 size;
    ifs->read((i8*)&size, sizeof(u32));
    ifs->seekg(size,std::ios::cur);
    Location(node-1, ifs);
}

LoadRequest Load(u32 node) {
    std::ifstream ifs(filename, std::ios::in | std::ios::binary);
    Location(node - 1, &ifs);
    u32 size;
    ifs.read((i8*)&size, sizeof(u32));
    void* ptr = new u8[size];
    ifs.read((i8*)ptr, size);
    LoadRequest lr;
    lr.data = ptr;
    lr.len = size;
    return lr;
}

void Save() {
    std::vector<SaveRequest> srqsts = {};
    if(funcs.empty()){
        Debg("Empty save file!");
        return;
    }
    // Effectively sorts the elements
    for(u16 i =0, j =0; i < funcs.size();i++){
        SaveRequest sr = funcs[i]();
        if(srqsts.empty()){
            srqsts.push_back(sr);
            continue;
        }
        for(j = 0; j < srqsts.size(); j++){
            if(srqsts[j].node > sr.node)
                break;
        }
        if(srqsts[j].node > sr.node)
            srqsts.insert(srqsts.begin() + j, sr);
        else srqsts.push_back(sr);
    }
    
    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    if(!ofs.is_open()){
        Warn("Save file isn't writable!");
        return;
    }
    // Calculates the amount of blocks between <last> and <current> node,
    // and fills them with zero-size blocks
    for(u16 i =0, last = 0; i < srqsts.size(); i++){
        int diff = srqsts[i].node - last -1;
        for(u16 j = 0; j < diff; j++)
            ofs.write((i8*)&zero, sizeof(u32));
        ofs.write((i8*)&srqsts[i].len, sizeof(u32));
        ofs.write((i8*)srqsts[i].data, srqsts[i].len);
        last = srqsts[i].node;
    }
    ofs.flush();
    ofs.close();
}
