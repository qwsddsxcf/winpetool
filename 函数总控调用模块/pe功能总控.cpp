#include "../工具模块/tool.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../进程操作模块/进程动态dll注入.h"

#include "../消息处理模块/消息处理函数.h"
#include "../资源模块/resource.h"
#include <CommCtrl.h>
#include "../pe静态操作/新增区段.h"
#include "../pe表操作解析/重定位表操作.h"
#include "../pe表操作解析/导出表操作.h"
#include "../pe表操作解析/导入表操作.h"
#include "../pe静态操作/注入代码.h"
#include "../工具模块/文件操作.h"
#include "../壳模块/读取程序加密后附加.h"
#include <string>
#include "pe功能总控.h"
#include "../工具模块/检查.h"
#include "../反汇编模块/反汇编引擎.h"
#include"../反汇编模块/反汇编tool.h"
#pragma warning(disable : 4996)
int lastquduanisfree = 0;
int start = 0;
extern int size;
namespace winpetoolfile {
	extern TCHAR filepath[MAX_PATH];
}
namespace winpejinchenginfo {
	extern TCHAR szPID[32];
	extern int isrunjinchengzhuru;
	extern char* gongxiangneicun;
}

int showpeui(_In_ HINSTANCE hInstance,HWND dadjubing)
{
	TCHAR tmp[MAX_PATH] = _T("");
	_sntprintf_s(tmp, _countof(tmp), _T("pe工具—by weizhi39，已选择的文件路径:%s,     已选择的进程id:%s"), winpetoolfile::filepath, winpejinchenginfo::szPID);
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

int showjinchengdllui(_In_ HINSTANCE hInstance)
{
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_dll),   // dialog box template					
		NULL,      // handle to owner window					
		pedllfun  // dialog box procedure					
	);
	return 1;
}
int apichuangjiangoongxiangmem() {
	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, L"weizhihack");
	winpejinchenginfo::gongxiangneicun = (char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 1024);
	return 1;
}
int apijinchengneicunzhuru(TCHAR * jinchengshellpath) {

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi;

	char* shellfiledata = duqufile(jinchengshellpath);
	if (!checkiswin32pe(shellfiledata)) {
		MessageBox(NULL, TEXT("该文件不是进程shell文件,请重新选择！"), TEXT("info"), NULL);
		return 0;
	}
	si.cb = sizeof(si);
	TCHAR tmp[MAX_PATH];
	_sntprintf_s(tmp, _countof(tmp), _T("%s  %s"), jinchengshellpath,winpejinchenginfo::szPID);

	BOOL res = CreateProcess(
		NULL,
		tmp,
		NULL,
		NULL,
		FALSE,
		CREATE_NEW_CONSOLE,
		NULL,
		NULL, &si, &pi);
	if (!res) {
		return 0;
	}
	winpejinchenginfo::isrunjinchengzhuru = 1;

	return 1;
}

int apijinchengcaozuo(int id) {
	switch (id)
	{
	case 1:
		sprintf(winpejinchenginfo::gongxiangneicun, "%s", "startiathook");
		break;
	case 2:
		sprintf(winpejinchenginfo::gongxiangneicun, "%s", "stopiathook");
		break;
	case 3:
		sprintf(winpejinchenginfo::gongxiangneicun, "%s", "diaoyong");
		break;
	case 4:
		sprintf(winpejinchenginfo::gongxiangneicun, "%s", "runcode");
		break;
	case 5:
		sprintf(winpejinchenginfo::gongxiangneicun, "%s", "exit");
		return 1;
		break;
	default:
		break;
	}
	MessageBox(NULL,TEXT("操作成功"), TEXT("成功"), NULL);
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
	if (!checkiswin32pe(shellfiledata)) {
		MessageBox(NULL, TEXT("该文件不是壳shell文件,请重新选择！"), TEXT("info"), NULL);
		return 0;
	}
	char* shellmemdata = pefiletomemory(shellfiledata);
	char* newshelldata=readsrctoshell(memdata, shellmemdata);
	char* newshellfiledata = pememorytofile(newshelldata);
	TCHAR filepath[] = TEXT("./newshell.exe");
	savefile(newshellfiledata, filepath, size);
	MessageBox(NULL, TEXT("已将加壳后程序保存到./newshell.exe"), TEXT("info"), NULL);
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

int apijinchengzhurudll(char* dllpath) {
	int tmppid = 0;
	swscanf_s(winpejinchenginfo::szPID, L"%d", &tmppid);
	if (tmppid == 0) {
		return 0;
	}
	if (jinchengzhurudll(tmppid, dllpath)) {
		return 1;
	}
	return 0;
}

bool openpefile(HWND dadjubing, TCHAR* outPath, DWORD outSize)
{
	OPENFILENAME pefileinfo{0};
	pefileinfo.lStructSize = sizeof(pefileinfo);
	pefileinfo.hwndOwner = dadjubing;
	pefileinfo.lpstrFile = outPath;               // 返回路径的缓冲区
	pefileinfo.nMaxFile = outSize;

	pefileinfo.lpstrFilter = _T("PE Files (*.exe;*.dll;*.sys)\0*.exe;*.dll;*.sys\0All Files (*.*)\0*.*\0");
	pefileinfo.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	pefileinfo.lpstrDefExt = _T("exe");              // 默认扩展名

	if (GetOpenFileName(&pefileinfo)) {
		//MessageBox(dadjubing, outPath, _T("已选择的 PE 文件"), MB_OK);
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
	ofn.lpstrFilter = _T("可执行文件 (*.exe)\0*.exe\0所有文件 (*.*)\0*.*\0");;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.lpstrDefExt = _T("exe");
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_HIDEREADONLY;

	return GetSaveFileName(&ofn);
}

int apifanhuibianmain(char *memdata,HWND dadjubing,int id)
{
	int pianyi = 0;
	int base = 0;
	HWND listjubing = GetDlgItem(dadjubing, id);
	ListView_DeleteAllItems(listjubing);
	toolgetperukoupianyi(memdata, pianyi, base);
	if (fabnhuibianmain(base + pianyi, memdata + pianyi, 0x500, dadjubing, id)==1) {
		MessageBox(NULL, TEXT("反汇编成功"), TEXT("成功"), NULL);
		return 1;
	}
	MessageBox(NULL, TEXT("反汇编失败"), TEXT("失败"), NULL);
	return 0;
}
