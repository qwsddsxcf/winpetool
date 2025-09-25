#pragma once
#include "framework.h"
int showpeui(_In_ HINSTANCE hInstance, HWND dadjubing);
int showpequduanui(_In_ HINSTANCE hInstance);
int showpequmuluui(_In_ HINSTANCE hInstance);
int showpebiaoinfoui(_In_ HINSTANCE hInstance);
int showpeinfo(HWND dadjubing, TCHAR* outPath, DWORD outSize);
int showpedllui(_In_ HINSTANCE hInstance);
int showpequduanshuruui(_In_ HINSTANCE hInstance);
int showaboutthisui(_In_ HINSTANCE hInstance);

int apiaddquduaninmemory(char*& memdata);
int apimovcdwbiaoinmem(char*& memdata);
int apimovedaochubiaoinmem(char*& memdata);
int apimovedaorubiaoinmem(char*& memdata);

int apimemoryquduanattcak(char* memdata, char* quduan);
int apidaorubiaoattackinmem(char* &memdata, char* dllname, char* dllfun);

bool openpefile(HWND dadjubing, TCHAR* outPath, DWORD outSize);
BOOL savefilegetpath(HWND dadjubing, LPTSTR szFileName, DWORD nMaxFileName);