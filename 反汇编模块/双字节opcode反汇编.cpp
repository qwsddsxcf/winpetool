#include"反汇编tool.h"
#include "双字节opcode反汇编.h"
#include"单字节opcode反汇编.h"
#include<stdio.h>
#pragma warning(disable : 4996)
extern const char* jmpshunxu[];
int twofanhuibian80to8f(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[1];
    if (opcode >= 0x80 && opcode <= 0x8f) {
        char truezijie[20] = { 0 };
        int calljmpx = 0;
        int sub = toolcheckkeyjuedingjmpaddresswei(curjizhi, 2);

        toolgettrue32byte(erjinzhidata + 2, truezijie, 4-sub);
        sscanf(truezijie, "%x", &calljmpx);

        int factaddress = curjizhi + calljmpx + 5;
        if (sub) {
            factaddress = (short)factaddress;
        }
        int jmpindex = opcode & 0xF;
        sprintf(outhuibian, "%s  %x\n", jmpshunxu[jmpindex], factaddress);
        return 6-sub;
    }
    sprintf(outhuibian, "双字节 80到8f系反汇编失败 \n");
    return -1;
}

int twofanhuibianmov(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    //该函数用于双字节的movsx,movzx操作，解析规则和单字节的mov一样，直接使用字符串替换
    //关键内容即可
    unsigned char opcode = erjinzhidata[1];
    unsigned char twoopcode = erjinzhidata[2];
    unsigned char firstmod = (twoopcode >> 6) & 0x03;
    unsigned char firstreg = (twoopcode >> 3) & 0x07;
    unsigned char firstrm = twoopcode & 0x07;
    //这里需要从当前双字节指令的第二个字节开始传入，（进行mov二级解析式正确传入）
    int ret= fanhuibian8b(erjinzhidata+1, outhuibian, firstmod, firstreg, firstrm);
    if (opcode >= 0xb6 && opcode <= 0xb7) {
        toolreplaceanystr(outhuibian, (char*)"MOV", 3, (char*)"MOVZX");
        if (opcode == 0xb6) {
            toolreplaceanystr(outhuibian, (char*)"DWORD", 5, (char*)"BYTE");
        }
        if (opcode == 0xb7) {
            toolreplaceanystr(outhuibian, (char*)"DWORD", 5, (char*)"WORD");
        }
    }
    if (opcode >= 0xbe && opcode <= 0xbf) {
        toolreplaceanystr(outhuibian, (char*)"MOV", 3, (char*)"MOVSX");
        if (opcode == 0xbe) {
            toolreplaceanystr(outhuibian, (char*)"DWORD", 5, (char*)"BYTE");
        }
        if (opcode == 0xbf) {
            toolreplaceanystr(outhuibian, (char*)"DWORD", 5, (char*)"WORD");
        }
    }
    return ret+1;
}
