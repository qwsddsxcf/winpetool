//#include "framework.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "tool.h"
#pragma warning(disable : 4996)
int daorubiaoattackinmem(char* memdata,int start,char* dllname,char* dllfun)
{
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	int drmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	PIMAGE_IMPORT_DESCRIPTOR factdraddress = (PIMAGE_IMPORT_DESCRIPTOR)(memdata+drmemadress);

	int lastquduan = int(memdata + pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress+start);
	int lastrva = pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress+ start;
	int chushi = lastquduan;

	const int size = sizeof(*factdraddress);
	for (; factdraddress->Name; factdraddress++)
	{
		memcpy((char*)lastquduan, (char*)factdraddress,size );
		lastquduan = lastquduan + size;
	}
	
	pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = lastrva;

	MessageBox(NULL, TEXT("成功移动导入表到最后一个区段，函数等数据未移动"), TEXT("info"), NULL);
	

	memset((char*)lastquduan, 0, size * 2);
	PIMAGE_IMPORT_DESCRIPTOR newbiao = (PIMAGE_IMPORT_DESCRIPTOR)lastquduan;
	lastquduan = lastquduan + (size * 2);

	int dllnamerva = lastquduan - (int)memdata;
	strcpy((char*)lastquduan, dllname);
	lastquduan = lastquduan + strlen(dllname)+1;

	int funnamerva= lastquduan-(int)memdata;
	*(WORD*)lastquduan = 0; // Hint = 0
	lastquduan += sizeof(WORD);
	strcpy((char*)lastquduan, dllfun);
	lastquduan = lastquduan + strlen(dllfun) + 1;

	PIMAGE_THUNK_DATA  intbiaoaddress = (PIMAGE_THUNK_DATA)lastquduan;
	intbiaoaddress->u1.AddressOfData = funnamerva;
	lastquduan = lastquduan + sizeof(IMAGE_THUNK_DATA);
	memset((char*)lastquduan, 0, sizeof(IMAGE_THUNK_DATA));
	lastquduan += sizeof(IMAGE_THUNK_DATA);

	PIMAGE_THUNK_DATA iatbiaoaddress = (PIMAGE_THUNK_DATA)lastquduan;
	iatbiaoaddress->u1.AddressOfData = funnamerva;
	lastquduan = lastquduan + sizeof(IMAGE_THUNK_DATA);
	memset((char*)lastquduan, 0, sizeof(IMAGE_THUNK_DATA));
	lastquduan += sizeof(IMAGE_THUNK_DATA);


	newbiao->OriginalFirstThunk = (int)intbiaoaddress - (int)memdata;
	newbiao->FirstThunk = (int)iatbiaoaddress - (int)memdata;
	newbiao->TimeDateStamp = 0;
	newbiao->ForwarderChain = 0;
	newbiao->Name = dllnamerva;

	pequduanheader[pefileheader->NumberOfSections - 1].Characteristics = 0xC0000040;

	TCHAR tmp[50] = _T("");
	_sntprintf_s(tmp, _countof(tmp), _T("成功注入新的dll:%hs !"),dllname);
	MessageBox(NULL, tmp, TEXT("成功"), NULL);
	
	return lastquduan - chushi + 5;
}
int daorubiaomoveinmem(char* memdata, int start) {

	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	int drmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	PIMAGE_IMPORT_DESCRIPTOR factdraddress = (PIMAGE_IMPORT_DESCRIPTOR)(memdata + drmemadress);

	int lastquduan = int(memdata + pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress + start);
	int lastrva = pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress + start;
	int chushi = lastquduan;

	const int size = sizeof(*factdraddress);
	for (; factdraddress->Name; factdraddress++)
	{
		memcpy((char*)lastquduan, (char*)factdraddress, size);
		lastquduan = lastquduan + size;
	}

	pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = lastrva;

	MessageBox(NULL, TEXT("成功移动导入表到最后一个区段，函数等数据未移动"), TEXT("info"), NULL);

	return lastquduan - chushi + 5;

}