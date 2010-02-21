#include "tungsten.h"
#include "itdb.h"
#include "glib.h"
#include <windows.h>
#include <commctrl.h>

extern HWND SongList;
extern BOOL SongListAddRow(HWND listview, char *name, char *artist, char *album, char *genre);
extern int DisplayType;
extern HWND StatusBar;

iPhone *phone;
Itdb_iTunesDB *itunesdb;

void FetchITunesDB()
{
    iPhone_CopyFileFromDevice(phone, "/private/var/mobile/Media/iTunes_Control/iTunes/iTunesCDB", "iTunesCDB");
    ConvertiTunesCDB("iTunesCDB", "iTunesDB");
    itunesdb = itdb_parse_file("iTunesDB", NULL);
    GList *tracks = itunesdb->tracks;
    Itdb_Track *track;
    while(tracks->next != NULL)
    {
        track = (Itdb_Track*)tracks->data;
        //if(track->mediatype & ITDB_MEDIATYPE_AUDIO)
            SongListAddRow(SongList, track->title, track->artist, track->album, track->genre);
        tracks = tracks->next;
    }
}

DWORD WINAPI ThreadCallback(void *param)
{
    FetchITunesDB();
    return TRUE;
}

void NotifyCallback(AMDeviceNotificationCallbackInfo* callback)
{
    switch(callback->msg)
    {
        case Connected:
            phone->handle = callback->dev;
            if(iPhone_Connect(phone) == TRUE)
            {
                CreateThread(NULL, 0, ThreadCallback, NULL, 0, NULL);
            }
            SendMessage(StatusBar, SB_SETTEXT, 0, (LPARAM)"Connected");
        break;
        case Disconnected:
            phone->connected = FALSE;
        break;
        default:
        break;
    }
}

void InitiPhone(HWND hwndDlg)
{
    phone = (iPhone*)malloc(sizeof(iPhone));
    memset(phone, 0, sizeof(iPhone));
    phone->dnc = NotifyCallback;
    iPhone_init(phone);
}
