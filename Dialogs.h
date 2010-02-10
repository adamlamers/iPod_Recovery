#ifndef DIALOGS_H_INCLUDED
#define DIALOGS_H_INCLUDED

#include <windows.h>
#include <shlobj.h>

class Dialogs
{
    public:
        static char *OpenFile(HWND, LPSTR);
        static char *FolderBrowser(HWND);
};

#endif // DIALOGS_H_INCLUDED
