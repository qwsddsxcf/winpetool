//#include "pe基础信息展示.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../工具模块/tool.h"
#include "../资源模块/resource.h"
#include <CommCtrl.h>
#include "../pe表操作解析/资源表解析.h"
#include "../pe表操作解析/重定位表解析.h"
#include "../pe表操作解析/导入表解析.h"
#include "../pe表操作解析/导出表解析.h"
#include "pe基础信息展示.h"
TCHAR tmp[MAX_PATH] = _T("");
 namespace winpetoolfile {
	 extern TCHAR filepath[MAX_PATH];
	 extern char* filedata;
}

 struct pequduan16zijie {
	 HWND dadjubing;
	 int id;
	 int filepianyinum ;
	 int filesizenum;
 };

int showpejichuinfo(char* filedata, HWND dadjubing)
{
	
	if (filedata == NULL) {
		MessageBox(NULL, TEXT("文件内容为空"), TEXT("error"), NULL);
		return 0;
	}
	_sntprintf_s(tmp, _countof(tmp), _T("pe基础信息，文件路径:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);

	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

	TCHAR chengxurukou[32]=_T("");
	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->AddressOfEntryPoint);
	SetDlgItemText(dadjubing, IDC_EDIT1, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->ImageBase);
	SetDlgItemText(dadjubing, IDC_EDIT3, chengxurukou);

	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->SizeOfImage);
	SetDlgItemText(dadjubing, IDC_EDIT9, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->BaseOfCode);
	SetDlgItemText(dadjubing, IDC_EDIT6, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->BaseOfData);
	SetDlgItemText(dadjubing, IDC_EDIT7, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->SectionAlignment);
	SetDlgItemText(dadjubing, IDC_EDIT8, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->FileAlignment);
	SetDlgItemText(dadjubing, IDC_EDIT5, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pefileheader->Characteristics);
	SetDlgItemText(dadjubing, IDC_EDIT2, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->Subsystem);
	SetDlgItemText(dadjubing, IDC_EDIT4, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("%d"), pefileheader->NumberOfSections);
	SetDlgItemText(dadjubing, IDC_EDIT11, chengxurukou);

	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pefileheader->TimeDateStamp);
	SetDlgItemText(dadjubing, IDC_EDIT16, chengxurukou);

	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->SizeOfHeaders);
	SetDlgItemText(dadjubing, IDC_EDIT13, chengxurukou);
	//pefileheader->SizeOfOptionalHeader
	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->DllCharacteristics);
	SetDlgItemText(dadjubing, IDC_EDIT14, chengxurukou);

	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->CheckSum);
	SetDlgItemText(dadjubing, IDC_EDIT15, chengxurukou);


	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pefileheader->SizeOfOptionalHeader);
	SetDlgItemText(dadjubing, IDC_EDIT12, chengxurukou);

	_sntprintf_s(chengxurukou, _countof(chengxurukou), _T("0x%08X"), pekexuanheader->SizeOfCode);
	SetDlgItemText(dadjubing, IDC_EDIT10, chengxurukou);

	
	return 1;
}

int showpequduaninfo(char* filedata, HWND dadjubing,int id)
{
	if (filedata == NULL) {
		MessageBox(NULL, TEXT("文件内容为空"), TEXT("error"), NULL);
		return 0;
	}
	_sntprintf_s(tmp, _countof(tmp), _T("pe区段信息，文件路径:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);

	LV_ITEM vitem;
	HWND moudlejubing;							
	memset(&vitem, 0, sizeof(LV_ITEM));
	moudlejubing = GetDlgItem(dadjubing, id);

	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	int pequduannum = pefileheader->NumberOfSections;
	TCHAR tmp[32] = _T("");
	for (size_t i = 0; i < pequduannum; i++)
	{
			_sntprintf_s(tmp, _countof(tmp), _T("%hs"), "");
			vitem.mask = LVIF_TEXT;
			vitem.pszText = tmp;
			vitem.iItem = i;
			vitem.iSubItem = 0;
		    SendMessage(moudlejubing, LVM_INSERTITEM, 0, (DWORD)&vitem);

			_sntprintf_s(tmp, _countof(tmp), _T("%hs"), pequduanheader[i].Name);
			vitem.mask = LVIF_TEXT;
			vitem.pszText = tmp;
			vitem.iItem = i;
			vitem.iSubItem = 0;
			ListView_SetItem(moudlejubing,&vitem);
			

			_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pequduanheader[i].VirtualAddress);
			vitem.mask = LVIF_TEXT;
			vitem.pszText = tmp;
			vitem.iItem = i;
			vitem.iSubItem = 1;
			ListView_SetItem(moudlejubing, &vitem);

			_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pequduanheader[i].Misc.VirtualSize);
			vitem.mask = LVIF_TEXT;
			vitem.pszText = tmp;
			vitem.iItem = i;
			vitem.iSubItem = 2;
			ListView_SetItem(moudlejubing, &vitem);

			_sntprintf_s(tmp, _countof(tmp),_T("0x%08X"), pequduanheader[i].PointerToRawData);
			vitem.mask = LVIF_TEXT;
			vitem.pszText = tmp;
			vitem.iItem = i;
			vitem.iSubItem = 3;
			ListView_SetItem(moudlejubing,&vitem);

			_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pequduanheader[i].SizeOfRawData);
			vitem.mask = LVIF_TEXT;
			vitem.pszText = tmp;
			vitem.iItem = i;
			vitem.iSubItem = 4;
			ListView_SetItem(moudlejubing, &vitem);

			_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pequduanheader[i].Characteristics);
			vitem.mask = LVIF_TEXT;
			vitem.pszText = tmp;
			vitem.iItem = i;
			vitem.iSubItem = 5;
			ListView_SetItem(moudlejubing, &vitem);
	
	}

	return 1;
}

int showpemuluinfo(char* filedata, HWND dadjubing)
{
	if (filedata == NULL) {
		MessageBox(NULL, TEXT("文件内容为空"), TEXT("error"), NULL);
		return 0;
	}
	_sntprintf_s(tmp, _countof(tmp), _T("pe目录信息，文件路径:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);

	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

	TCHAR tmp[32] = _T("");

	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[0].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT1, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[0].Size);
	SetDlgItemText(dadjubing, IDC_EDIT2, tmp);

	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[1].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT5, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[1].Size);
	SetDlgItemText(dadjubing, IDC_EDIT6, tmp);

	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[2].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT7, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[2].Size);
	SetDlgItemText(dadjubing, IDC_EDIT8, tmp);

	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[3].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT17, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[3].Size);
	SetDlgItemText(dadjubing, IDC_EDIT9, tmp);

	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[4].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT4, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[4].Size);
	SetDlgItemText(dadjubing, IDC_EDIT11, tmp);

	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[5].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT12, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[5].Size);
	SetDlgItemText(dadjubing, IDC_EDIT13, tmp);


	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[6].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT14, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[6].Size);
	SetDlgItemText(dadjubing, IDC_EDIT15, tmp);


	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[7].VirtualAddress);
	SetDlgItemText(dadjubing, IDC_EDIT16, tmp);
	_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[7].Size);
	SetDlgItemText(dadjubing, IDC_EDIT18, tmp);

	int cindex = IDC_EDIT19;
	for (size_t i = 8; i < 15; i++)
	{
		_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[i].VirtualAddress);
		SetDlgItemText(dadjubing, cindex++, tmp);
		_sntprintf_s(tmp, _countof(tmp), _T("0x%08X"), pekexuanheader->DataDirectory[i].Size);
		SetDlgItemText(dadjubing, cindex++, tmp);
		if (cindex == 1039) {
			cindex += 2;
		}
	}
	
	return 1;
}

int showpedaochubiaoinfo(char* filedata, HWND dadjubing)
{
	_sntprintf_s(tmp, _countof(tmp), _T("导入表信息，文件路径:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);
	daochubiaojiexiinfile(filedata, dadjubing);
	return 1;
}

int showpedaorubiaoinfo(char* filedata, HWND dadjubing)
{
	_sntprintf_s(tmp, _countof(tmp), _T("导入表信息，文件路径:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);
	jiexidaorubiaoinfile(filedata, dadjubing);
	return 1;
}

int showpecdwbiaoinfo(char* filedata, HWND dadjubing)
{
	_sntprintf_s(tmp, _countof(tmp), _T("重定位表信息，文件路径:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);
	filechongdingweibiao(filedata, dadjubing);
	return 1;
}

int showpeziyuanbiaoinfo(char* filedata, HWND dadjubing,int id)
{
	_sntprintf_s(tmp, _countof(tmp), _T("资源表信息，文件路径:%s"), winpetoolfile::filepath);
	SetWindowText(dadjubing, tmp);
	ziyuanbiaojiexiinfile(filedata, dadjubing, id);
	return 1;
}

int showpeiatbiaoinfo(char* filedata, HWND dadjubing)
{
	return 0;
}

int showpequduan16zijie(LPARAM moreinfo, HWND dadjubing, int id) {
	//该函数用于展示用户点击一个区段详细原始文件16进制字节(创建线程)
	//moreinfo在该函数结束后，这个指针指向的一些数据会销毁导致另一个线程无法获取数据，所以先拿到
	//需要的信息，在传入另一个线程函数中使用
	
	struct pequduan16zijie* info = (pequduan16zijie*)malloc(sizeof(struct pequduan16zijie));

	NMHDR* pNMHDR = (NMHDR*)(moreinfo);

	NMITEMACTIVATE* pItemActivate = (NMITEMACTIVATE*)(moreinfo);
	int hang = pItemActivate->iItem;
	if (hang == -1)return 0;

	TCHAR filepianyi[20] = { 0 };
	TCHAR filesize[20] = { 0 };
	TCHAR tmp[80] = { 0 };
	//获取用户点击的哪一行字符串转换为整数，文件偏移，大小
	ListView_GetItemText(pNMHDR->hwndFrom, hang, 3, filepianyi, _countof(filepianyi));
	ListView_GetItemText(pNMHDR->hwndFrom, hang, 4, filesize, _countof(filesize));
	swscanf_s(filepianyi, TEXT("%x"), &info->filepianyinum);
	swscanf_s(filesize, TEXT("%x"), &info->filesizenum);
	info->dadjubing = dadjubing;
	info->id = id;

	HANDLE xiancheng = CreateThread(NULL, 0, xianchengshowpequduan16zijie,
		(void*)info, 0, NULL);

	return 1;
}
DWORD WINAPI xianchengshowpequduan16zijie(LPVOID lpParameter) {
	struct pequduan16zijie* info = (struct pequduan16zijie*)lpParameter;

	HWND hEdit = GetDlgItem(info->dadjubing, info->id);
	SendMessage(hEdit, WM_SETTEXT, 0, (LPARAM)TEXT(""));//清空原本的内容
	SendMessage(hEdit, EM_SETLIMITTEXT, 0, 0);//设置最大原始字节无限制
	AppendEditText(info->dadjubing, info->id, TEXT("原始文件中字节如下：  \n"));

	//每一行显示16个字节和地址
	char* nowaddress = winpetoolfile::filedata + info->filepianyinum;
	int cishu = info->filesizenum / 16;
	int nowxianshiaddress = info->filepianyinum;
	for (size_t i = 0; i < cishu; i++)
	{
		_sntprintf_s(tmp, _countof(tmp), _T("%d 0x%08X:"), i, nowxianshiaddress);
		for (size_t i = 0; i < 16; i++)
		{
			_sntprintf_s(tmp, _countof(tmp), _T("%s %x"), tmp, (unsigned char)nowaddress[0]);
			nowaddress++;
		}
		nowxianshiaddress += 16;
		_sntprintf_s(tmp, _countof(tmp), _T("%s\n"), tmp);
		AppendEditText(info->dadjubing, info->id, tmp);
	}
	return 1;
}
int showpefanhuibian(HWND dadjubing, int id, int jizhi, char* yuanshizijie, char* fanhuibian)
{
	HWND listjubing = GetDlgItem(dadjubing, id);
	

	LV_ITEM data{ 0 };
	TCHAR cjizhi[20] = { 0 };
	TCHAR cyuanshizijie[50] = { 0 };
	TCHAR cfanhuibian[80] = { 0 };
	MultiByteToWideChar(CP_ACP, 0, fanhuibian, -1, cfanhuibian, _countof(cfanhuibian));

	_sntprintf_s(cjizhi,_countof(cjizhi), _T("0x%x"),jizhi);
	_sntprintf_s(cyuanshizijie, _countof(cyuanshizijie), _T("%hs"), yuanshizijie);

	data.mask = LVIF_TEXT;
	data.iItem = ListView_GetItemCount(listjubing);
	data.iSubItem = 0;
	data.pszText = cjizhi;
	SendMessage(listjubing, LVM_INSERTITEM, 0, (DWORD)&data);


	data.mask = LVIF_TEXT;
	data.iSubItem = 1;
	data.pszText = cyuanshizijie;
	ListView_SetItem(listjubing, &data);

	data.mask = LVIF_TEXT;
	data.iSubItem = 2;
	data.pszText = cfanhuibian;
	ListView_SetItem(listjubing, &data);

	return 1;
}
