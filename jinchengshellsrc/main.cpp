#include<stdio.h>
#include<windows.h>
#include <psapi.h>
#include"tool.h"
#pragma warning(disable : 4996)
//DWORD baseAddr = 0;

int changeiatfun(int oldaddress,int newaddress) {

	
	HMODULE hModule = GetModuleHandleA(NULL);  

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);

	PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)(
		(BYTE*)hModule + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
		);

	for (; importDesc->Name != 0; importDesc++) {
		
		PIMAGE_THUNK_DATA intThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);  // INT������/��ű�
		PIMAGE_THUNK_DATA iatThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);  // IAT��������ַ��

		for (; intThunk->u1.AddressOfData != 0; intThunk++, iatThunk++) {
			if (IMAGE_SNAP_BY_ORDINAL(intThunk->u1.Ordinal)) {
			

				if (iatThunk->u1.Function == oldaddress) {
					DWORD oldProtect;
					VirtualProtect(&iatThunk->u1.Function, sizeof(ULONG_PTR), PAGE_READWRITE, &oldProtect);
					iatThunk->u1.Function = newaddress;
				
					return 1;
				}
			}
			else {

				if (iatThunk->u1.Function ==oldaddress) {
					DWORD oldProtect;
					VirtualProtect(&iatThunk->u1.Function, sizeof(ULONG_PTR), PAGE_READWRITE, &oldProtect);
					iatThunk->u1.Function = newaddress;
		
					return 1;
				}

			}
		}
	}
	
	return 0;
}
int huanyuaniatfun(int oldaddress, int newaddress) {
	

	HMODULE hModule = GetModuleHandleA(NULL);

	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);


	PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)(
		(BYTE*)hModule + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
		);

	for (; importDesc->Name != 0; importDesc++) {
		PIMAGE_THUNK_DATA intThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);  // INT������/��ű�
		PIMAGE_THUNK_DATA iatThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);  // IAT��������ַ��

		for (; intThunk->u1.AddressOfData != 0; intThunk++, iatThunk++) {
			if (IMAGE_SNAP_BY_ORDINAL(intThunk->u1.Ordinal)) {
				if (iatThunk->u1.Function == newaddress) {
					DWORD oldProtect;
					VirtualProtect(&iatThunk->u1.Function, sizeof(ULONG_PTR), PAGE_READWRITE, &oldProtect);
					iatThunk->u1.Function = oldaddress;
					return 1;
				}
			}
			else {
				if (iatThunk->u1.Function == newaddress) {
					DWORD oldProtect;
					VirtualProtect(&iatThunk->u1.Function, sizeof(ULONG_PTR), PAGE_READWRITE, &oldProtect);
					iatThunk->u1.Function = oldaddress;
					return 1;
				}

			}
		}
	}

	return 0;
}



DWORD WINAPI FixIAT_InRemoteProcess(LPVOID lpParam) {
	// ����1����ȡ����ģ�����ַ��PEͷ
	int hModule =(int) lpParam;
	//HMODULE hModule = GetModuleHandleA(NULL);  // ��ǰģ�飨ע�뵽Ŀ����̵���������ַ
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);

	// ����2����λ�����IMAGE_IMPORT_DESCRIPTOR���飩
	PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)(
		(BYTE*)hModule + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
		);

	// ����3��������������DLL�ĵ������
	for (; importDesc->Name != 0; importDesc++) {
		// 3.1 ��������DLL��ֱ�ӵ���Ŀ����̵�LoadLibraryA��
		LPCSTR dllName = (LPCSTR)((BYTE*)hModule + importDesc->Name);  // DLL����������ģ���еĵ�ַ
		HMODULE hDll = LoadLibraryA(dllName);  // Ŀ������м���DLL

		// 3.2 ����IAT���޸�������ַ��INT��IAT���볡������ͬʱ����OriginalFirstThunk��FirstThunk��
		PIMAGE_THUNK_DATA intThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);  // INT������/��ű�
		PIMAGE_THUNK_DATA iatThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);  // IAT��������ַ��

		for (; intThunk->u1.AddressOfData != 0; intThunk++, iatThunk++) {
			if (IMAGE_SNAP_BY_ORDINAL(intThunk->u1.Ordinal)) {
				// ����ŵ��룺����GetProcAddress����Ÿ�λΪ0x8000������룩
				DWORD ordinal = IMAGE_ORDINAL(intThunk->u1.Ordinal);
				iatThunk->u1.Function = (ULONG_PTR)GetProcAddress(hDll, (LPCSTR)ordinal);
			}
			else {
				// �����Ƶ��룺����IMAGE_IMPORT_BY_NAME��ȡ������
				PIMAGE_IMPORT_BY_NAME importName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)hModule + intThunk->u1.AddressOfData);
				iatThunk->u1.Function = (ULONG_PTR)GetProcAddress(hDll, importName->Name);  // �޸�IAT��ַ
			}
		}
	}


	
	HANDLE hMap = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, L"weizhihack");
	char* message = (char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 1024);

	while (true) {
		if (strlen(message) > 0) {
			char tmp[30]{ 0 };
			if (strcmp(message, "startiathook") == 0)
			{
				if (changeiatfun((int)MessageBoxW, (int)myMessageBoxA)) {
					MessageBoxA(NULL, "iathook�ɹ�", "�ɹ�", NULL);
				}
			}
			else if (strcmp(message, "stopiathook") == 0) {
				if (huanyuaniatfun((int)MessageBoxW, (int)myMessageBoxA)) {
					MessageBoxA(NULL, "ֹͣiathook�ɹ�", "�ɹ�", NULL);
				}
			}
			else if (strcmp(message, "diaoyong") == 0) {
				zhurucode();
			}
			else if (strcmp(message, "runcode") == 0) {
				runallcode();
			}
			else if (strcmp(message, "exit") == 0) {
				CloseHandle(hMap);
				break;
			}
			message[0] = '\0';
		}
		Sleep(1000); 
	}

	return 1;
}



int main(int num, char* canshu[]) {

	
	int pid = 0;
	sscanf(canshu[1], "%d", &pid);
	if (pid == 0) {
		return 0;
	}
	

	HMODULE selfexe = GetModuleHandleA(NULL);


	PIMAGE_DOS_HEADER selfdosHeader = (PIMAGE_DOS_HEADER)selfexe;
	PIMAGE_NT_HEADERS selffileheader = (PIMAGE_NT_HEADERS)((BYTE*)selfexe + selfdosHeader->e_lfanew);
	DWORD imageSize = selffileheader->OptionalHeader.SizeOfImage;  // ����EXE���ܴ�С

	char* tmpselfdata =(char*)malloc(imageSize);
	memcpy(tmpselfdata, selfexe, imageSize);

	HANDLE shellhProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);  // ��Ŀ����̣������ԱȨ�ޣ�
	HMODULE hMod;
	DWORD cbNeeded;


	PVOID shelladdress = VirtualAllocEx(shellhProcess, NULL, imageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

	int pianyi = (int)shelladdress - (int)selfexe;
	xiufucdwbiaoinmem(tmpselfdata, pianyi);

	WriteProcessMemory(shellhProcess, shelladdress, tmpselfdata, imageSize, NULL);

	int funadddress = (int)shelladdress+(int)FixIAT_InRemoteProcess- (int)selfexe;


	HANDLE hMap = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 1024, L"weizhihack");
	char* message = (char*)MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 1024);

	HANDLE hThread = CreateRemoteThread(
		shellhProcess,          // Ŀ����̾��
		NULL,              // Ĭ�ϰ�ȫ����
		0,                 // Ĭ��ջ��С
		(LPTHREAD_START_ROUTINE)funadddress,  // �߳���ں�����������ԭ�ͣ�
		shelladdress,              // ���ݸ��̵߳Ĳ������˴�δʹ�ã�
		0,                 // ����ִ��
		NULL               // �����߳�ID
	);

	
}