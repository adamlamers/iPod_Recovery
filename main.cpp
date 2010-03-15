/* File: main.cpp
 * Creation Date: December 23rd, 2009
 * Last Modified Date: February 22nd, 2010
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
#include "songlist.h"
#include "statusbar.h"

HINSTANCE hInst;

CSongList *SongList;
CStatusBar *StatusBar;

HWND SearchBox;
HMENU SongListContextMenu;

int DisplayType = 0;
int songCount = 0;

BOOL InitSearchBox(HWND hwndDlg)
{
    SearchBox = GetDlgItem(hwndDlg, IDC_SEARCH);
    SendMessage(SearchBox, WM_SETTEXT, (WPARAM)0, (LPARAM)"Search...");
    return true;
}

void SongListItemAdded()
{
    char statusText[256];
    songCount++;
    sprintf(statusText, "%d Items", songCount);
    StatusBar->SetSegmentText(1, statusText);
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
            StatusBar = new CStatusBar(hwndDlg);
            InitSearchBox(hwndDlg);
            InitiPhone(hwndDlg);
        return TRUE;

        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
        return TRUE;
        
        case WM_SIZE:
            SongList->Scale();
            StatusBar->Scale();
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
                    //MessageBox(NULL, "Context Save Song", "", MB_OK);
                    SongList->ContextSaveSong();
                return TRUE;
                
                case IDM_CONTEXTCHECKSELECTED:
                    SongList->CheckSelectedItems(true);
                return TRUE;
                
                case IDM_CONTEXTUNCHECKSELECTED:
                    SongList->CheckSelectedItems(false);
                return TRUE;
                
                case IDM_EXIT1:
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
                                break;
                                case 1: //Name
                                SongList->Sort(1);
                                break;
                                case 2: //Artist
                                SongList->Sort(2);
                                break;
                                case 3: //Album
                                SongList->Sort(3);
                                break;
                                case 4: //Genre
                                SongList->Sort(4);
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
    InitCommonCtrls();
    return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc);
}
