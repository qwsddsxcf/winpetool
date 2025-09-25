#include "消息处理函数.h"
TCHAR filepath[MAX_PATH] = TEXT("");
TCHAR savefilepath[MAX_PATH] = TEXT("");

char* filedata;
char* memdata;
char* dllname = NULL;
char* dllfun = NULL;
char* quduanstr = NULL;

int isopenpeui;
int showui = -1;
int ischosefile = 0;



HWND peuijubing= NULL;
HWND pequduanjubing = NULL;
HWND pemulujubing = NULL;


extern _In_ HINSTANCE hAppInstance;
extern int size;

void CloseAllWindows()
{

	if (peuijubing && IsWindow(peuijubing)) {
		SendMessage(peuijubing, WM_CLOSE, 0, 0);
		peuijubing = NULL;
	}

	if (pequduanjubing && IsWindow(pequduanjubing)) {
		SendMessage(pequduanjubing, WM_CLOSE, 0, 0);
		pequduanjubing = NULL;
	}

	if (pemulujubing && IsWindow(pemulujubing)) {
		SendMessage(pemulujubing, WM_CLOSE, 0, 0);
		pemulujubing = NULL;
	}
	isopenpeui = 0;
}



int freeneicun(int cfiledata,int cmemdata)
{
	if (cfiledata == 1) {
		if (filedata) {
			free(filedata);
			filedata = NULL;
		}
	}
	if (cmemdata == 1) {
		if (memdata) {
			free(memdata);
			memdata = NULL;
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

		initmoudlelist(hwndDlg, IDC_LIST1_model);
		initshowlist(hwndDlg, IDC_LIST4_info);
		//insertdemodata(hwndDlg, IDC_LIST1_model);
		showjinchengmaininfo(hwndDlg, IDC_LIST1_model);
		settubiao(hAppInstance, hwndDlg);
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		exit(0);
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			exit(0);
			return true;
		}
		return false;
	case WM_NOTIFY:
	{
		NMHDR* moreinfo = (NMHDR*)(lParam);
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
			exit(0);
			return TRUE;
		case IDC_BUTTON_mainshuaxin:
			showjinchengmaininfo(hwndDlg, IDC_LIST1_model);
			return true;
		case IDC_BUTTON_pesee:
			
			if (openpefile(hwndDlg, filepath, _countof(filepath))) {
				if (isopenpeui) {
					CloseAllWindows();
				}
				isopenpeui = 1;
				ischosefile = 1;
				showpeui(hAppInstance, hwndDlg);
			}
			return true;
		case IDC_EDIT1_save:
			if (ischosefile && savefilegetpath(hwndDlg, savefilepath, MAX_PATH)) {
				memdata = pememorytofile(memdata);
				savefile(memdata, savefilepath, size);
				TCHAR tmp[30] = _T("");
				_sntprintf_s(tmp, _countof(tmp), _T("成功保存文件到路径：%s"), savefilepath);
				MessageBox(NULL, tmp, TEXT("成功"), NULL);
			}
			else {
				MessageBox(NULL, TEXT("请先选择文件！"), TEXT("info"), NULL);
			}
			return true;
		case IDC_BUTTON_attackdemocode:
			if (ischosefile) {
				showpequduanshuruui(hAppInstance);
			}else {
				MessageBox(NULL, TEXT("请先选择文件！"), TEXT("info"), NULL);
			}
			return true;
		case IDC_BUTTON_pesee3_attackdll:
			//showpedllui
			if (ischosefile) {
				showpedllui(hAppInstance);
			}
			else {
				MessageBox(NULL, TEXT("请先选择文件！"), TEXT("info"), NULL);
			}
			return true;
		case IDC_BUTTON_about:
			showaboutthisui(hAppInstance);
			return true;
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
		peuijubing = hwndDlg;
		filedata = duqufile(filepath);
		if (filedata) {
			memdata = pefiletomemory(filedata);
			showpejichuinfo(filedata, hwndDlg);
		}
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		freeneicun(1, 1);
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			freeneicun(1, 1);
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
			freeneicun(1, 1);
			return TRUE;
		case IDC_BUTTON3_pequduan:
			if (pequduanjubing) {
				MessageBox(NULL,TEXT("已经存在该窗口！"), TEXT("info"), NULL);
				return true;
			}
			showpequduanui(hAppInstance);
			return true;
		case IDC_BUTTON2_pemulu:
			if (pemulujubing) {
				MessageBox(NULL, TEXT("已经存在该窗口！"), TEXT("info"), NULL);
				return true;
			}
			showpequmuluui(hAppInstance);
			return true;
		case IDC_BUTTON2_addquduan:
			apiaddquduaninmemory(memdata);
			return true;
		case IDC_BUTTON2_mvcdw:
			apimovcdwbiaoinmem(memdata);
			return true;
		case IDC_BUTTON2_mvdaochu:
			apimovedaochubiaoinmem(memdata);
			return true;
		case IDC_BUTTON2_mvdaoru:
			apimovedaorubiaoinmem(memdata);
			return true;
		default:
			break;
		}
	}
	}
	return FALSE;
}
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
		pequduanjubing = hwndDlg;
		initpequduanshowlist(hwndDlg, IDC_LIST2_pequduan);
		showpequduaninfo(filedata, hwndDlg, IDC_LIST2_pequduan);
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		pequduanjubing =NULL;
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { // 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			pequduanjubing =NULL;
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
		pemulujubing = hwndDlg;
		showpemuluinfo(filedata, hwndDlg);
		return true;
	case WM_CLOSE:
		EndDialog(hwndDlg, 0);
		pemulujubing = NULL;
		return TRUE;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) { 
			// 检查是否为关闭指令
			EndDialog(hwndDlg, 0);
			pemulujubing = NULL;
			return true;
		}
		return false;
	case  WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON2_pemuluexit:
			pemulujubing = NULL;
			EndDialog(hwndDlg, 0);
			return true;
		case IDC_BUTTON1_daochuinfo:
			showui = 0;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_daoruinfo:
			showui = 1;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_ziyuaninfo:
			showui = 2;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_cdwinfo:
			showui = 3;
			showpebiaoinfoui(hAppInstance);
			return true;
		case IDC_BUTTON1_iatinfo:
			showui = 4;
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
		switch (showui)
		{
		case 0:
			showpedaochubiaoinfo(filedata, hwndDlg);
			return true;
		case 1:
			
			showpedaorubiaoinfo(filedata, hwndDlg);
			return true;
		case 2:
			
			showpeziyuanbiaoinfo(filedata, hwndDlg, 18);
			return true;
		case 3:
			
			showpecdwbiaoinfo(filedata, hwndDlg);
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
		case IDOK_dllshuruqueding:
			dllname=getshuruinfothentoascii(hwndDlg, IDC_EDIT1_dllname);
			dllfun=getshuruinfothentoascii(hwndDlg, IDC_EDIT2_dllfun);
			apidaorubiaoattackinmem(memdata,dllname,dllfun);
			EndDialog(hwndDlg, 0);
			return true;
		case IDC_BUTTON1_demodll:
			dllname = (char*)"InjectDll.dll";
			dllfun = (char*)"ExportFunction";
			apidaorubiaoattackinmem(memdata,dllname,dllfun);
			EndDialog(hwndDlg, 0);
			return true;
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
		case IDOK_shuruquduanqueding:
			quduanstr= getshuruinfothentoascii(hwndDlg, IDC_EDIT1_shuruquduan);
			apimemoryquduanattcak(memdata, quduanstr);
			EndDialog(hwndDlg, 0);
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