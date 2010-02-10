#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <iostream>

extern std::string envvar(char *name);
extern bool setenvvar(char *name, char *value);


#endif // UTIL_H_INCLUDED
