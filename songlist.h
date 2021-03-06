/* File: songlist.h
 * Creation Date: February 21st, 2010
 * Last Modified Date: February 22nd, 2010
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
#include <cstdio>
#include <vector>
#include <list>
#include <itdb.h>
#include "resource.h"
#include "listview.h"

class CSongList
{
    HWND handle;
    HWND parent;
    HMENU ContextMenu;
    std::list<Itdb_Track*> *tracks;
    bool colSortDirs[5];
    static int SortCallback(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);
    
    public:
        void(*OnAddItem)();
    
        CSongList(HWND Parent);
        void Clear();
        bool AddRow(char *name, char *artist, char *album, char *genre);
        void Scale();
        void CheckSelectedItems(bool check);
        void ShowContextMenu();
        void ContextSaveSong();
        
        std::vector<int> GetSelected();
        void SetTrackList(std::list<Itdb_Track*> *list);
        bool Sort(int columnIndex);
        HWND GetHandle();
       ~CSongList();
};

#endif // SONGLIST_H_INCLUDED
