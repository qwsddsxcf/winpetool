#pragma once
//#include "framework.h"
#include <windows.h>
int showpejichuinfo(char* filedata, HWND dadjubing);
int showpequduaninfo(char* filedata, HWND dadjubing,int id);
int showpemuluinfo(char* filedata, HWND dadjubing);
int showpedaochubiaoinfo(char* filedata, HWND dadjubing);
int showpedaorubiaoinfo(char* filedata, HWND dadjubing);
int showpecdwbiaoinfo(char* filedata, HWND dadjubing);
int showpeziyuanbiaoinfo(char* filedata, HWND dadjubing,int id);
int showpeiatbiaoinfo(char* filedata, HWND dadjubing);

int showpequduan16zijie(LPARAM moreinfo, HWND dadjubing, int id);
int showpefanhuibian(HWND dadjubing, int id, int jizhi, char* yuanshizijie, char* fanhuibian);


DWORD WINAPI xianchengshowpequduan16zijie(LPVOID lpParameter);