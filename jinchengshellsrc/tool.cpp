#include"tool.h"
#pragma warning(disable : 4996)
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

	return 1;
}

void zhurucode() {
	MessageBoxA(NULL, "调用成功", "成功", NULL);
}

void runallcode() {
	MessageBoxA(NULL, "任意代码执行", "成功", NULL);
}

int WINAPI myMessageBoxA(
	_In_opt_ HWND hWnd,
	_In_opt_ LPCSTR lpText,
	_In_opt_ LPCSTR lpCaption,
	_In_ UINT uType) {

	MessageBoxA(NULL, "iat注入成功", "成功", NULL);
	FILE* infofile = fopen("C:/info.txt", "a+");
	if (infofile == NULL) {
		MessageBoxA(NULL, "打开文件失败！", "失败", NULL);
		return 0;
	}

	TCHAR tmp[200]{ 0 };
	wsprintf(tmp, TEXT("hWnd: %d,lpText: %s,lpCaption: %s,uType: %d \n"), hWnd, lpText, lpCaption, uType);

	char narrowTmp[400] = { 0 }; 
	WideCharToMultiByte(CP_UTF8, 0, tmp, -1, narrowTmp, sizeof(narrowTmp), NULL, NULL);


	fwrite(narrowTmp, 1, strlen(narrowTmp), infofile);

	MessageBoxA(NULL, "参数保存在：C:/info.txt", "成功", NULL);
	fclose(infofile);
	return 1;
}