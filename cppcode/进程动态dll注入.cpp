#include "进程动态dll注入.h"
#include<stdio.h>
#include<windows.h>
int jinchengzhurudll(int pid, char* dllpath)
{
	HANDLE shelljubing = NULL;
	shelljubing = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (shelljubing == NULL) {
		MessageBox(NULL, TEXT("打开进程句柄失败！"), TEXT("失败"), NULL);
		return 0;
	}
	LPVOID shellmemjubing = VirtualAllocEx(shelljubing, NULL, strlen(dllpath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (shellmemjubing == NULL) {
		MessageBox(NULL, TEXT("申请内存失败！"), TEXT("失败"), NULL);
		return 0;
	}
	int xieruisok = WriteProcessMemory(shelljubing, shellmemjubing, dllpath, strlen(dllpath) + 1, NULL);
	if (!xieruisok) {
		MessageBox(NULL, TEXT("写入内存失败！"), TEXT("失败"), NULL);
		return 0;
	}
	HANDLE shellnewxianchengjubing = CreateRemoteThread(shelljubing, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, shellmemjubing, 0, NULL);
	if (shellnewxianchengjubing == NULL) {
		MessageBox(NULL, TEXT("创建进程失败！"), TEXT("失败"), NULL);
		return 0;
	}


	MessageBox(NULL, TEXT("动态注入dll成功！"), TEXT("成功"), NULL);

	return 1;
}
