
//#include "framework.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../资源模块/resource.h"
#include "../工具模块/tool.h"
void ziyuanbiaojiexiinfile(char* filedata, HWND dadjubing,int id)
{
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

	int ziyuanmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress;
	int size = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size;

	TCHAR tmp[60] = _T("");

	PIMAGE_RESOURCE_DIRECTORY ziyuanfileaddress = (PIMAGE_RESOURCE_DIRECTORY)(filedata + addressmemtofile(filedata, ziyuanmemadress));
	PIMAGE_RESOURCE_DIRECTORY_ENTRY oneziyuan = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(ziyuanfileaddress + 1);
	for (size_t i = 0; i < ziyuanfileaddress->NumberOfIdEntries+ziyuanfileaddress->NumberOfNamedEntries; i++)
	{
		int mytypeid = oneziyuan[i].Name;
		/*if (mytypeid == id) {*/

			_sntprintf_s(tmp, _countof(tmp), _T("%hs"), "===========================\n");
			AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
			_sntprintf_s(tmp, _countof(tmp), _T("找到要查找的资源：id:%d\n"), mytypeid);
			AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
		

			PIMAGE_RESOURCE_DIRECTORY twoziyuanheader=(PIMAGE_RESOURCE_DIRECTORY)(filedata + addressmemtofile(filedata, ziyuanmemadress+ oneziyuan[i].OffsetToDirectory));
			PIMAGE_RESOURCE_DIRECTORY_ENTRY twoziyuan = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(twoziyuanheader + 1);

			for (size_t j = 0; j < twoziyuanheader->NumberOfIdEntries + twoziyuanheader->NumberOfNamedEntries; j++)
			{
				if (twoziyuan[j].NameIsString == 1) {
					PIMAGE_RESOURCE_DIR_STRING_U namefileaddress=(PIMAGE_RESOURCE_DIR_STRING_U)(filedata + addressmemtofile(filedata, ziyuanmemadress + twoziyuan[j].NameOffset));
					_sntprintf_s(tmp, _countof(tmp), _T("资源名：%s\n"), namefileaddress->Length, namefileaddress->NameString);
					AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
				}
				else {
					//0x%08X
					_sntprintf_s(tmp, _countof(tmp), _T("资源id:0x%08x\n"), twoziyuan[j].Name);
					AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
				}

				PIMAGE_RESOURCE_DIRECTORY threeziyuanheader = (PIMAGE_RESOURCE_DIRECTORY)(filedata + addressmemtofile(filedata, ziyuanmemadress + twoziyuan[j].OffsetToDirectory));
				PIMAGE_RESOURCE_DIRECTORY_ENTRY threeziyuan = (PIMAGE_RESOURCE_DIRECTORY_ENTRY)(threeziyuanheader + 1);

				for (size_t k = 0; k < threeziyuanheader->NumberOfIdEntries + threeziyuanheader->NumberOfNamedEntries; k++)
				{
					int langid = threeziyuan[k].Id;
					_sntprintf_s(tmp, _countof(tmp), _T("语言id:%x\n"), langid);
					AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
			
					PIMAGE_RESOURCE_DATA_ENTRY fourdata=(PIMAGE_RESOURCE_DATA_ENTRY)(filedata + addressmemtofile(filedata, ziyuanmemadress + threeziyuan[k].OffsetToDirectory));
					_sntprintf_s(tmp, _countof(tmp), _T("资源rva: 0x%08x"), fourdata->OffsetToData);
					AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
					_sntprintf_s(tmp, _countof(tmp), _T(", 资源size: 0x%08x"), fourdata->Size);
					AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);
					_sntprintf_s(tmp, _countof(tmp), _T(",资源codepage:0x%08x ,\n"), fourdata->CodePage);
					AppendEditText(dadjubing, IDC_EDIT2_biaoinfo, tmp);

				}

			}
	}

	MessageBox(NULL, TEXT("解析资源表成功"), TEXT("成功"), NULL);

}
