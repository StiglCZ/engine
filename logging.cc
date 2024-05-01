#include <iostream>
#include "logging.hh"
const char *green = "\033[32m",   *red = "\033[31m",     *yellow = "\033[33m",
           *blue = "\033[34m",    *magenta = "\033[35m", *cyan = "\033[36m",
           *white = "\033[37m",   *reset = "\033[0m",    *bold = "\033[1m",
           *underline = "\033[4m",*blink = "\033[5m",    *reverse = "\033[7m",
           *hidden = "\033[8m",   *gray = "\033[90m";
const char
    *info = cyan,
    *warn = yellow,
    *err = red,
    *spec = blue,
    *debg = gray;
void Debg(const char *str) {
    (void)str;
    #if LOGLEVEL < 1
    std::cout << debg << str << reset << std::endl;
    #endif
}
void Info(const char *str) {
    (void)str;
    #if LOGLEVEL < 2
    std::cout << info << str << reset << std::endl;
    #endif
}
void Warn(const char *str) {
    (void)str;
    #if LOGLEVEL < 3
    std::cout << warn << str << reset << std::endl;
    #endif
}
void Err (const char *str, int code)  {
    std::cout << err  << str << reset << std::endl;
    exit(code);
}
void Spec(const char *str) {
    std::cout << spec  << str << reset << std::endl;
}

void Debg(const std::string str) {
    (void)str;
    #if LOGLEVEL < 1
    std::cout << debg << str << reset << std::endl;
    #endif
}
void Info(const std::string str) {
    (void)str;
    #if LOGLEVEL < 2
    std::cout << info << str << reset << std::endl;
    #endif
}
void Warn(const std::string str) {
    (void)str;
    #if LOGLEVEL < 3
    std::cout << warn << str << reset << std::endl;
    #endif
}
void Err (const std::string str, int code)  {
    std::cout << err  << str << reset << std::endl;
    exit(code);
}
void Spec(const std::string str) {
    std::cout << spec  << str << reset << std::endl;
}
void Log(const char *str, int level) {
    if(LOGLEVEL > level + 1)return;
    switch(level){
        case LDebg: Debg(str);
            break;
        case LInfo: Info(str);
            break;
        case LWarn: Warn(str);
            break;
        case LError: Err(str, -1);
            break;
        case LSpec: Spec(str);
            break;
    }
}
