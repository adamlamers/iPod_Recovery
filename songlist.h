/* File: songlist.h
 * Creation Date: February 21st, 2010
 * Last Modified Date: February 21st, 2010
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#ifndef SONGLIST_H_INCLUDED
#define SONGLIST_H_INCLUDED

#undef _WIN32_WINNT
#define _WIN32_WINNT 0x500
#define _WIN32_IE 0x600
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "listview.h"
#include "resource2.h"

class CSongList
{
    HWND handle;
    HWND parent;
    HMENU ContextMenu;
    
    public:
        void(*OnAddItem)();
    
        CSongList(HWND Parent);
        void Clear();
        bool AddRow(char *name, char *artist, char *album, char *genre);
        void Scale();
        void CheckSelectedItems(bool check);
        void ShowContextMenu();
        HWND GetHandle();
       ~CSongList();
};

#endif // SONGLIST_H_INCLUDED
