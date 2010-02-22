#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED

#include <iostream>
#include <windows.h>

extern std::string envvar(char *name);
extern bool setenvvar(char *name, char *value);
extern bool SetMenuItemBitmap(HINSTANCE hInstance, HMENU menu, int subitemPosy, int subitemPosx, int resourceID);
extern bool SetWindowIcon(HWND hwnd, HICON icon);
extern bool InitCommonCtrls();
#endif // UTIL_H_INCLUDED
