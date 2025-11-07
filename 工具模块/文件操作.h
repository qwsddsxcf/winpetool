#pragma once
#include<stdio.h>
#include<windows.h>
char* duqufile(TCHAR* filepath);
char* pefiletomemory(char* fildata);
char* pememorytofile(char* memdata);
void savefile(char* filedata, TCHAR* filepath,int size);