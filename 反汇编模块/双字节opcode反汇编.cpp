#include"·´»ã±àtool.h"
#include "Ë«×Ö½Úopcode·´»ã±à.h"
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
    sprintf(outhuibian, "Ë«×Ö½Ú 80µ½8fÏµ·´»ã±àÊ§°Ü \n");
    return -1;
}
