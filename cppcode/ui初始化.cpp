#include "ui初始化.h"

void initmoudlelist(HWND hwndDlg, int id)
{
	HWND moudlejubing;
	LV_COLUMN lv;
	memset(&lv, 0, sizeof(LV_COLUMN));
	moudlejubing = GetDlgItem(hwndDlg, id);
	SendMessage(moudlejubing, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


	TCHAR t1[] = TEXT("进程");
	TCHAR t2[] = TEXT("PID");
	TCHAR t3[] = TEXT("镜像基址");
	TCHAR t4[] = TEXT("镜像大小");
	//第一列								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = t1;				//列标题				
	lv.cx = 200;								//列宽
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//第二列								
	lv.pszText = t2;
	lv.cx = 100;
	lv.iSubItem = 1;
	//ListView_InsertColumn(hListProcess, 1, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	//第三列								
	lv.pszText = t3;
	lv.cx = 100;
	lv.iSubItem = 2;
	ListView_InsertColumn(moudlejubing, 2, &lv);
	//第四列								
	lv.pszText = t4;
	lv.cx = 100;
	lv.iSubItem = 3;
	ListView_InsertColumn(moudlejubing, 3, &lv);
}

void insertdemodata(HWND hwndDlg, int id)
{
	LV_ITEM vitem;
	HWND moudlejubing;
	//初始化								
	memset(&vitem, 0, sizeof(LV_ITEM));
	moudlejubing = GetDlgItem(hwndDlg, id);

	vitem.mask = LVIF_TEXT;

	vitem.pszText = (TCHAR*)TEXT("csrss.exe");
	vitem.iItem = 0;
	vitem.iSubItem = 0;
	//ListView_InsertItem(hListProcess, &vitem);								
	SendMessage(moudlejubing, LVM_INSERTITEM, 0, (DWORD)&vitem);

	vitem.pszText = (TCHAR*)TEXT("448");
	vitem.iItem = 0;
	vitem.iSubItem = 1;
	ListView_SetItem(moudlejubing, &vitem);

	vitem.pszText = (TCHAR*)TEXT("56590000");
	vitem.iItem = 0;
	vitem.iSubItem = 2;
	ListView_SetItem(moudlejubing, &vitem);

	vitem.pszText = (TCHAR*)TEXT("000F0000");
	vitem.iItem = 0;
	vitem.iSubItem = 3;
	ListView_SetItem(moudlejubing, &vitem);

	vitem.pszText = (TCHAR*)TEXT("winlogon.exe");
	vitem.iItem = 1;
	vitem.iSubItem = 0;
	//ListView_InsertItem(hListProcess, &vitem);								
	SendMessage(moudlejubing, LVM_INSERTITEM, 0, (DWORD)&vitem);

	vitem.pszText = (TCHAR*)TEXT("456");
	vitem.iSubItem = 1;
	ListView_SetItem(moudlejubing, &vitem);

	vitem.pszText = (TCHAR*)TEXT("10000000");
	vitem.iSubItem = 2;
	ListView_SetItem(moudlejubing, &vitem);

	vitem.pszText = (TCHAR*)TEXT("000045800");
	vitem.iSubItem = 3;
	ListView_SetItem(moudlejubing, &vitem);

}

void initshowlist(HWND hwndDlg, int id)
{
	HWND moudlejubing;
	LV_COLUMN lv;
	memset(&lv, 0, sizeof(LV_COLUMN));
	moudlejubing = GetDlgItem(hwndDlg, id);
	SendMessage(moudlejubing, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	TCHAR t1[] = TEXT("模块名称");
	TCHAR t2[] = TEXT("模块位置");
	//第一列								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = t1;				//列标题				
	lv.cx = 300;								//列宽
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//第二列								
	lv.pszText = t2;
	lv.cx = 300;
	lv.iSubItem = 1;
	//ListView_InsertColumn(hListProcess, 1, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 1, (DWORD)&lv);

}

void initpequduanshowlist(HWND hwndDlg, int id)
{
	HWND moudlejubing;
	LV_COLUMN lv;
	memset(&lv, 0, sizeof(LV_COLUMN));
	moudlejubing = GetDlgItem(hwndDlg, id);
	SendMessage(moudlejubing, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	TCHAR t1[] = TEXT("区段名称");
	TCHAR t2[] = TEXT("内存偏移");
	TCHAR t3[] = TEXT("内存大小");
	TCHAR t4[] = TEXT("文件偏移");
	TCHAR t5[] = TEXT("文件大小");
	TCHAR t6[] = TEXT("区段属性");
	//第一列								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = t1;				//列标题				
	lv.cx = 100;								//列宽
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//第二列								
	lv.pszText = t2;
	lv.cx = 100;
	lv.iSubItem = 1;
	//ListView_InsertColumn(hListProcess, 1, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	//第三列								
	lv.pszText = t3;
	lv.cx = 100;
	lv.iSubItem = 2;
	ListView_InsertColumn(moudlejubing, 2, &lv);
	//第四列								
	lv.pszText = t4;
	lv.cx = 100;
	lv.iSubItem = 3;
	ListView_InsertColumn(moudlejubing, 3, &lv);
	lv.pszText = t5;
	lv.cx = 100;
	lv.iSubItem = 4;
	ListView_InsertColumn(moudlejubing, 4, &lv);
	lv.pszText = t6;
	lv.cx = 100;
	lv.iSubItem = 5;
	ListView_InsertColumn(moudlejubing, 5, &lv);

}

void settubiao(HINSTANCE hAppInstance, HWND hwndDlg)
{
	HICON hIcon;
	hIcon = LoadIcon(hAppInstance, MAKEINTRESOURCE(IDI_ICON1_my1));
	//设置图标		
	SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon);
	SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon);


}
