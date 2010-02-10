#ifndef IPHONE_H_INCLUDED
#define IPHONE_H_INCLUDED

#include <windows.h>
#include "tungsten.h"

extern iPhone *phone;
extern bool phone_connected;

extern void *InitiPhone(HWND hwndDlg);

#endif // IPHONE_H_INCLUDED
