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
			//printf("该地址在区段地址之前\n");
			return mempianyi;
		}
		for (size_t i = 0; i < pehead->FileHeader.NumberOfSections; i++)
		{
			if (mempianyi >= quduanhead[i].VirtualAddress && mempianyi <= quduanhead[i].VirtualAddress + quduanhead[i].Misc.VirtualSize)
			{

				//printf("该地址在区段中，已经找到该内存地址对应的文件偏移\n");
				return mempianyi - quduanhead[i].VirtualAddress + quduanhead[i].PointerToRawData;
			}
		}
}
// 把新行追加到 Edit 控件
void AppendEditText(HWND hDlg, int editId, const TCHAR* newText)
{
	HWND hEdit = GetDlgItem(hDlg, editId);
	if (!hEdit) return;

	// 获取当前文本长度 (字符数)
	int len = GetWindowTextLength(hEdit);

	// 移到文本结束处
	SendMessage(hEdit, EM_SETSEL, (WPARAM)len, (LPARAM)len);
	// 将光标设置为在末尾，选择长度为零

	// 用 EM_REPLACESEL 插入文本（包括换行符）
	SendMessage(hEdit, EM_REPLACESEL, FALSE, (LPARAM)newText);

}

char* getshuruinfothentoascii(HWND dadjubing, int id)
{
	HWND hEdit = GetDlgItem(dadjubing, id);
	int textLength = GetWindowTextLength(hEdit);

	if (textLength > 0)
	{
		// 分配缓冲区（多分配一个字符用于 null 终止符）
		wchar_t* wideBuffer = new wchar_t[textLength + 1];

		// 获取文本内容（Unicode）
		GetWindowTextW(hEdit, wideBuffer, textLength + 1);

		// 转换为 ASCII
		int asciiLength = WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, NULL, 0, NULL, NULL);
		char* asciiBuffer = new char[asciiLength];

		WideCharToMultiByte(CP_ACP, 0, wideBuffer, -1, asciiBuffer, asciiLength, NULL, NULL);

		return asciiBuffer;

	}

	return nullptr;
}
