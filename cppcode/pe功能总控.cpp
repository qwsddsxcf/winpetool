
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "tool.h"
#include "��Ϣ������.h"
#include "resource.h"
#include <CommCtrl.h>
#include "��������.h"
#include "�ض�λ�����.h"
#include "���������.h"
#include "��������.h"
#include "ע�����.h"
#include "�ļ�����.h"
#include "��ȡ������ܺ󸽼�.h"
#include <string>
#include "pe�����ܿ�.h"

int lastquduanisfree = 0;
int start = 0;
extern int size;
namespace winpetoolfile {
	extern TCHAR filepath[MAX_PATH];
}


int showpeui(_In_ HINSTANCE hInstance,HWND dadjubing)
{
	TCHAR tmp[MAX_PATH] = _T("");
	_sntprintf_s(tmp, _countof(tmp), _T("pe���ߡ�by weizhi39����ѡ����ļ�·��:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_peui),   // dialog box template					
		NULL,      // handle to owner window					
		peuifun  // dialog box procedure					
	);
	return 1;
}

int showpequduanui(HINSTANCE hInstance)
{
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_pequduan),   // dialog box template					
		NULL,      // handle to owner window					
		pequduanfun // dialog box procedure					
	);
	return 1;
}

int showpequmuluui(HINSTANCE hInstance)
{
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_pemulu),   // dialog box template					
		NULL,      // handle to owner window					
		pemulufun // dialog box procedure					
	);
	return 1;
}

int showpebiaoinfoui(HINSTANCE hInstance)
{
	//LoadLibrary(TEXT("Msftedit.dll"));
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG_biaoinfo),   // dialog box template					
		NULL,      // handle to owner window					
		pebiaofun // dialog box procedure					
	);
	return 1;
}



int showpedllui(_In_ HINSTANCE hInstance)
{
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_dll),   // dialog box template					
		NULL,      // handle to owner window					
		pedllfun  // dialog box procedure					
	);
	return 1;
}
int showpequduanshuruui(_In_ HINSTANCE hInstance)
{
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_shuruquduan),   // dialog box template					
		NULL,      // handle to owner window					
		pequduanshurufun  // dialog box procedure					
	);
	return 1;
}


int showaboutthisui(_In_ HINSTANCE hInstance)
{
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_aboutthis),   // dialog box template					
		NULL,      // handle to owner window					
		pequaboutthisfun  // dialog box procedure					
	);
	return 1;
}
int showpeinfo(HWND dadjubing, TCHAR* outPath, DWORD outSize)
{

	return 0;
}

int apiaddquduaninmemory(char* &memdata) {
	char* tmp = memdata;
	char* tmpmemdata = addquduaninmemory(memdata,0x1000);
	if (tmpmemdata) {
		memdata = tmpmemdata;
		free(tmp);
		lastquduanisfree = 1;
		return 1;
	}
	return 0;
}
int apimovcdwbiaoinmem(char* & memdata) {
	if (lastquduanisfree == 0) {
		if (apiaddquduaninmemory(memdata) == 0) {
			return 0;
		}
	}
	int tmp=movcdwbiaoinmem(memdata, start);
	start = tmp + start;
	return 1;
}

int apimovedaochubiaoinmem(char*& memdata) {
	if (lastquduanisfree == 0) {
		if (apiaddquduaninmemory(memdata) == 0) {
			return 0;
		}
	}
	int tmp=movedaochubiaoinmem(memdata, start);
	start = tmp + start;
	return 1;
}
int apimovedaorubiaoinmem(char*& memdata) {
	if (lastquduanisfree == 0) {
		if (apiaddquduaninmemory(memdata) == 0) {
			return 0;
		}
	}
	int tmp = daorubiaomoveinmem(memdata, start);
	start = tmp + start;
	return 1;
}

int apireadsrctoshell(char* memdata, TCHAR* shellfilepath)
{
	char* shellfiledata = duqufile(shellfilepath);
	char* shellmemdata = pefiletomemory(shellfiledata);
	char* newshelldata=readsrctoshell(memdata, shellmemdata);
	char* newshellfiledata = pememorytofile(newshelldata);
	TCHAR filepath[] = TEXT("./newshell.exe");
	savefile(newshellfiledata, filepath, size);
	MessageBox(NULL, TEXT("�ѽ��ӿǺ���򱣴浽./newshell.exe"), TEXT("info"), NULL);
	return 1;
}

int apimemoryquduanattcak(char* memdata, char* quduan) {
	std::string asciiText = quduan;
	int number = atoi(asciiText.c_str());
	1 + 1;
	memoryquduanattcak(memdata, number);
	return 1;
}
int apidaorubiaoattackinmem(char* &memdata,char* dllname,char* dllfun) {
	if (lastquduanisfree == 0) {
		if (apiaddquduaninmemory(memdata) == 0) {
			return 0;
		}
	}
	int tmp = daorubiaoattackinmem(memdata, start, dllname,dllfun);
	start = tmp + start;
	return 1;
}
bool openpefile(HWND dadjubing, TCHAR* outPath, DWORD outSize)
{
	OPENFILENAME pefileinfo{0};
	pefileinfo.lStructSize = sizeof(pefileinfo);
	pefileinfo.hwndOwner = dadjubing;
	pefileinfo.lpstrFile = outPath;               // ����·���Ļ�����
	pefileinfo.nMaxFile = outSize;

	pefileinfo.lpstrFilter = _T("PE Files (*.exe;*.dll;*.sys)\0*.exe;*.dll;*.sys\0All Files (*.*)\0*.*\0");
	pefileinfo.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	pefileinfo.lpstrDefExt = _T("exe");              // Ĭ����չ��

	if (GetOpenFileName(&pefileinfo)) {
		//MessageBox(dadjubing, outPath, _T("��ѡ��� PE �ļ�"), MB_OK);
		return true;
	}
	return false;
}
BOOL savefilegetpath(HWND dadjubing, LPTSTR szFileName, DWORD nMaxFileName)
{
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = dadjubing;
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = nMaxFileName;
	ofn.lpstrFilter = _T("��ִ���ļ� (*.exe)\0*.exe\0�����ļ� (*.*)\0*.*\0");;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = _T("exe");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	return GetSaveFileName(&ofn);
}