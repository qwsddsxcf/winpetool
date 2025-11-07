
//#include "framework.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../资源模块/resource.h"
#include "../工具模块/tool.h"

void filechongdingweibiao(char* filedata, HWND dadjubing)
{

	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

	int cdwmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	int size = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

	int cdwfileaddress = int(filedata+addressmemtofile(filedata, cdwmemadress));
	PIMAGE_BASE_RELOCATION pecdwbiao = (PIMAGE_BASE_RELOCATION)cdwfileaddress;
	int cdwendaddress = cdwfileaddress + size;

	TCHAR tmp[60] = _T("");

	while (int(pecdwbiao) < cdwendaddress && pecdwbiao->SizeOfBlock > 0)
	{
		int pagememaddress = pecdwbiao->VirtualAddress;
		int pegefileaddress = int(filedata + addressmemtofile(filedata, pagememaddress));
	
		int datanum = (pecdwbiao->SizeOfBlock - 8) / 2;
		WORD* data = (WORD*)(pecdwbiao + 1);

		_sntprintf_s(tmp, _countof(tmp), _T("%hs"), "===========================\n");
		AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
		_sntprintf_s(tmp, _countof(tmp), _T("pagememaddress:0x%08x\n"), pagememaddress);
		AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
		_sntprintf_s(tmp, _countof(tmp), _T("pegefileaddress:0x%08x\n"), pegefileaddress);
		AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
		_sntprintf_s(tmp, _countof(tmp), _T("datanum:%d\n"), datanum);
		AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);


		for (size_t i = 0; i < datanum; i++)
		{
			WORD shuju = data[i];
			if (shuju >> 12 == 3)
			{

				int offset = shuju & 0x0FFF;
				_sntprintf_s(tmp, _countof(tmp), _T("该数据需要修改, 内存偏移： 0x%08x\n"), pagememaddress+offset);
				AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
			}
			else {
		
				int offset = shuju & 0x0FFF;
				_sntprintf_s(tmp, _countof(tmp), _T("该数据不需要修改, 内存偏移： 0x%08x\n"), pagememaddress + offset);
				AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
			}
		}

		pecdwbiao = PIMAGE_BASE_RELOCATION((char*)pecdwbiao + pecdwbiao->SizeOfBlock);
	}


	MessageBox(NULL, TEXT("成功解析重定位表"), TEXT("成功"), NULL);
	
}
