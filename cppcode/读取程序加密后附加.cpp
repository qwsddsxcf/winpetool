#include "��ȡ������ܺ󸽼�.h"
#include "��������.h"
#include"tool.h"
#include<stdio.h>
#include<windows.h>
#include<malloc.h>

char* readsrctoshell(char* srcmemdata, char* shellmemdata)
{
	PIMAGE_DOS_HEADER srcpedosheader = (PIMAGE_DOS_HEADER)srcmemdata;
	PIMAGE_NT_HEADERS32 srcpepeheader = (PIMAGE_NT_HEADERS32)(srcmemdata + srcpedosheader->e_lfanew);

	PIMAGE_OPTIONAL_HEADER32 srcpekexuanheader = &srcpepeheader->OptionalHeader;
	char* memorydata = (char*)malloc(sizeof(char) * srcpekexuanheader->SizeOfImage);
	memset(memorydata, 0, srcpekexuanheader->SizeOfImage);
	memcpy(memorydata, srcmemdata, srcpekexuanheader->SizeOfImage);

	for (size_t i = 0; i < srcpekexuanheader->SizeOfImage; i++)
	{
		memorydata[i] = memorydata[i] ^ 'h';
	}

	char* newshellmemdata = addquduaninmemory(shellmemdata, srcpekexuanheader->SizeOfImage);


	PIMAGE_DOS_HEADER newshellpedosheader = (PIMAGE_DOS_HEADER)newshellmemdata;
	PIMAGE_NT_HEADERS32 newshellpepeheader = (PIMAGE_NT_HEADERS32)(newshellmemdata + newshellpedosheader->e_lfanew);
	PIMAGE_FILE_HEADER newshellpefileheader = &newshellpepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 newshellpekexuanheader = &newshellpepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER newshellpequduanheader = PIMAGE_SECTION_HEADER((char*)newshellpekexuanheader + newshellpefileheader->SizeOfOptionalHeader);

	char* shelllastquduanmemaddress = newshellmemdata + newshellpequduanheader[newshellpefileheader->NumberOfSections - 1].VirtualAddress;
	memcpy(shelllastquduanmemaddress, memorydata, srcpekexuanheader->SizeOfImage);


	//printf("��������ܺ󸽼ӵ���һ����ɹ���\n");
	MessageBox(NULL, TEXT("��������ܺ󸽼ӵ��ǳ���ɹ�!"), TEXT("info"), NULL);
	return newshellmemdata;
}
