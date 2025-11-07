#pragma once
#include<windows.h>
int addressmemtofile(char*  ,int);
void AppendEditText(HWND hDlg, int editId, const TCHAR* newText);
char* getshuruinfothentoascii(HWND dadjubing,int id);