
#include "framework.h"
int movcdwbiaoinmem(char* memdata,int start)
{

	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	//IMAGE_DIRECTORY_ENTRY_BASERELOC
	int cdwmemadress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	int size = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size;

	int lastquduan = int(memdata + pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress)+start;
	int chushi = lastquduan;
	int lastrva = pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress+start;

	int cdwaddress = int(memdata + cdwmemadress);
	PIMAGE_BASE_RELOCATION pecdwbiao = (PIMAGE_BASE_RELOCATION)cdwaddress;
	int cdwendaddress = cdwaddress + size;

	while (int(pecdwbiao) < cdwendaddress && pecdwbiao->SizeOfBlock > 0)
	{
	
		memcpy((char*)lastquduan, (char*)pecdwbiao, pecdwbiao->SizeOfBlock);

		lastquduan = lastquduan + pecdwbiao->SizeOfBlock;
		pecdwbiao = PIMAGE_BASE_RELOCATION((char*)pecdwbiao + pecdwbiao->SizeOfBlock);
	}
	memcpy((char*)lastquduan, (char*)pecdwbiao, 8);
	pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress = lastrva;

	/*printf("�ض�λ���ƶ��ɹ���\n");*/
	MessageBox(NULL, TEXT("�ɹ��ƶ��ض�λ�����һ�����Σ�"), TEXT("�ɹ�"), NULL);

	return lastquduan + 8 - chushi+5;

}
