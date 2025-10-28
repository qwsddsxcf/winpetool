#include "重定位表修复.h"
#include <iostream>
#include<stdio.h>
#include<windows.h>
#include<malloc.h>
int xiufucdwbiaoinmem(char* memdata, int pianyi)
{
    PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
    PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
    PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

    PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
    PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

    int cdwmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
    int size = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

    if (cdwmemadress == 0 || size == 0)
        return 0;

    int cdwaddress = int(memdata + cdwmemadress);
    PIMAGE_BASE_RELOCATION pecdwbiao = (PIMAGE_BASE_RELOCATION)cdwaddress;
    int cdwendaddress = cdwaddress + size;

    DWORD delta = pianyi;

    while (int(pecdwbiao) < cdwendaddress && pecdwbiao->SizeOfBlock > 0)
    {
        int datanum = (pecdwbiao->SizeOfBlock - 8) / 2;
        WORD* data = (WORD*)((char*)pecdwbiao + 8);

        for (int i = 0; i < datanum; i++)
        {
            if ((data[i] & 0xF000) == 0x3000)
            {
                DWORD* fixaddress = (DWORD*)(memdata + pecdwbiao->VirtualAddress + (data[i] & 0x0FFF));
                *fixaddress += delta;
            }
        }

        pecdwbiao = PIMAGE_BASE_RELOCATION((char*)pecdwbiao + pecdwbiao->SizeOfBlock);
    }
    pekexuanheader->ImageBase = pekexuanheader->ImageBase + pianyi;

	return 1;
}

