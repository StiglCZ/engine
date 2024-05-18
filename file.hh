#ifndef FILE_HH
#define FILE_HH

#include "types.hh"
#include <string>

extern std::vector<std::string> split(std::string str, char c);
extern std::vector<std::string> readFileLines(std::string filename);
extern u8 eDirectory(const char *dir);
extern std::vector<std::string> getFiles(std::string dir);
extern Model loadModel(std::string filename);
extern void SaveScreenshot(u8* data, Point size, const char* filename);
#endif
