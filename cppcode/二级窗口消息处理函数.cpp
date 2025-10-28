#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "tool.h"
#include "resource.h"
#include "pe功能总控.h"
#include "pe基础信息展示.h"
#include "文件操作.h"
#include "ui初始化.h"
#include <CommCtrl.h>
namespace winpetoolfile {
	extern TCHAR filepath[MAX_PATH];
	extern TCHAR savefilepath[MAX_PATH];
	extern char* filedata;
	extern char* memdata;
}
namespace winpetooljubing {

	extern HWND peuijubing ;
	extern HWND pequduanjubing ;
	extern HWND pemulujubing;
}
namespace winpetoolkey {
	extern int isopenpeui;
	extern int showui;
	extern int ischosefile;
	extern int isrundllzhuru;
}
extern _In_ HINSTANCE hAppInstance;

BOOL CALLBACK pequduanfun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		winpetooljubing::pequduanjubing = hwndDlg;
		initpequduanshowlist(hwndDlg, IDC_LIST2_pequduan);
		showpequduaninfo(winpetoolfile::filedata, hwndDlg, IDC_LIST2_pequduan);
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		winpetooljubing::pequduanjubing = NULL;
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			winpetooljubing::pequduanjubing = NULL;
			return true;
		}
		return false;

	case  WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		default:
			break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK pemulufun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		winpetooljubing::pemulujubing = hwndDlg;
		showpemuluinfo(winpetoolfile::filedata, hwndDlg);
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		winpetooljubing::pemulujubing = NULL;
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) {
			// 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			winpetooljubing::pemulujubing = NULL;
			return true;
		}
		return false;
	case  WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON2_pemuluexit:
			winpetooljubing::pemulujubing = NULL;
			EndDialog(hwndDlg, 0);
			return true;
		case IDC_BUTTON1_daochuinfo:
			winpetoolkey::showui = 0;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_daoruinfo:
			winpetoolkey::showui = 1;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_ziyuaninfo:
			winpetoolkey::showui = 2;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_cdwinfo:
			winpetoolkey::showui = 3;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_iatinfo:
			winpetoolkey::showui = 4;
			showpebiaoinfoui(hAppInstance);
			return true;
		default:
			break;
		}
	}
	}
	return FALSE;
}
BOOL CALLBACK pebiaofun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		switch (winpetoolkey::showui)
		{
		case 0:
			showpedaochubiaoinfo(winpetoolfile::filedata, hwndDlg);
			return true;
		case 1:
			showpedaorubiaoinfo(winpetoolfile::filedata, hwndDlg);
			return true;
		case 2:
			showpeziyuanbiaoinfo(winpetoolfile::filedata, hwndDlg, 18);
			return true;
		case 3:
			showpecdwbiaoinfo(winpetoolfile::filedata, hwndDlg);
			return true;
		case 4:

			return true;
		default:
			break;
		}
		return false;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) {
			// 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			return true;
		}
		return false;

	}
	return FALSE;
}

BOOL CALLBACK pedllfun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			return true;
		}
		return false;
	case  WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK_dllshuruqueding: {
			char* dllname = getshuruinfothentoascii(hwndDlg, IDC_EDIT1_dllname);
			char* dllfun = getshuruinfothentoascii(hwndDlg, IDC_EDIT2_dllfun);
			if (winpetoolkey::isrundllzhuru) {
				apijinchengzhurudll(dllname);
			}
			else {
				apidaorubiaoattackinmem(winpetoolfile::memdata, dllname, dllfun);
			}
			
			EndDialog(hwndDlg, 0);
			return true;
		}
		case IDC_BUTTON1_demodll: {
			char* dllname = (char*)"InjectDll.dll";
			char* dllfun = (char*)"ExportFunction";
			if (winpetoolkey::isrundllzhuru) {
				apijinchengzhurudll(dllname);
			}
			else {
				apidaorubiaoattackinmem(winpetoolfile::memdata, dllname, dllfun);
			}
			EndDialog(hwndDlg, 0);
			return true;
		}
		default:
			break;
		}
	}
	}
	return FALSE;
}
BOOL CALLBACK pequduanshurufun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			return true;
		}
		return false;
	case  WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDOK_shuruquduanqueding: {
			char* quduanstr = getshuruinfothentoascii(hwndDlg, IDC_EDIT1_shuruquduan);
			apimemoryquduanattcak(winpetoolfile::memdata, quduanstr);
			EndDialog(hwndDlg, 0);
			return true;
		}
		default:
			break;
		}
	}
	}
	return FALSE;
}

BOOL CALLBACK pequaboutthisfun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			return true;
		}
		return false;
	}
	return FALSE;
}