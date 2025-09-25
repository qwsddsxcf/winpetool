#include "tool.h"
#include<windows.h>
#include<stdio.h>

int addressmemtofile(char* filedata,int mempianyi)
{
	
		PIMAGE_DOS_HEADER doshead = (PIMAGE_DOS_HEADER)filedata;

		PIMAGE_NT_HEADERS pehead = (PIMAGE_NT_HEADERS)(doshead->e_lfanew + filedata);

		PIMAGE_SECTION_HEADER quduanhead = IMAGE_FIRST_SECTION(pehead);

		if (mempianyi < quduanhead[0].VirtualAddress)
		{
			//printf("�õ�ַ�����ε�ַ֮ǰ\n");
			return mempianyi;
		}
		for (size_t i = 0; i < pehead->FileHeader.NumberOfSections; i++)
		{
			if (mempianyi >= quduanhead[i].VirtualAddress && mempianyi <= quduanhead[i].VirtualAddress + quduanhead[i].Misc.VirtualSize)
			{

				//printf("�õ�ַ�������У��Ѿ��ҵ����ڴ��ַ��Ӧ���ļ�ƫ��\n");
				return mempianyi - quduanhead[i].VirtualAddress + quduanhead[i].PointerToRawData;
			}
		}
}
// ������׷�ӵ� Edit �ؼ�
void AppendEditText(HWND hDlg, int editId, const TCHAR* newText)
{
	HWND hEdit = GetDlgItem(hDlg, editId);
	if (!hEdit) return;

	// ��ȡ��ǰ�ı����� (�ַ���)
	int len = GetWindowTextLength(hEdit);

	// �Ƶ��ı�������
	SendMessage(hEdit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
	// ���������Ϊ��ĩβ��ѡ�񳤶�Ϊ��

	// �� EM_REPLACESEL �����ı����������з���
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)newText);

}

char* getshuruinfothentoascii(HWND dadjubing, int id)
{
	HWND hEdit = GetDlgItem(dadjubing, id);
	int textLength = GetWindowTextLength(hEdit);

	if (textLength > 0)
	{
		// ���仺�����������һ���ַ����� null ��ֹ����
		wchar_t* wideBuffer = new wchar_t[textLength + 1];

		// ��ȡ�ı����ݣ�Unicode��
		GetWindowTextW(hEdit, wideBuffer, textLength + 1);

		// ת��Ϊ ASCII
		int asciiLength = WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, NULL, 0, NULL, NULL);
		char* asciiBuffer = new char[asciiLength];

		WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, asciiBuffer, asciiLength, NULL, NULL);

		return asciiBuffer;

	}

	return nullptr;
}
