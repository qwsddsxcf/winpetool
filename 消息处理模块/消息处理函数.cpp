#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../工具模块/tool.h"
#include "../资源模块/resource.h"
#include "../函数总控调用模块/pe功能总控.h"
#include "../pe静态操作/pe基础信息展示.h"
#include "../进程操作模块/进程信息展示.h"
#include "../工具模块/文件操作.h"
#include "../ui模块/ui初始化.h"
#include "../工具模块/检查.h"
#include <CommCtrl.h>

extern _In_ HINSTANCE hAppInstance;
extern int size;
namespace winpetoolfile {
	TCHAR filepath[MAX_PATH] = TEXT("");
	TCHAR savefilepath[MAX_PATH] = TEXT("");
	char* filedata;
	char* memdata;
}
namespace winpetooljubing {

	HWND peuijubing = NULL;
	HWND pequduanjubing = NULL;
	HWND pemulujubing = NULL;
}
namespace winpetoolkey {
	int isopenpeui;
	int showui = -1;
	int ischosefile = 0;
	int isrundllzhuru;
}
namespace winpejinchenginfo {
	extern int ischosejincheng;
	extern int isrunjinchengzhuru;
	extern int isiathook;
}
void CloseAllWindows()
{

	if (winpetooljubing::peuijubing && IsWindow(winpetooljubing::peuijubing)) {
		SendMessage(winpetooljubing::peuijubing, WM_CLOSE, 0, 0);
		winpetooljubing::peuijubing = NULL;
	}

	if (winpetooljubing::pequduanjubing && IsWindow(winpetooljubing::pequduanjubing)) {
		SendMessage(winpetooljubing::pequduanjubing, WM_CLOSE, 0, 0);
		winpetooljubing::pequduanjubing = NULL;
	}

	if (winpetooljubing::pemulujubing && IsWindow(winpetooljubing::pemulujubing)) {
		SendMessage(winpetooljubing::pemulujubing, WM_CLOSE, 0, 0);
		winpetooljubing::pemulujubing = NULL;
	}
	winpetoolkey::isopenpeui = 0;
}

int freeneicun(int cfiledata,int cmemdata)
{
	if (cfiledata == 1) {
		if (winpetoolfile::filedata) {
			free(winpetoolfile::filedata);
			winpetoolfile::filedata = NULL;
		}
	}
	if (cmemdata == 1) {
		if (winpetoolfile::memdata) {
			free(winpetoolfile::memdata);
			winpetoolfile::memdata = NULL;
		}
	}
	return 1;
}

BOOL CALLBACK mainfun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		apichuangjiangoongxiangmem();//创建进程共享内存，用于进程相关操作功能
		//初始化list组件
		initmoudlelist(hwndDlg, IDC_LIST1_model);
		initshowlist(hwndDlg, IDC_LIST4_info);
		showjinchengmaininfo(hwndDlg, IDC_LIST1_model);//在list展示当前32进程
		settubiao(hAppInstance, hwndDlg);//设置应用程序图标
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		apijinchengcaozuo(5);//结束时将共享内存值改为exit
		exit(0);
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			apijinchengcaozuo(5);
			exit(0);
			return true;
		}
		return false;
	case WM_NOTIFY:
	{
		//展示进程详细信息，如基址，模块
		NMHDR* moreinfo = (NMHDR*)(lParam); //lParam中存储的是该动作的（点击）详细信息（多少行）
		if (moreinfo->idFrom == IDC_LIST1_model && moreinfo->code == NM_CLICK) {
			showmorejinchengmaininfo(lParam, hwndDlg, IDC_LIST4_info);
			return true;
		}
		return false;
	}

	case  WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_exit:
			EndDialog(hwndDlg, 0);
			apijinchengcaozuo(5);
			exit(0);
			return TRUE;
		case IDC_BUTTON_mainshuaxin:
			showjinchengmaininfo(hwndDlg, IDC_LIST1_model);
			return true;
		case IDC_BUTTON_pesee:
			//这里进行选择文件的操作，该操作是在打开pe查看敞口
			if (openpefile(hwndDlg, winpetoolfile::filepath, _countof(winpetoolfile::filepath))) {
				//这里检查是否已经选择了文件，已选择将关闭原来的窗口
				if (winpetoolkey::isopenpeui) {
					CloseAllWindows();
				}
				winpetoolkey::isopenpeui = 1;
				winpetoolkey::ischosefile = 1;
				showpeui(hAppInstance, hwndDlg);
			}
			return true;
		case IDC_EDIT1_save: {
			if (winpetoolkey::ischosefile && savefilegetpath(hwndDlg, winpetoolfile::savefilepath, MAX_PATH)) {
				char* filedata = pememorytofile(winpetoolfile::memdata);
				savefile(filedata, winpetoolfile::savefilepath, size);
				TCHAR tmp[80] = _T("");
				_sntprintf_s(tmp, _countof(tmp), _T("成功保存文件到路径：%s"), winpetoolfile::savefilepath);
				MessageBox(NULL, tmp, TEXT("成功"), NULL);
			}
			else {
				MessageBox(NULL, TEXT("请先选择文件！"), TEXT("info"), NULL);
			}
			return true;
		}
		case IDC_BUTTON_attackdemocode:
			if (winpetoolkey::ischosefile) {
				showpequduanshuruui(hAppInstance);
			}else {
				MessageBox(NULL, TEXT("请先选择文件！"), TEXT("info"), NULL);
			}
			return true;
		case IDC_BUTTON_pesee3_attackdll:
			//showpedllui
			if (winpetoolkey::ischosefile) {
				winpetoolkey::isrundllzhuru = 0;
				showpedllui(hAppInstance);
			}
			else {
				MessageBox(NULL, TEXT("请先选择文件！"), TEXT("info"), NULL);
			}
			return true;
		case IDC_BUTTON_about:
			showaboutthisui(hAppInstance);
			return true;
		case IDC_BUTTON_jiake: {
			if (winpetoolkey::ischosefile) {
				TCHAR shellfilepath[MAX_PATH] = TEXT("");
				if (openpefile(hwndDlg, shellfilepath, MAX_PATH)) {
					apireadsrctoshell(winpetoolfile::memdata, shellfilepath);
				}
				else {
					MessageBox(NULL, TEXT("打开壳程序失败！"), TEXT("error"), NULL);
				}
			}
			else {
				MessageBox(NULL, TEXT("请先选择文件(要加壳的程序)！"), TEXT("info"), NULL);
			}
			return true;
		}
		case IDC_BUTTON2_dongdllzhuru: {
			if (winpejinchenginfo::ischosejincheng)
			{
				winpetoolkey::isrundllzhuru = 1;
				showjinchengdllui(hAppInstance);
				return true;
			}else{
				MessageBox(NULL, TEXT("请先选择进程！"), TEXT("info"), NULL);
			}
			return true;
		}
		case IDC_BUTTON5_funjiankong: {
			if (!winpejinchenginfo::ischosejincheng) {
				MessageBox(NULL, TEXT("请先选择进程！"), TEXT("info"), NULL);
				return true;
			}
			TCHAR shellfilepath[MAX_PATH] = TEXT("");
			if (!winpejinchenginfo::isrunjinchengzhuru && openpefile(hwndDlg, shellfilepath, MAX_PATH)) {
				if(!apijinchengneicunzhuru(shellfilepath)) return true;
			}
			HWND hButton = GetDlgItem(hwndDlg, IDC_BUTTON5_funjiankong);
			if (winpejinchenginfo::isiathook) {
				winpejinchenginfo::isiathook = 0;
				apijinchengcaozuo(1);
				SetWindowText(hButton, L"取消监控");
			}
			else {
				winpejinchenginfo::isiathook = 1;
				apijinchengcaozuo(2);
				SetWindowText(hButton, L"函数监控");
			}
			return true;
		}
		case IDC_BUTTON6_funcall: {
			if (!winpejinchenginfo::ischosejincheng) {
				MessageBox(NULL, TEXT("请先选择进程！"), TEXT("info"), NULL);
				return true;
			}
			TCHAR shellfilepath[MAX_PATH] = TEXT("");
			if (!winpejinchenginfo::isrunjinchengzhuru && openpefile(hwndDlg, shellfilepath, MAX_PATH)) {
				if (!apijinchengneicunzhuru(shellfilepath)) return true;
			}
			apijinchengcaozuo(3);
			return true;
		}
		case IDC_BUTTON7_runcode: {
			if (!winpejinchenginfo::ischosejincheng) {
				MessageBox(NULL, TEXT("请先选择进程！"), TEXT("info"), NULL);
				return true;
			}
			TCHAR shellfilepath[MAX_PATH] = TEXT("");
			if (!winpejinchenginfo::isrunjinchengzhuru && openpefile(hwndDlg, shellfilepath, MAX_PATH)) {
				if (!apijinchengneicunzhuru(shellfilepath)) return true;
			}
			apijinchengcaozuo(4);
			return true;
		}
		default:
			break;
		}
	}

	}
	return FALSE;
}

BOOL CALLBACK peuifun(
	HWND hwndDlg,  // handle to dialog box			
	UINT uMsg,     // message			
	WPARAM wParam, // first message parameter			
	LPARAM lParam  // second message parameter			
)
{
	switch (uMsg)
	{
	case WM_INITDIALOG:
		initfanhuibianlist(hwndDlg, IDC_LIST2_fanhuibian);
		winpetooljubing::peuijubing = hwndDlg;
		winpetoolfile::filedata = duqufile(winpetoolfile::filepath);
		if (!checkiswin32pe(winpetoolfile::filedata)) {
			winpetoolkey::ischosefile = 0;
			MessageBox(NULL, TEXT("该文件不是win32pe文件,请重新选择！"), TEXT("info"), NULL);
			EndDialog(hwndDlg, 0);
			return true;
		}
		if (winpetoolfile::filedata) {
			winpetoolfile::memdata = pefiletomemory(winpetoolfile::filedata);
			showpejichuinfo(winpetoolfile::filedata, hwndDlg);
		}
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
	case WM_NOTIFY:
	{
		return false;
	}

	case  WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON5_peexit:
			EndDialog(hwndDlg, 0);
			return TRUE;
		case IDC_BUTTON3_pequduan:
			if (winpetooljubing::pequduanjubing) {
				MessageBox(NULL,TEXT("已经存在该窗口！"), TEXT("info"), NULL);
				return true;
			}
			showpequduanui(hAppInstance);
			return true;
		case IDC_BUTTON2_pemulu:
			if (winpetooljubing::pemulujubing) {
				MessageBox(NULL, TEXT("已经存在该窗口！"), TEXT("info"), NULL);
				return true;
			}
			showpequmuluui(hAppInstance);
			return true;
		case IDC_BUTTON2_addquduan:
			apiaddquduaninmemory(winpetoolfile::memdata);
			return true;
		case IDC_BUTTON2_mvcdw:
			apimovcdwbiaoinmem(winpetoolfile::memdata);
			return true;
		case IDC_BUTTON2_mvdaochu:
			apimovedaochubiaoinmem(winpetoolfile::memdata);
			return true;
		case IDC_BUTTON2_mvdaoru:
			apimovedaorubiaoinmem(winpetoolfile::memdata);
			return true;
		case IDC_BUTTON5_pefanhuibian:
			apifanhuibianmain(winpetoolfile::memdata, hwndDlg, IDC_LIST2_fanhuibian);
			return true;
		default:
			break;
		}
	}
	}
	return FALSE;
}



