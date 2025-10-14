//#include "������Ϣչʾ.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "resource.h"
#include "tool.h"
#include <Psapi.h>
#include <CommCtrl.h>
#include <TlHelp32.h>

struct ModuleInfo
{
	TCHAR szModuleName[MAX_PATH];
	TCHAR szBaseAddress[32];
};

int showjinchengmaininfo(HWND dadjubing, int id)
{

	HWND listjubing= GetDlgItem(dadjubing, id);

	ListView_DeleteAllItems(listjubing);
	DWORD arrpid[1024], copysize1, pidnum;
	
	if(!EnumProcesses(arrpid, sizeof(arrpid), &copysize1))return 0;

	pidnum = copysize1 / 4;
	for (int i = 0; i < pidnum; i++)
	{
		DWORD pid = arrpid[i];
		if (pid == 0)continue;

		HANDLE jinchengjubing = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
			FALSE, pid);

		if (!jinchengjubing)continue;

		HMODULE jinchengmainmoudlejubing;
		DWORD copysize2;
		if (!EnumProcessModules(jinchengjubing, &jinchengmainmoudlejubing, sizeof(jinchengmainmoudlejubing)
			, &copysize2)) {
			CloseHandle(jinchengjubing);
			continue;
		}

		TCHAR mainmoudlename[MAX_PATH] = TEXT("δ֪");
		GetModuleBaseName(jinchengjubing, jinchengmainmoudlejubing, mainmoudlename, MAX_PATH);

		MODULEINFO moduleinfo;
		if (!GetModuleInformation(jinchengjubing, jinchengmainmoudlejubing, &moduleinfo, sizeof(moduleinfo))) {
			CloseHandle(jinchengjubing);
			continue;
		};

		LV_ITEM data{0};

		data.mask = LVIF_TEXT;
		data.iItem = ListView_GetItemCount(listjubing);
		data.iSubItem = 0;
		data.pszText = mainmoudlename;
		SendMessage(listjubing, LVM_INSERTITEM, 0, (DWORD)&data);
		/*ListView_SetItem(listjubing, &data);*/

		TCHAR strpid[32];
		_stprintf_s(strpid, _T("%u"), pid);
		data.mask = LVIF_TEXT;
		data.iSubItem = 1;
		data.pszText = strpid;
		ListView_SetItem(listjubing, &data);

		TCHAR strmodule[32];
		_stprintf_s(strmodule, _T("0x%p"), moduleinfo.lpBaseOfDll);
		data.mask = LVIF_TEXT;
		data.iSubItem = 2;
		data.pszText = strmodule;
		ListView_SetItem(listjubing, &data);

		TCHAR strmodulesize[32];
		_stprintf_s(strmodulesize, _T("0x%p"), moduleinfo.SizeOfImage);
		data.mask = LVIF_TEXT;
		data.iSubItem = 3;
		data.pszText = strmodulesize;
		ListView_SetItem(listjubing, &data);

	}

	return 1;
}

int showmorejinchengmaininfo(LPARAM moreinfo, HWND dadjubing,int id)
{
	NMHDR* pNMHDR = (NMHDR*)(moreinfo);
	HWND moudlejubing = GetDlgItem(dadjubing, id);
	ListView_DeleteAllItems(moudlejubing);
	NMITEMACTIVATE* pItemActivate = (NMITEMACTIVATE*)(moreinfo);
	int hang = pItemActivate->iItem;
	if (hang == -1)return 0;
	TCHAR szProcessName[MAX_PATH] = { 0 };  // ����������0��
	TCHAR szPID[32] = { 0 };                // PID����1��
	TCHAR szBaseAddr[32] = { 0 };            // �����ַ����2��
	TCHAR szImageSize[32] = { 0 };

	ListView_GetItemText(
		pNMHDR->hwndFrom,  // ListView�ľ��������NMHDR��
		hang,             // ��Ŀ�������кţ�
		0,                 // ������������0����������
		szProcessName,     // Ŀ�껺����
		MAX_PATH            // ��������С
	);

	ListView_GetItemText(pNMHDR->hwndFrom, hang, 1, szPID, _countof(szPID));
	ListView_GetItemText(pNMHDR->hwndFrom, hang, 2, szBaseAddr, _countof(szBaseAddr));
	ListView_GetItemText(pNMHDR->hwndFrom, hang, 3, szImageSize, _countof(szImageSize));


	DWORD mokuai = _ttoi(szPID);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, mokuai);
	MODULEENTRY32 me32 = { 0 };
	me32.dwSize = sizeof(MODULEENTRY32);

	if (Module32First(hSnapshot, &me32))
	{
		int index = 0;
		do
		{
			ModuleInfo info = { 0 };

			// ����ģ������
			_tcsncpy_s(info.szModuleName, me32.szModule, _countof(info.szModuleName) - 1);

			// ��ʽ������ַ
			_stprintf_s(info.szBaseAddress, _T("0x%08X"), (DWORD)me32.modBaseAddr);

			LV_ITEM data{ 0 };

			data.mask = LVIF_TEXT;
			data.iItem = index;
			data.iSubItem = 0;
			data.pszText = info.szModuleName;
			SendMessage(moudlejubing, LVM_INSERTITEM, 0, (DWORD)&data);

			data.mask = LVIF_TEXT;
			data.iItem = index;
			data.iSubItem = 1;
			data.pszText = info.szBaseAddress;
			ListView_SetItem(moudlejubing, &data);
			index++;

		} while (Module32Next(hSnapshot, &me32));
	}

	MessageBox(NULL, TEXT("�ɹ�����ģ��"), TEXT("�ɹ�"), NULL);
	return 1;
}



