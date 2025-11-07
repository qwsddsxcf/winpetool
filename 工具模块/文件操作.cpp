
//#include "framework.h"
#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include "../资源模块/resource.h"
#include "tool.h"
#include <stdio.h>
#pragma warning(disable : 4996)
int size=0;
char* duqufile(TCHAR* filepath)
{
	FILE* file = _wfopen(filepath, L"rb");
	//读取文件
	if (file == NULL)
	{
		MessageBox(NULL, TEXT("读取文件失败"), TEXT("error"), NULL);
		return NULL;
	}
	//获得文件大小
	fseek(file, 0, SEEK_END);
	int filesize = ftell(file);
	rewind(file);
	//将文件内容存入缓冲区并返回
	char* filedata = (char*)malloc(sizeof(char) * filesize);
	fread(filedata, 1, filesize, file);
	printf("读取文件成功！\n");
	/*printf("%syyyyyy\n", filedata);*/
	return filedata;
}

char* pefiletomemory(char* filedata)
{
	PIMAGE_DOS_HEADER pedosheader= (PIMAGE_DOS_HEADER)filedata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(filedata+pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
	
	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);

	/*printf("%d",pekexuanheader->Magic);*/
	//printf("%d\n", pepeheader->FileHeader.NumberOfSections);
	//printf("%d\n", pekexuanheader->FileAlignment);
	//printf("%d\n", pedosheader->e_magic);
	//printf("%d\n", pedosheader->e_lfanew);
	//printf("%s", filedata);
	char* memorydata = (char*)malloc(sizeof(char) * pekexuanheader->SizeOfImage);
	if (!memorydata) {
		fprintf(stderr, "malloc failed, SizeOfImage=%u\n", pekexuanheader->SizeOfImage);
		return NULL;
	}
	memset(memorydata, 0,pekexuanheader->SizeOfImage);

	memcpy(memorydata, filedata, pekexuanheader->SizeOfHeaders);

	//printf("1\n");
	for (size_t i = 0; i < pefileheader->NumberOfSections; i++)
	{
		int filesize = pequduanheader[i].SizeOfRawData;
		int memsize = pequduanheader[i].Misc.VirtualSize;
		int factsize = filesize < memsize ? filesize : memsize;
		/*printf("%d\n", pequduanheader[i].VirtualAddress);*/
		memcpy(memorydata + pequduanheader[i].VirtualAddress, filedata + pequduanheader[i].PointerToRawData, factsize);
		/*printf("1\n");*/
	}
	printf("已经成功将pe文件拉伸\n");
	return memorydata;

}

char* pememorytofile(char* memdata)
{
	//printf("111\n");
	PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
	PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
	PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
	PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;
	PIMAGE_SECTION_HEADER pequduanheader = PIMAGE_SECTION_HEADER((char*)pekexuanheader + pefileheader->SizeOfOptionalHeader);
	//printf("111\n");
	int quduanshu = pefileheader->NumberOfSections;
	int filesize = pequduanheader[quduanshu - 1].PointerToRawData + pequduanheader[quduanshu - 1].SizeOfRawData;
	//printf("111\n");
	char* filedata = (char*)malloc(sizeof(char) * filesize);
	memset(filedata, 0, filesize);

	memcpy(filedata, memdata, pekexuanheader->SizeOfHeaders);


	for (size_t i = 0; i < pefileheader->NumberOfSections; i++)
	{
		memcpy(filedata + pequduanheader[i].PointerToRawData, memdata + pequduanheader[i].VirtualAddress,
			pequduanheader[i].SizeOfRawData);
	}
	//MessageBox(NULL, TEXT("1"), NULL, NULL);
	printf("已经成功将pe文件还原\n");
	size = filesize;
	return filedata;

}

void savefile(char* filedata, TCHAR* path,int size)
{
	FILE* file = _wfopen(path, L"wb");
	//读取文件
	if (file == NULL)
	{
		MessageBox(NULL, TEXT("读取文件失败"), TEXT("error"), NULL);
		return;
	}
	fwrite(filedata, 1, size, file);
	printf("已成功将文件内容保存！");

}

