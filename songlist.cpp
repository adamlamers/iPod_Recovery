/* File: songlist.cpp
 * Creation Date: February 21st, 2010
 * Last Modified Date: February 21st, 2010
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/
#include "songlist.h"

CSongList::CSongList(HWND Parent) : OnAddItem(NULL)
{
    parent = Parent;
    handle = GetDlgItem(parent, IDC_SONGLIST);
    SendMessage(handle, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    ListViewAddColumn(handle, 0, "", 23);
    ListViewAddColumn(handle, 1, "Name", 234);
    ListViewAddColumn(handle, 2, "Artist", 234);
    ListViewAddColumn(handle, 3, "Album", 234);
    ListViewAddColumn(handle, 4, "Genre", 218);
    
    ContextMenu = CreatePopupMenu();
    InsertMenu(ContextMenu, 0, MF_BYPOSITION | MF_STRING, IDM_CONTEXTSAVESONG, "Save Song(s)...");
    InsertMenu(ContextMenu, 1, MF_BYPOSITION | MF_STRING, IDM_CONTEXTCHECKSELECTED, "Check Selected Items");
    InsertMenu(ContextMenu, 2, MF_BYPOSITION | MF_STRING, IDM_CONTEXTUNCHECKSELECTED, "Uncheck Selected Items");
    
    Scale();
}

void CSongList::Clear()
{
    SendMessage(handle, LVM_DELETEALLITEMS, 0, 0L);
}

bool CSongList::AddRow(char *name, char *artist, char *album, char *genre)
{
    LVITEM item;
    memset(&item, 0, sizeof(LVITEM));
    item.mask = LVIF_TEXT;
    item.iItem = 0;
    item.iSubItem = 0;
    item.pszText = "";
    if(ListView_InsertItem(handle, &item) == -1) return false;
    
    item.iSubItem = 1;
    item.pszText = name;
    if(ListView_SetItem(handle, &item) == -1) return false;
    
    item.iSubItem = 2;
    item.pszText = artist;
    if(ListView_SetItem(handle, &item) == -1) return false;
    
    item.iSubItem = 3;
    item.pszText = album;
    if(ListView_SetItem(handle, &item) == -1) return false;
    
    item.iSubItem = 4;
    item.pszText = genre;
    if(ListView_SetItem(handle, &item) == -1) return false;
    
    if(OnAddItem)OnAddItem();
    return true;
}

void CSongList::Scale()
{
    RECT windowRect;
    GetWindowRect(parent, &windowRect);
    int width = (windowRect.right - windowRect.left);
    int height = (windowRect.bottom - windowRect.top);
    SetWindowPos(handle,
                        HWND_TOP,
                        0,
                        25,
                        width - 16,
                        height - 103,
                        SWP_NOZORDER);
}

void CSongList::CheckSelectedItems(bool check)
{
    int ItemCount = ListView_GetItemCount(handle);
    int i;
    for(i = 0; i < ItemCount; i++)
    {
        LVITEM item;
        item.iItem = i;
        item.mask = LVIF_STATE;
        item.stateMask = LVIS_SELECTED;
        if(ListView_GetItem(handle, &item))
        {
            if(item.state & LVIS_SELECTED)
            {
                ListView_SetCheckState(handle, item.iItem, check);
            }
        }
    }
}

void CSongList::ShowContextMenu()
{
    POINT mouse;
    GetCursorPos(&mouse);
    
    if(ListView_GetSelectedCount(handle) > 1)
    {
        EnableMenuItem(ContextMenu, 1, MF_BYPOSITION | MF_ENABLED);
        EnableMenuItem(ContextMenu, 2, MF_BYPOSITION | MF_ENABLED);
    }
    else
    {
        EnableMenuItem(ContextMenu, 1, MF_BYPOSITION | MF_GRAYED);
        EnableMenuItem(ContextMenu, 2, MF_BYPOSITION | MF_GRAYED);
    }
    
    TrackPopupMenu(ContextMenu, TPM_TOPALIGN | TPM_LEFTALIGN, mouse.x, mouse.y, 0, parent, (RECT*)NULL);
}

HWND CSongList::GetHandle()
{
    return handle;
}

CSongList::~CSongList(){}
