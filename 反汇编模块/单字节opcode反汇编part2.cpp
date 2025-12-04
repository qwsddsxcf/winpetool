#include "单字节opcode反汇编part2.h"
#include"反汇编tool.h"
#include<stdio.h>
#include <vcruntime_string.h>
#include <string.h>
#pragma warning(disable : 4996)


int fanhuibian6x(char* erjinzhidata, char* outhuibian)
{
	//该函数是反汇编6x系指令的调用总接口
	unsigned char opcode = erjinzhidata[0];
	if (opcode == 0x68 || opcode == 0x6a) {
		return  fanhuibian68and6a(erjinzhidata, outhuibian);
	}


	sprintf(outhuibian, "6x系反汇编失败 opcode:%x\n", opcode);
	return -1;
}

int fanhuibian68and6a(char* erjinzhidata, char* outhuibian)
{
	//该函数用于反汇编68，6a指令，该指令是push立即数，分别是4字节和一字节
	//4字节受66前缀影响
	unsigned char opcode = erjinzhidata[0];
	const char** myreg32shunxu = NULL;
	int sub = toolcheckkeyjuedingreg(myreg32shunxu, 2);
	int ret = -1;
	char tmpzijie[20] = { 0 };
	if (opcode == 0x68) {
		toolgettrue32byte(erjinzhidata + 1, tmpzijie, 4 - sub);
		ret = 5 - sub;
	}
	else if(opcode == 0x6a)
	{
		toolgettrue32byte(erjinzhidata + 1, tmpzijie, 1);
		ret = 2;
	}
	sprintf(outhuibian, "PUSH  %s\n", tmpzijie);
	return ret;
}
