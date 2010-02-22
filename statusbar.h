/* File: statusbar.h
 * Creation Date: February 22nd, 2010
 * Last Modified Date: Februrary 22nd, 2010
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#ifndef STATUSBAR_H_INCLUDED
#define STATUSBAR_H_INCLUDED

#include <windows.h>
#include <commctrl.h>
#include "resource.h"

class CStatusBar
{
    HWND handle;
    HWND parent;
    
    public:
        CStatusBar(HWND parent);
        void Scale();
        bool SetSegmentText(int index, const char *text);
       ~CStatusBar();
};

#endif // STATUSBAR_H_INCLUDED
