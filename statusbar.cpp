/* File: statusbar.cpp
 * Creation Date: February 22nd, 2010
 * Last Modified Date: Februrary 22nd, 2010
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#include "statusbar.h"

CStatusBar::CStatusBar(HWND Parent)
{
    parent = Parent;
    int parts[] = {100, 220, -1};
    handle = CreateWindowEx(0,
                            STATUSCLASSNAME,
                            NULL,
                            WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
                            0, 0, 0, 0, parent,
                            (HMENU)IDC_STATUSBAR,
                            GetModuleHandle(NULL),
                            NULL);
    SendMessage(handle, SB_SETPARTS, sizeof(parts) / sizeof(int), (LPARAM)parts);
    SendMessage(handle, SB_SETTEXT, (WPARAM)0, (LPARAM)"Disconnected");
    SendMessage(handle, SB_SETTEXT, (WPARAM)1, (LPARAM)"");
}

void CStatusBar::Scale()
{
    SendMessage(handle, WM_SIZE, 0, 0);
}

bool CStatusBar::SetSegmentText(int index, const char *text)
{
    return SendMessage(handle, SB_SETTEXT, (WPARAM)index, (LPARAM)text);
}

CStatusBar::~CStatusBar(){}
