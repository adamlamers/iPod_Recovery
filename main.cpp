/* File: Dialogs.cpp
 * Creation Date: 12/23/2009 3:11PM
 * Last Modified Date: 12/23/2009 5:17PM
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#define _WIN32_WINNT 0x500
#define _WIN32_IE 0x600
#include <windows.h>
#include "Dialogs.h"
#include "resource.h"
#include "itdb.h"
#include "iphone.h"
#include "util.h"
#include <iostream>

#define IDM_CONTEXTSAVESONG 789

HINSTANCE hInst;

HWND SongList;
HWND SearchBox;
HWND SongCountLabel;

HWND musicRadioButton;
HWND videoRadioButton;

HMENU SongListContextMenu;

int DisplayType = 0;


BOOL ListViewAddColumn(HWND listview, int colIndex, char *headerText, int colWidth)
{
    LVCOLUMN lvc;
    memset(&lvc, 0, sizeof(lvc));
    lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvc.iSubItem = colIndex;
    lvc.pszText = headerText;
    lvc.cx = colWidth;
    if(ListView_InsertColumn(listview, colIndex, &lvc) == -1) return FALSE;
    return TRUE;
}

BOOL InitSongList(HWND hwndDlg)
{
    SongList = GetDlgItem(hwndDlg, IDC_SONGLIST);
    SendMessage(SongList, LVM_SETEXTENDEDLISTVIEWSTYLE, 0, LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    ListViewAddColumn(SongList, 0, "", 23);
    ListViewAddColumn(SongList, 1, "Name", 234);
    ListViewAddColumn(SongList, 2, "Artist", 234);
    ListViewAddColumn(SongList, 3, "Album", 234);
    ListViewAddColumn(SongList, 4, "Genre", 218);
    
    SongListContextMenu = CreatePopupMenu();
    InsertMenu(SongListContextMenu, 0, MF_BYPOSITION | MF_STRING, IDM_CONTEXTSAVESONG, "Save Song...");
    
    return TRUE;
}

void ClearSongList(HWND listview)
{
     SendMessage(listview, LVM_DELETEALLITEMS, 0, 0L);
}

BOOL SongListAddRow(HWND listview, char *name, char *artist, char *album, char *genre)
{
    LVITEM item;
    memset(&item, 0, sizeof(LVITEM));
    item.mask = LVIF_TEXT;
    item.iItem = 0;
    item.iSubItem = 0;
    item.pszText = "";
    ListView_InsertItem(listview, &item);
    
    item.iSubItem = 1;
    item.pszText = name;
    ListView_SetItem(listview, &item);
    
    item.iSubItem = 2;
    item.pszText = artist;
    ListView_SetItem(listview, &item);
    
    item.iSubItem = 3;
    item.pszText = album;
    ListView_SetItem(listview, &item);
    
    item.iSubItem = 4;
    item.pszText = genre;
    ListView_SetItem(listview, &item);
    
    return TRUE;
}

BOOL ScaleSongList(HWND hwndDlg)
{
    RECT windowRect;
    GetWindowRect(hwndDlg, &windowRect);
    int width = (windowRect.right - windowRect.left);
    int height = (windowRect.bottom - windowRect.top);
    return SetWindowPos(SongList,
                        HWND_TOP,
                        0,
                        25,
                        width - 16,
                        height - 151,
                        SWP_NOZORDER);
}

BOOL InitSearchBox(HWND hwndDlg)
{
    SearchBox = GetDlgItem(hwndDlg, IDC_SEARCH);
    SendMessage(SearchBox, WM_SETTEXT, (WPARAM)0, (LPARAM)"Search...");
    return true;
}

BOOL SetMenuItemBitmap(HINSTANCE hInstance, HMENU menu, int subitemPosy, int subitemPosx, int resourceID)
{
    HBITMAP bitmap = LoadBitmap(hInstance, MAKEINTRESOURCE(resourceID));
    return SetMenuItemBitmaps(GetSubMenu(menu, subitemPosy), subitemPosx, MF_BYPOSITION, bitmap, bitmap);
}

BOOL SetWindowIcon(HWND hwnd, HICON icon)
{
    return (SetClassLong(hwnd, GCL_HICON, (LONG)icon) != 0);
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            
            HICON AppIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));
            SetWindowIcon(hwndDlg, AppIcon);
            InitSongList(hwndDlg);
            ScaleSongList(hwndDlg);
            SongCountLabel = GetDlgItem(hwndDlg, IDC_SONGCOUNTLABEL);
            InitSearchBox(hwndDlg);
            InitiPhone(hwndDlg);
            return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;
        
        case WM_SIZE:
            ScaleSongList(hwndDlg);
            return TRUE;

        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDC_SEARCH:
                    if(HIWORD(wParam) == EN_SETFOCUS)
                    {
                        char text[1024];
                        SendMessage(SearchBox, WM_GETTEXT, (WPARAM)1024, (LPARAM)text);
                        if(strcmp(text, "Search...") == 0)
                        {
                            SendMessage(SearchBox, WM_SETTEXT, 0, (LPARAM)"");
                        }
                    }
                    if(HIWORD(wParam) == EN_KILLFOCUS)
                    {
                       char text[1024];
                        SendMessage(SearchBox, WM_GETTEXT, (WPARAM)1024, (LPARAM)text);
                        if(strcmp(text, "") == 0)
                        {
                            SendMessage(SearchBox, WM_SETTEXT, 0, (LPARAM)"Search...");
                        } 
                    }
                return TRUE;
                
                case IDC_RADIO1: // Music
                    if(HIWORD(wParam) == BN_CLICKED)
                    {
                        CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);
                        DisplayType = ITDB_MEDIATYPE_AUDIO | ITDB_MEDIATYPE_AUDIOBOOK;
                    }
                return TRUE;
                
                case IDC_RADIO2: // Videos
                    if(HIWORD(wParam) == BN_CLICKED)
                    {
                        CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO2);
                        DisplayType = ITDB_MEDIATYPE_MOVIE | ITDB_MEDIATYPE_TVSHOW | ITDB_MEDIATYPE_MUSICVIDEO;
                    }
                return TRUE;
                
                case IDC_RADIO3: // Podcasts
                    if(HIWORD(wParam) == BN_CLICKED)
                    {
                        CheckRadioButton(hwndDlg, IDC_RADIO1, IDC_RADIO3, IDC_RADIO3);
                        DisplayType = ITDB_MEDIATYPE_PODCAST;
                    }
                return TRUE;
                
                case IDM_CONTEXTSAVESONG:
                    MessageBox(NULL, "Context Save Song", "", MB_OK);
                return TRUE;
                
                case IDM__EXIT1:
                    SendMessage(hwndDlg, WM_CLOSE, 0, 0);
                return TRUE;
            }
        case WM_NOTIFY:
            switch(wParam)
            {
                case IDC_SONGLIST:
                    switch(((LPNMHDR)lParam)->code)
                    {
                        case NM_RCLICK:
                            if(((LPNMHDR)lParam)->idFrom == IDC_SONGLIST)
                            {
                                POINT mouse;
                                GetCursorPos(&mouse);
                                TrackPopupMenu(SongListContextMenu, TPM_TOPALIGN | TPM_LEFTALIGN, mouse.x, mouse.y, 0, hwndDlg, (RECT*)NULL);
                            }
                        break;
                    }
                break;
                        
            }
            return TRUE;
    }
    return FALSE;
}


int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    hInst = hInstance;
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc);
}
