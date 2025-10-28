
#include <iostream>
#include<stdio.h>
#include<windows.h>
#include<malloc.h>
#include "文件操作.h"
#include "重定位表修复.h"
extern int size;

typedef NTSTATUS(NTAPI* pZwUnmapViewOfSection)(
	_In_    HANDLE ProcessHandle,
	_In_opt_ PVOID  BaseAddress
	);

int main()
{
    TCHAR shellfilepath[MAX_PATH] = { 0 };
  
	if (GetModuleFileName(NULL, shellfilepath, MAX_PATH) == 0)
	{
		MessageBox(NULL, TEXT("获取文件路径失败"), TEXT("error"), NULL);
		return 1;
	}

    char* shellfiledata = duqufile(shellfilepath);

	
	PIMAGE_DOS_HEADER shellpedosheader = (PIMAGE_DOS_HEADER)shellfiledata;
	PIMAGE_NT_HEADERS32 shellpepeheader = (PIMAGE_NT_HEADERS32)(shellfiledata + shellpedosheader->e_lfanew);
	PIMAGE_FILE_HEADER shellpefileheader = &shellpepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 shellpekexuanheader = &shellpepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER shellpequduanheader = PIMAGE_SECTION_HEADER((char*)shellpekexuanheader + shellpefileheader->SizeOfOptionalHeader);

	char* srcmemdata = (shellfiledata+shellpequduanheader[shellpefileheader->NumberOfSections - 1].PointerToRawData);

	
	for (size_t i = 0; i < shellpequduanheader[shellpefileheader->NumberOfSections - 1].Misc.VirtualSize; i++)
	{
		srcmemdata[i] = srcmemdata[i] ^ 'h';
	}
	
	STARTUPINFO si2 = { 0 };
	PROCESS_INFORMATION pi2;
	si2.cb = sizeof(si2);

	BOOL res = CreateProcess(
		NULL,
		shellfilepath,
		NULL,
		NULL,
		TRUE,
		CREATE_NEW_CONSOLE| CREATE_SUSPENDED,
		NULL,
		NULL, &si2, &pi2);

	if (!res) {
		MessageBox(NULL, TEXT("创建进程失败！"), TEXT("error"), NULL);
		return 1;
	}
	HMODULE nt = LoadLibrary(TEXT("ntdll.dll"));
	pZwUnmapViewOfSection ZwUnmapViewOfSection = (pZwUnmapViewOfSection)GetProcAddress(nt, "ZwUnmapViewOfSection");
	if (ZwUnmapViewOfSection == NULL)
	{
		MessageBox(NULL, TEXT("获取ZwUnmapViewOfSection失败"), TEXT("提示"), MB_OK);
		return -1;
	}

	ZwUnmapViewOfSection(pi2.hProcess, (PVOID)shellpekexuanheader->ImageBase);
	
	//printf("新进程已创建！PID: %d\n", pi2.dwProcessId);  


	PIMAGE_DOS_HEADER srcpedosheader = (PIMAGE_DOS_HEADER)srcmemdata;
	PIMAGE_NT_HEADERS32 srcpepeheader = (PIMAGE_NT_HEADERS32)(srcmemdata + srcpedosheader->e_lfanew);
	PIMAGE_FILE_HEADER srcpefileheader = &srcpepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 srcpekexuanheader = &srcpepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER srcpequduanheader = PIMAGE_SECTION_HEADER((char*)srcpekexuanheader + srcpefileheader->SizeOfOptionalHeader);

	int size = srcpekexuanheader->SizeOfImage;

	LPVOID pRemoteBase = VirtualAllocEx(
		pi2.hProcess,                  // 新进程句柄
		(LPVOID)shellpekexuanheader->ImageBase,  // 指定 ImageBase 为基址
		size,        // 大小=解密PE的SizeOfImage
		MEM_COMMIT | MEM_RESERVE,       // 提交+保留内存
		PAGE_EXECUTE_READWRITE          // 权限：可执行读写（简化处理）
	);
	if (!pRemoteBase) {
	
		MessageBox(NULL, TEXT("申请内存失败！"), TEXT("error"), NULL);
		return 1;
	}
	if ((int)pRemoteBase != shellpekexuanheader->ImageBase) {
		MessageBox(NULL, TEXT("没有申请到指定的内存！"), TEXT("error"), NULL);
		return 1;
	}
	if (srcpekexuanheader->ImageBase != shellpekexuanheader->ImageBase) {
		int pianyi = shellpekexuanheader->ImageBase - srcpekexuanheader->ImageBase;
		if (xiufucdwbiaoinmem(srcmemdata, pianyi)) {
		}
		else {
			MessageBox(NULL, TEXT("修复重定位表失败！"), TEXT("error"), NULL);
			return 1;
		}
	}

	if(!WriteProcessMemory(
		pi2.hProcess,
		pRemoteBase,                  // 目标地址=新进程的ImageBase
		srcmemdata,                   // 源数据=壳最后一个区段解密后的PE程序
		size,                  // 长度=壳最后一个区段的大小（完整PE）
		NULL)) {
		MessageBox(NULL, TEXT("写入内存失败"), TEXT("error"), NULL);
		return 1;
	}
	CONTEXT ctx = { 0 };
	ctx.ContextFlags = CONTEXT_FULL;
	GetThreadContext(pi2.hThread, &ctx);
	ctx.Eax = (DWORD)((char*)pRemoteBase + srcpekexuanheader->AddressOfEntryPoint); 

	SetThreadContext(pi2.hThread, &ctx);
	GetThreadContext(pi2.hThread, &ctx);

	
	ResumeThread(pi2.hThread);
	
	//MessageBox(NULL, TEXT("运行成功！"), TEXT("成功"), NULL);


   
}
