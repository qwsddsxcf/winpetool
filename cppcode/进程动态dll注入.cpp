#include "���̶�̬dllע��.h"
#include<stdio.h>
#include<windows.h>
int jinchengzhurudll(int pid, char* dllpath)
{
	HANDLE shelljubing = NULL;
	shelljubing = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	if (shelljubing == NULL) {
		MessageBox(NULL, TEXT("�򿪽��̾��ʧ�ܣ�"), TEXT("ʧ��"), NULL);
		return 0;
	}
	LPVOID shellmemjubing = VirtualAllocEx(shelljubing, NULL, strlen(dllpath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (shellmemjubing == NULL) {
		MessageBox(NULL, TEXT("�����ڴ�ʧ�ܣ�"), TEXT("ʧ��"), NULL);
		return 0;
	}
	int xieruisok = WriteProcessMemory(shelljubing, shellmemjubing, dllpath, strlen(dllpath) + 1, NULL);
	if (!xieruisok) {
		MessageBox(NULL, TEXT("д���ڴ�ʧ�ܣ�"), TEXT("ʧ��"), NULL);
		return 0;
	}
	HANDLE shellnewxianchengjubing = CreateRemoteThread(shelljubing, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, shellmemjubing, 0, NULL);
	if (shellnewxianchengjubing == NULL) {
		MessageBox(NULL, TEXT("��������ʧ�ܣ�"), TEXT("ʧ��"), NULL);
		return 0;
	}


	MessageBox(NULL, TEXT("��̬ע��dll�ɹ���"), TEXT("�ɹ�"), NULL);

	return 1;
}
