/* File: iphone.cpp
 * Creation Date: 2/4/2010
 * Last Modified Date: 2/21/2010
 * Version: 0.0.1
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

#include "tungsten.h"
#include "itdb.h"
#include "glib.h"
#include <windows.h>
#include <commctrl.h>
#include "songlist.h"
#include "statusbar.h"

#ifdef __cplusplus
extern "C"
{
#endif
extern void __declspec(dllimport) ConvertiTunesCDB(char *filePath, char *newFilePath);
#ifdef __cplusplus
}
#endif

extern CSongList *SongList;
extern int DisplayType;
extern CStatusBar *StatusBar;

iPhone *phone;
Itdb_iTunesDB *itunesdb;

void FetchITunesDB()
{
    std::list <Itdb_Track*> *tracksList = new std::list<Itdb_Track*>;
    iPhone_CopyFileFromDevice(phone, "/private/var/mobile/Media/iTunes_Control/iTunes/iTunesCDB", "iTunesCDB");
    ConvertiTunesCDB("iTunesCDB", "iTunesDB");
    itunesdb = itdb_parse_file("iTunesDB", NULL);
    GList *tracks = itunesdb->tracks;
    Itdb_Track *track;
    while(tracks != NULL)
    {
        track = (Itdb_Track*)tracks->data;
        tracksList->push_back(track);
        //if(track->mediatype & ITDB_MEDIATYPE_AUDIO)
            SongList->AddRow(track->title, track->artist, track->album, track->genre);
        tracks = tracks->next;
    }
    SongList->SetTrackList(tracksList);
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
            StatusBar->SetSegmentText(0, "Connected");
        break;
        case Disconnected:
            phone->connected = FALSE;
            StatusBar->SetSegmentText(0, "Disconnected");
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
