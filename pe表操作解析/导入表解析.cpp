
//#include "framework.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../资源模块/resource.h"
#include "../工具模块/tool.h"
#include <cstdint>
void jiexidaorubiaoinfile(char* filedata,HWND dadjubing)
{


	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

	int drmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	PIMAGE_IMPORT_DESCRIPTOR factdraddress = (PIMAGE_IMPORT_DESCRIPTOR)(filedata + addressmemtofile(filedata, drmemadress));

	TCHAR tmp[60] = _T("");
	


	for (; factdraddress->Name; factdraddress++)
	{
		char* dllname = (filedata + addressmemtofile(filedata, factdraddress->Name));
		_sntprintf_s(tmp, _countof(tmp), _T("%hs"), "===========================\n");
		AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
		_sntprintf_s(tmp, _countof(tmp), _T("dllname:%hs,\n"), dllname);
		AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
		
		//MessageBox(NULL, TEXT("1"), TEXT("1"), NULL);
		PIMAGE_THUNK_DATA INTbiao = (PIMAGE_THUNK_DATA)(filedata + addressmemtofile(filedata, factdraddress->OriginalFirstThunk));
		for (;INTbiao->u1.AddressOfData; INTbiao++)
		{
			int addr = INTbiao->u1.AddressOfData;
			if (addr & IMAGE_ORDINAL_FLAG32)
			{
				_sntprintf_s(tmp, _countof(tmp), _T("该导入项为序号导入，序号：0x%08x\n"), (uint16_t)addr);
				AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);

			}
			else {
			    PIMAGE_IMPORT_BY_NAME namebiao=(PIMAGE_IMPORT_BY_NAME)(filedata + addressmemtofile(filedata, INTbiao->u1.AddressOfData));
				_sntprintf_s(tmp, _countof(tmp), _T("该导入项为函数名字导入，名字：%hs\n"), namebiao->Name);
				AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
			}

		}

	}

	MessageBox(NULL, TEXT("成功解析导入表！"), TEXT("成功"), NULL);
}
