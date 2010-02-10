/* File: tungsten.h
 * Creation Date: December 17th, 2009
 * Last Modified Date: January 10th, 2010
 * Version: 0.0.14
 * Contact: Adam Lamers <adam@millenniumsoftworks.com>
*/

/********************************************************************************
* This file is part of Tungsten.                                                *
*                                                                               *
* Tungsten is free software: you can redistribute it and/or modify              *
* it under the terms of the GNU General Public License as published by          *
* the Free Software Foundation, either version 3 of the License, or             *
* (at your option) any later version.                                           *
*                                                                               *
* Tungsten is distributed in the hope that it will be useful,                   *
* but WITHOUT ANY WARRANTY; without even the implied warranty of                *
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                 *
* GNU General Public License for more details.                                  *
*                                                                               *
* You should have received a copy of the GNU General Public License             *
* along with Tungsten.  If not, see <http://www.gnu.org/licenses/>.             *
********************************************************************************/

#ifndef __TUNGSTEN_H__
#define __TUNGSTEN_H__
#include <windows.h>
#include "iTunesMobileDevice.h"

enum { AMD_FILE = 1, AMD_DIR, AMD_LINK, AMD_UNKNOWN };
enum { AMDFM_READ = 1, AMDFM_WRITE, AMDFM_READWRITE };

typedef struct iPhoneFile_tag
{
    uint64_t handle;
    int      mode;
    long     size;
}iPhoneFile;

typedef struct iPhone_tag
{
    DeviceNotificationCallback        dnc;  /** < Standard device notification callback */

    void* handle;                           /** < Handle to device. */
    void* hAFC;                             /** < com.apple.AFC service handle */
    void* hService;                         /** < Service handle */
    BOOL  connected;                        /** < Connection status */ 
}iPhone;

typedef struct iPhoneFileInfo_tag
{
    int    blocks;
    int    nlink;
    long   size;
    char  *ifmt;
} iPhoneFileInfo;

#define SHARED
#ifdef SHARED
 #ifdef BUILD_DLL
     #define DLL_EXPORT __declspec(dllexport)
 #else
     #define DLL_EXPORT __declspec(dllimport)
 #endif 
#endif
#ifdef STATIC
    #define DLL_EXPORT extern
#endif

#ifndef DLL_EXPORT
    #error Define SHARED or STATIC
#endif

#ifdef __cplusplus
extern "C"
{
#endif

DLL_EXPORT void        iPhone_init(iPhone*);
DLL_EXPORT BOOL        iPhone_Connect(iPhone*);
DLL_EXPORT BOOL        GetFiles(iPhone*,char*);
DLL_EXPORT BOOL        iPhone_FileExists(iPhone*,char*);
DLL_EXPORT long        iPhone_GetFileSize(iPhone*, char*);
DLL_EXPORT void        iPhone_WaitForConnect(iPhone*);
DLL_EXPORT char        *iPhone_GetLastError();
DLL_EXPORT iPhoneFile  *iPhone_fopen(iPhone *iphone, char *path, char *mode);
DLL_EXPORT void        iPhone_fclose(iPhone *iphone, iPhoneFile *file);
DLL_EXPORT long        iPhone_ftell(iPhone *iphone, iPhoneFile *file);
DLL_EXPORT int         iPhone_fread(iPhone *iphone, iPhoneFile *file, void *buffer, int count);
DLL_EXPORT int         iPhone_CopyFileFromDevice(iPhone*, char*, char*);

/* Private functions */
void iPhone_SetLastError(char *format, ...);

#ifdef __cplusplus
}
#endif

#endif // __TUNGSTEN_H__
