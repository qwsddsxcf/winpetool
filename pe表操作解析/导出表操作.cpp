

//#include "framework.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../工具模块/tool.h"

int namegetdaochufuninfile(char* filedata,char* name)
{
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	
	int daochumemaddress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	int daochufileaddress = addressmemtofile(filedata, daochumemaddress);
	/*printf("%x,\n", daochumemaddress);
	printf("%x,\n", daochufileaddress);*/

	PIMAGE_EXPORT_DIRECTORY pedaochubiao = (PIMAGE_EXPORT_DIRECTORY)(filedata+daochufileaddress);
	int base = pedaochubiao->Base;
	int num = pedaochubiao->NumberOfFunctions;
	//printf("%x,\n", base);
	//printf("%x\n", num);
	int* funname = (int*)(filedata+addressmemtofile(filedata, pedaochubiao->AddressOfNames));
	WORD* xuhao = (WORD*)(filedata+addressmemtofile(filedata, pedaochubiao->AddressOfNameOrdinals));
	int* funaddress = (int*)(filedata+addressmemtofile(filedata, pedaochubiao->AddressOfFunctions));


	for (size_t i = 0; i < pedaochubiao->NumberOfNames; i++)
	{
		char* namezhi = (char*)(filedata + addressmemtofile(filedata, funname[i]));
		if (strcmp(name, namezhi) == 0)
		{
			int resfunaddress = (int)(filedata + addressmemtofile(filedata, funaddress[xuhao[i]]));
			//printf("%s\n", namezhi);
			/*printf("%x\n", resfunaddress);
			printf("%x\n", addressmemtofile(filedata, funaddress[xuhao[i]]));
			printf("成功找到该函数！\n");*/
			return resfunaddress;
		}
		
	}
	
	//printf("没有找到该函数！\n");
	return 0;
}

int xuhaogetdaochufuninfile(char* filedata, int cxuhao)
{
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;

	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

	int daochumemaddress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	int daochufileaddress = addressmemtofile(filedata, daochumemaddress);
	//printf("%x,\n", daochumemaddress);
	//printf("%x,\n", daochufileaddress);

	PIMAGE_EXPORT_DIRECTORY pedaochubiao = (PIMAGE_EXPORT_DIRECTORY)(filedata + daochufileaddress);
	int base = pedaochubiao->Base;
	int num = pedaochubiao->NumberOfFunctions;
	//printf("%x,\n", base);
	//printf("%x\n", num);
	int* funname = (int*)(filedata + addressmemtofile(filedata, pedaochubiao->AddressOfNames));
	WORD* xuhao = (WORD*)(filedata + addressmemtofile(filedata, pedaochubiao->AddressOfNameOrdinals));
	int* funaddress = (int*)(filedata + addressmemtofile(filedata, pedaochubiao->AddressOfFunctions));


	if (cxuhao - base < 0 || ((cxuhao - base) > (num - 1)))
	{
		//printf("该序号没有对应函数！\n");
		return 0;
	}

	//printf("%x\n", funaddress[cxuhao - base]);

	int resfunaddress = (int)(filedata + addressmemtofile(filedata, funaddress[cxuhao - base]));

	//printf("%x\n", resfunaddress);
	//printf("成功找到该函数！\n");

	return resfunaddress;
}

int movedaochubiaoinmem(char* memdata,int start)
{
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	int lastrva = (int)pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress+start;
	char* pelastquduan = (char*)(memdata+pequduanheader[pefileheader->NumberOfSections - 1].VirtualAddress+start);
	int chushi = (int)pelastquduan;
	int daochumemaddress = pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	PIMAGE_EXPORT_DIRECTORY factmemaddress = (PIMAGE_EXPORT_DIRECTORY)(memdata + daochumemaddress);
	if (daochumemaddress == 0) {
		MessageBox(NULL, TEXT("该pe无导出表！"), TEXT("info"), NULL);
		return 0;
	}

	int cdwbiaosize= pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	int funsize = factmemaddress->NumberOfFunctions * 4;
	int funnamesize = factmemaddress->NumberOfNames * 4;
	int xuhaosize = factmemaddress->NumberOfNames * 2;
	//printf("1111\n");
	memcpy(pelastquduan, memdata + factmemaddress->AddressOfFunctions, funsize);
	factmemaddress->AddressOfFunctions = lastrva;
	pelastquduan = pelastquduan + funsize;
	//printf("1111\n");
	memcpy(pelastquduan, memdata + factmemaddress->AddressOfNames, funnamesize);
	factmemaddress->AddressOfNames = lastrva+ funsize;
	pelastquduan = pelastquduan + funnamesize;
	//printf("1111\n");
	memcpy(pelastquduan, memdata + factmemaddress->AddressOfNameOrdinals, xuhaosize);
	factmemaddress->AddressOfNameOrdinals = lastrva + funsize+ funnamesize;
	pelastquduan = pelastquduan + xuhaosize;
	//printf("1111\n");
	int* data = (int*)(memdata+factmemaddress->AddressOfNames);

	for (size_t i = 0; i < factmemaddress->NumberOfNames; i++)
	{
		int size = strlen((char*)(memdata+data[i]));
		memcpy(pelastquduan, (char*)(memdata+data[i]), size+1);
		data[i]= (int)(pelastquduan-(int)memdata);
		pelastquduan = pelastquduan + size+1;
	}

	memcpy(pelastquduan, (char*)factmemaddress, cdwbiaosize);
	pekexuanheader->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress = lastrva;

	/*printf("成功将导出表表移动到最后一个区段起始！\n");*/
	MessageBox(NULL, TEXT("成功移动导出表到最后一个区段！"), TEXT("成功"), NULL);

	return int(pelastquduan)+ cdwbiaosize-chushi+5;
}
