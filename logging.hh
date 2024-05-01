#ifndef LOGGING
#define LOGGING
#define LOGLEVEL 0

#include <string>
#define CSSTR const std::string str
#define CCSTR const char *str
extern void Debg(CCSTR);
extern void Info(CCSTR);
extern void Warn(CCSTR);
extern void Spec(CCSTR);
extern void Err (CCSTR, int code);
extern void Log (CCSTR, int level);

extern void Debg(CSSTR);
extern void Info(CSSTR);
extern void Warn(CSSTR);
extern void Spec(CSSTR);
extern void Err (CSSTR, int code);
enum LogLevel {
    LDebg = 0,
    LInfo = 1,
    LWarn = 2,
    LError= 3,
    LSpec = 4,
};
#endif
