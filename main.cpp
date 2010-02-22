/* File: Dialogs.cpp
 * Creation Date: 12/23/2009
 * Last Modified Date: 2/21/2010
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#define _WIN32_WINNT 0x500
#define _WIN32_IE 0x600
#include <windows.h>
#include <iostream>
#include <commctrl.h>
#include "Dialogs.h"
#include "resource.h"
#include "itdb.h"
#include "iphone.h"
#include "listview.h"
#include "util.h"
#include "resource2.h"
#include "songlist.h"

HINSTANCE hInst;

CSongList *SongList;
HWND SearchBox;
HWND StatusBar;

HWND musicRadioButton;
HWND videoRadioButton;

HMENU SongListContextMenu;

int DisplayType = 0;
int songCount = 0;

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

BOOL InitStatusBar(HWND hwndDlg)
{
    int parts[] = {100, 220, -1};
    StatusBar = CreateWindowEx(0, STATUSCLASSNAME, NULL, WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP, 0, 0, 0, 0, hwndDlg, (HMENU)IDC_STATUSBAR, GetModuleHandle(NULL), NULL);
    SendMessage(StatusBar, SB_SETPARTS, sizeof(parts) / sizeof(int), (LPARAM)parts);
    SendMessage(StatusBar, SB_SETTEXT, (WPARAM)0, (LPARAM)"Disconnected");
    SendMessage(StatusBar, SB_SETTEXT, (WPARAM)1, (LPARAM)"");
    return TRUE;
}

void SongListItemAdded()
{
    char statusText[256];
    songCount++;
    sprintf(statusText, "%d Items", songCount);
    SendMessage(StatusBar, SB_SETTEXT, (WPARAM)1, (LPARAM)statusText);
}

BOOL CALLBACK DialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_INITDIALOG:
            
            HICON AppIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_APPICON));
            SetWindowIcon(hwndDlg, AppIcon);
            SongList = new CSongList(hwndDlg);
            SongList->OnAddItem = &SongListItemAdded;
            InitSearchBox(hwndDlg);
            InitiPhone(hwndDlg);
            InitStatusBar(hwndDlg);
            return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            return TRUE;
        
        case WM_SIZE:
            SongList->Scale();
            SendMessage(StatusBar, WM_SIZE, 0, 0);
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
                
                case IDM_CONTEXTCHECKSELECTED:
                    SongList->CheckSelectedItems(true);
                return TRUE;
                
                case IDM_CONTEXTUNCHECKSELECTED:
                    SongList->CheckSelectedItems(false);
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
                                SongList->ShowContextMenu();
                            }
                        break;
                        
                        case LVN_COLUMNCLICK:
                        {
                            NMLISTVIEW *arg = (LPNMLISTVIEW)lParam;
                            switch(arg->iSubItem)
                            {
                                case 0: //Checkboxes, do nothing
                                case 1: //Name
                                case 2: //Artist
                                case 3: //Album
                                case 4: //Genre
                                break;
                            }
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
    INITCOMMONCONTROLSEX InitCtrls;
    InitCtrls.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES;
    InitCtrls.dwSize = sizeof(INITCOMMONCONTROLSEX);
    InitCommonControlsEx(&InitCtrls);
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc);
}
