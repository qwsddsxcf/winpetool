
#include "framework.h"
#pragma warning(disable : 4996)
////75e11600
//rukou:00021000
void memoryquduanattcak(char* memdata,int quduan)
{
	int messagehanshu = (int)MessageBoxA;
	char shellcode[] = {
		0x6A,0x00,
		0x6A,0x00,
		0x6A,0x00,
		0x6A,0x00,
		0xE8,0x77,
		0xF7,0xA0,
		0x75,0xE9,
		0x70,0xF4,
		0xFF,0xFF
	};
	int e8pianyi = 9;
	int e9pianyi = 14;
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	int entry = pekexuanheader->AddressOfEntryPoint;
	int imageqishi = pekexuanheader->ImageBase;
	int textquduanqishi = pequduanheader[quduan].VirtualAddress;
	int textquduansize = pequduanheader[quduan].Misc.VirtualSize;
	int daimaqishi = textquduanqishi + textquduansize+imageqishi;
	//5500

	int shijie8 = messagehanshu - (daimaqishi + 13);
	int shellsize = sizeof(shellcode);

	int shijie9 = (entry + imageqishi) - (daimaqishi + shellsize);

	for (size_t i = 0; i < 4; i++)
	{
		shellcode[e8pianyi+i] = (shijie8>>(8*i)) & 0xFF;
		shellcode[e9pianyi + i] = (shijie9 >> (8 * i)) & 0xFF;
	}

	char* shellstart = memdata + textquduanqishi + textquduansize;



	if (quduan + 1 < pefileheader->NumberOfSections) {
		if (shellsize > pequduanheader[quduan + 1].VirtualAddress - textquduanqishi - textquduansize
			|| shellsize + textquduansize > pequduanheader[quduan].SizeOfRawData)
		{
			MessageBox(NULL, TEXT("�ô�������̫С�޷���Ӵ��룡"), TEXT("info"), NULL);
			return;
		}
	}
	else if(quduan + 1 == pefileheader->NumberOfSections)
	{
		if (shellsize + textquduansize > pequduanheader[quduan].SizeOfRawData)
		{
			MessageBox(NULL, TEXT("�ô�������̫С�޷���Ӵ��룡"), TEXT("info"), NULL);
			return;
		}
	}
	else {
		MessageBox(NULL, TEXT("�������ô��󣬳������η�Χ��"), TEXT("info"), NULL);
		return;
	}
	
	memcpy(shellstart, shellcode, shellsize);

	pekexuanheader->AddressOfEntryPoint = textquduanqishi + textquduansize;
	pequduanheader[quduan].Characteristics = 0x60000020;

	MessageBox(NULL, TEXT("�ѳɹ�ע����룡"), TEXT("�ɹ�"), NULL);

}
