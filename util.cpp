/* File: ${filename}.c
 * Creation Date: ${date} ${time}
 * Last Modified Date: ${date} ${time}
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/
#include <windows.h>
#include <iostream>

std::string *envvar(char *name)
{
    char value[MAX_PATH];
    GetEnvironmentVariable(name, value, MAX_PATH);
    return new std::string(value);
}

bool setenvvar(char *name, char *value)
{
    return SetEnvironmentVariable(name, value);
}
