/* File: Dialogs.cpp
 * Creation Date: 12/23/2009 5:17PM
 * Last Modified Date: 12/23/2009 5:17PM
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#include "Dialogs.h"

char* Dialogs::OpenFile(HWND parent, LPSTR filter)
{
    OPENFILENAME ofn;
    char *szFileName = (char*)malloc(MAX_PATH);
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = parent;
    ofn.lpstrFilter = filter;
    ofn.lpstrFile = szFileName;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
    ofn.lpstrDefExt = "txt";
    if(GetOpenFileName(&ofn))
        return szFileName;
    else
        return NULL;
}

char* Dialogs::FolderBrowser(HWND parent)
{
        BROWSEINFO bi;
        char *szSelectedFolder = (char*)malloc(MAX_PATH);
        ZeroMemory(&bi, sizeof(bi));
        bi.hwndOwner = parent;
        bi.pidlRoot = NULL;
        bi.pszDisplayName = szSelectedFolder;
        bi.lpszTitle = "Select a Folder";
        LPITEMIDLIST idl = SHBrowseForFolder(&bi);
        if(idl != NULL)
        {
            if(!SHGetPathFromIDList(idl, szSelectedFolder)) return NULL;
            return szSelectedFolder;
        }
        else
            return NULL;
}
