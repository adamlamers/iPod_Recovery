/* File: util.cpp
 * Creation Date: January 26th, 2010
 * Last Modified Date: Februrary 22nd, 2010
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#define _WIN32_WINNT 0x500
#define _WIN32_IE 0x600
#include <windows.h>
#include <iostream>
#include <commctrl.h>

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

bool SetMenuItemBitmap(HINSTANCE hInstance, HMENU menu, int subitemPosy, int subitemPosx, int resourceID)
{
    HBITMAP bitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(resourceID));
    return SetMenuItemBitmaps(GetSubMenu(menu, subitemPosy), subitemPosx, MF_BYPOSITION, bitmap, bitmap);
}

bool SetWindowIcon(HWND hwnd, HICON icon)
{
    return (SetClassLong(hwnd, GCL_HICON, (LONG)icon) != 0);
}

bool InitCommonCtrls()
{
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    return InitCommonControlsEx(&InitCtrls);
}
    
