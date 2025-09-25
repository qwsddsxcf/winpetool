#include "pe基础信息展示.h"
TCHAR tmp[MAX_PATH] = _T("");
extern TCHAR filepath[MAX_PATH];

int showpejichuinfo(char* filedata, HWND dadjubing)
{
	
	if (filedata == NULL) {
		MessageBox(NULL, TEXT("文件内容为空"), TEXT("error"), NULL);
		return 0;
	}
	_sntprintf_s(tmp, _countof(tmp), _T("pe基础信息，文件路径:%s"), filepath);
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
	_sntprintf_s(tmp, _countof(tmp), _T("pe区段信息，文件路径:%s"), filepath);
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
	_sntprintf_s(tmp, _countof(tmp), _T("pe目录信息，文件路径:%s"), filepath);
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
	_sntprintf_s(tmp, _countof(tmp), _T("导入表信息，文件路径:%s"), filepath);
	SetWindowText(dadjubing, tmp);
	daochubiaojiexiinfile(filedata, dadjubing);
	return 1;
}

int showpedaorubiaoinfo(char* filedata, HWND dadjubing)
{
	_sntprintf_s(tmp, _countof(tmp), _T("导入表信息，文件路径:%s"), filepath);
	SetWindowText(dadjubing, tmp);
	jiexidaorubiaoinfile(filedata, dadjubing);
	return 1;
}

int showpecdwbiaoinfo(char* filedata, HWND dadjubing)
{
	_sntprintf_s(tmp, _countof(tmp), _T("重定位表信息，文件路径:%s"), filepath);
	SetWindowText(dadjubing, tmp);
	filechongdingweibiao(filedata, dadjubing);
	return 1;
}

int showpeziyuanbiaoinfo(char* filedata, HWND dadjubing,int id)
{
	_sntprintf_s(tmp, _countof(tmp), _T("资源表信息，文件路径:%s"), filepath);
	SetWindowText(dadjubing, tmp);
	ziyuanbiaojiexiinfile(filedata, dadjubing, id);
	return 1;
}

int showpeiatbiaoinfo(char* filedata, HWND dadjubing)
{
	return 0;
}
