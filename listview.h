#ifndef LISTVIEW_H_INCLUDED
#define LISTVIEW_H_INCLUDED

#include <windows.h>

extern bool ListViewAddColumn(HWND listview, int colIndex, char *headerText, int colWidth);
#endif // LISTVIEW_H_INCLUDED
