#include "ui��ʼ��.h"

void initmoudlelist(HWND hwndDlg, int id)
{
	HWND moudlejubing;
	LV_COLUMN lv;
	memset(&lv, 0, sizeof(LV_COLUMN));
	moudlejubing = GetDlgItem(hwndDlg, id);
	SendMessage(moudlejubing, LVM_SETEXTENDEDLISTVIEWSTYLE, LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);


	TCHAR t1[] = TEXT("����");
	TCHAR t2[] = TEXT("PID");
	TCHAR t3[] = TEXT("�����ַ");
	TCHAR t4[] = TEXT("�����С");
	//��һ��								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = t1;				//�б���				
	lv.cx = 200;								//�п�
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//�ڶ���								
	lv.pszText = t2;
	lv.cx = 100;
	lv.iSubItem = 1;
	//ListView_InsertColumn(hListProcess, 1, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	//������								
	lv.pszText = t3;
	lv.cx = 100;
	lv.iSubItem = 2;
	ListView_InsertColumn(moudlejubing, 2, &lv);
	//������								
	lv.pszText = t4;
	lv.cx = 100;
	lv.iSubItem = 3;
	ListView_InsertColumn(moudlejubing, 3, &lv);
}

void insertdemodata(HWND hwndDlg, int id)
{
	LV_ITEM vitem;
	HWND moudlejubing;
	//��ʼ��								
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

	TCHAR t1[] = TEXT("ģ������");
	TCHAR t2[] = TEXT("ģ��λ��");
	//��һ��								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = t1;				//�б���				
	lv.cx = 300;								//�п�
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//�ڶ���								
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

	TCHAR t1[] = TEXT("��������");
	TCHAR t2[] = TEXT("�ڴ�ƫ��");
	TCHAR t3[] = TEXT("�ڴ��С");
	TCHAR t4[] = TEXT("�ļ�ƫ��");
	TCHAR t5[] = TEXT("�ļ���С");
	TCHAR t6[] = TEXT("��������");
	//��һ��								
	lv.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	lv.pszText = t1;				//�б���				
	lv.cx = 100;								//�п�
	lv.iSubItem = 0;
	//ListView_InsertColumn(hListProcess, 0, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 0, (DWORD)&lv);
	//�ڶ���								
	lv.pszText = t2;
	lv.cx = 100;
	lv.iSubItem = 1;
	//ListView_InsertColumn(hListProcess, 1, &lv);								
	SendMessage(moudlejubing, LVM_INSERTCOLUMN, 1, (DWORD)&lv);
	//������								
	lv.pszText = t3;
	lv.cx = 100;
	lv.iSubItem = 2;
	ListView_InsertColumn(moudlejubing, 2, &lv);
	//������								
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
	//����ͼ��		
	SendMessage(hwndDlg, WM_SETICON, ICON_BIG, (DWORD)hIcon);
	SendMessage(hwndDlg, WM_SETICON, ICON_SMALL, (DWORD)hIcon);


}
