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
		
		PIMAGE_THUNK_DATA intThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);  // INT（名称/序号表）
		PIMAGE_THUNK_DATA iatThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);  // IAT（函数地址表）

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
		PIMAGE_THUNK_DATA intThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);  // INT（名称/序号表）
		PIMAGE_THUNK_DATA iatThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);  // IAT（函数地址表）

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
	// 步骤1：获取自身模块基地址和PE头
	int hModule =(int) lpParam;
	//HMODULE hModule = GetModuleHandleA(NULL);  // 当前模块（注入到目标进程的自身）基地址
	PIMAGE_DOS_HEADER dosHeader = (PIMAGE_DOS_HEADER)hModule;
	PIMAGE_NT_HEADERS ntHeader = (PIMAGE_NT_HEADERS)((BYTE*)hModule + dosHeader->e_lfanew);

	// 步骤2：定位导入表（IMAGE_IMPORT_DESCRIPTOR数组）
	PIMAGE_IMPORT_DESCRIPTOR importDesc = (PIMAGE_IMPORT_DESCRIPTOR)(
		(BYTE*)hModule + ntHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress
		);

	// 步骤3：遍历所有依赖DLL的导入表项
	for (; importDesc->Name != 0; importDesc++) {
		// 3.1 加载依赖DLL（直接调用目标进程的LoadLibraryA）
		LPCSTR dllName = (LPCSTR)((BYTE*)hModule + importDesc->Name);  // DLL名称在自身模块中的地址
		HMODULE hDll = LoadLibraryA(dllName);  // 目标进程中加载DLL

		// 3.2 遍历IAT表，修复函数地址（INT和IAT分离场景，需同时处理OriginalFirstThunk和FirstThunk）
		PIMAGE_THUNK_DATA intThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->OriginalFirstThunk);  // INT（名称/序号表）
		PIMAGE_THUNK_DATA iatThunk = (PIMAGE_THUNK_DATA)((BYTE*)hModule + importDesc->FirstThunk);  // IAT（函数地址表）

		for (; intThunk->u1.AddressOfData != 0; intThunk++, iatThunk++) {
			if (IMAGE_SNAP_BY_ORDINAL(intThunk->u1.Ordinal)) {
				// 按序号导入：调用GetProcAddress（序号高位为0x8000，需剥离）
				DWORD ordinal = IMAGE_ORDINAL(intThunk->u1.Ordinal);
				iatThunk->u1.Function = (ULONG_PTR)GetProcAddress(hDll, (LPCSTR)ordinal);
			}
			else {
				// 按名称导入：解析IMAGE_IMPORT_BY_NAME获取函数名
				PIMAGE_IMPORT_BY_NAME importName = (PIMAGE_IMPORT_BY_NAME)((BYTE*)hModule + intThunk->u1.AddressOfData);
				iatThunk->u1.Function = (ULONG_PTR)GetProcAddress(hDll, importName->Name);  // 修复IAT地址
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
					MessageBoxA(NULL, "iathook成功", "成功", NULL);
				}
			}
			else if (strcmp(message, "stopiathook") == 0) {
				if (huanyuaniatfun((int)MessageBoxW, (int)myMessageBoxA)) {
					MessageBoxA(NULL, "停止iathook成功", "成功", NULL);
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
	DWORD imageSize = selffileheader->OptionalHeader.SizeOfImage;  // 自身EXE的总大小

	char* tmpselfdata =(char*)malloc(imageSize);
	memcpy(tmpselfdata, selfexe, imageSize);

	HANDLE shellhProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);  // 打开目标进程（需管理员权限）
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
		shellhProcess,          // 目标进程句柄
		NULL,              // 默认安全属性
		0,                 // 默认栈大小
		(LPTHREAD_START_ROUTINE)funadddress,  // 线程入口函数（已适配原型）
		shelladdress,              // 传递给线程的参数（此处未使用）
		0,                 // 立即执行
		NULL               // 忽略线程ID
	);

	
}