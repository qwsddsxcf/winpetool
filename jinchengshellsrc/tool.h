#pragma once
#include<windows.h>
#include<stdio.h>
int xiufucdwbiaoinmem(char* memdata, int pianyi);
void zhurucode();
void runallcode();
int WINAPI myMessageBoxA(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType);