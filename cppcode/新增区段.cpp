
//#include "framework.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "resource.h"
#include "tool.h"
#include "新增区段.h"
#pragma warning(disable : 4996)
char* addquduaninmemory(char* memdata, int newquduansize)
{
	int quduansize = 40;
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	int pequduannum = pefileheader->NumberOfSections;
	int peheaderend = pekexuanheader->SizeOfHeaders;
	int quduanend = int((char*)pequduanheader+ quduansize * pequduannum-(int)memdata);
	int freesize = peheaderend - quduanend;


	if (freesize < 80)
	{
		MessageBox(NULL, TEXT("多余空间不够添加一个区段"), TEXT("info"), NULL);
	;
		return NULL;
	}

	int newquduanmemsize = newquduansize;
	int newquduanfilesize = newquduansize+0x500;
	int size1 = pequduanheader[pequduannum - 1].VirtualAddress + pequduanheader[pequduannum - 1].Misc.VirtualSize;
	int size2 = pequduanheader[pequduannum - 1].PointerToRawData + pequduanheader[pequduannum - 1].SizeOfRawData;
	int newquduanmemaddress = duiqi(size1,pekexuanheader->SectionAlignment);
	int newquduanfileaddress= duiqi(size2, pekexuanheader->FileAlignment);

	
	int newsize = newquduanmemaddress + newquduansize+ 0x1000;

	char* newmemdata = (char*)malloc(newsize);
	int oldsize = pekexuanheader->SizeOfImage;
	memset(newmemdata,0,newsize);
	memcpy(newmemdata,memdata, oldsize);

	PIMAGE_DOS_HEADER newpedosheader = (PIMAGE_DOS_HEADER)newmemdata;
	PIMAGE_NT_HEADERS32 newpepeheader = (PIMAGE_NT_HEADERS32)(newmemdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER newpefileheader = &newpepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 newpekexuanheader = &newpepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER newpequduanheader = PIMAGE_SECTION_HEADER((char*)newpekexuanheader + newpefileheader->SizeOfOptionalHeader);

	int size3 = newquduanmemaddress + newquduanmemsize;
	newpekexuanheader->SizeOfImage = duiqi(size3, pekexuanheader->SectionAlignment);

	//printf("%x\n", newpekexuanheader->SizeOfImage);
	newpefileheader->NumberOfSections = newpefileheader->NumberOfSections + 1;
	newpequduanheader[pequduannum].Characteristics = 0x60000020;
	newpequduanheader[pequduannum].Misc.PhysicalAddress = newpequduanheader[pequduannum - 1].Misc.PhysicalAddress;
	newpequduanheader[pequduannum].Misc.VirtualSize = newquduanmemsize;
	
	memcpy(newpequduanheader[pequduannum].Name, ".newsec", 8);
	newpequduanheader[pequduannum].NumberOfLinenumbers = newpequduanheader[pequduannum - 1].NumberOfLinenumbers;
	newpequduanheader[pequduannum].NumberOfRelocations = newpequduanheader[pequduannum - 1].NumberOfRelocations;
	newpequduanheader[pequduannum].PointerToLinenumbers = newpequduanheader[pequduannum - 1].PointerToLinenumbers;
	newpequduanheader[pequduannum].PointerToRawData = newquduanfileaddress;
	newpequduanheader[pequduannum].PointerToRelocations = newpequduanheader[pequduannum - 1].PointerToRelocations;
	newpequduanheader[pequduannum].SizeOfRawData = newquduanfilesize;
	newpequduanheader[pequduannum].VirtualAddress = newquduanmemaddress;

	//printf("新增区段成功！\n");
	MessageBox(NULL, TEXT("新增区段成功！"), TEXT("成功"), NULL);

	return newmemdata;
}

int duiqi(int size, int duiqi)
{
	int newsize;
	if (size % duiqi == 0)
	{
		newsize = size;
	}
	else {
		newsize = duiqi*(size / duiqi+1);
	}

	return newsize;
}
