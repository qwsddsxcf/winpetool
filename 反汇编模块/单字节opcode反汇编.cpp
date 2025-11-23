#include"反汇编tool.h"
#include "单字节opcode反汇编.h"
#include<stdio.h>
#include <vcruntime_string.h>
#include <string.h>
#pragma warning(disable : 4996)

const char* reg32shunxu[] = {
    "EAX", "ECX", "EDX", "EBX",
    "ESP", "EBP", "ESI", "EDI"
};
const char* reg16shunxu[] = {
    "AX", "CX", "DX", "BX",
    "SP", "BP", "SI", "DI"
};
const char* reg8to16shunxu[] = {
    "AL", "CL", "DL", "BL",
    "AH", "CH", "DH", "BH"
};
const char* jmpshunxu[] = { "JO","JNO", "JB/JNAE/JC","JNB/JAE/JNC",
    "JZ/JE", "JNZ/JNE","JBE/JNA", "JNBE/JA", "JS", "JNS", "JP/JPE",
    "JNP/JPO","JL/JNGE", "JNL/JGE","JLE/JNG", "JNLE/JG"
};
const char* loopshunxu[] = { "LOOPNE/LOOPNZ","LOOPE/LOOPZ","LOOP","JrCXZ" };
const char* calljmpshunxu[] = { "CALL","JMP","JMP FAR","JMP SHORT" };
const char* retshunxu[] = { "RET","RET","RETF","RETF" };
const char* duanregshunxu[] = {"", "ES:","CS:","SS:","DS:","FS:","GS:"};
const char* yunsuanshunxu[] = {"ADD", "OR","ADC:","SBB","AND","SUB","XOR","CMP","TEST"};
const char* ffopcodecaozuofanyi[] = { "INC","DEC", "CALL", "CALL FAR", "JMP", "JMP FAR", "PUSH"};

int fanhuibianB0toBf(char* erjinzhidata, char* outhuibian)
{
    unsigned char opcode = erjinzhidata[0];
    if (opcode >= 0xb0 && opcode <= 0xbf) {

        int jicunqi8to16 = opcode & 0xF;

        if (jicunqi8to16 <= 0x07) {

            sprintf(outhuibian, "MOV  %s , %02x\n", reg8to16shunxu[jicunqi8to16], (unsigned char)erjinzhidata[1]);
            return 2;
        }
        else {
            const char** myreg32shunxu = NULL;
            int size=toolcheckkeyjuedingreg(myreg32shunxu, 2);
            char datatmp[20] = { 0 };
            int jicun32 = jicunqi8to16 - 0x8;
            toolgettrue32byte(erjinzhidata + 1, datatmp, 4-size);
            sprintf(outhuibian, "MOV  %s , %s\n", myreg32shunxu[jicun32], datatmp);
            return 5-size;
        }
    }
    sprintf(outhuibian, "b0到bf系反汇编失败 \n", opcode);
    return -1;
}

int fanhuibian90to97(char* erjinzhidata, char* outhuibian)
{
    unsigned char opcode = erjinzhidata[0];
    if (opcode >= 0x90 && opcode <= 0x97) {
        int jicunqi32 = opcode & 0xF;
        if (jicunqi32 == 0x0) {
            sprintf(outhuibian, "NOP \n");
        }
        else {
            const char** myreg32shunxu = NULL;
            toolcheckkeyjuedingreg(myreg32shunxu, 0);
            sprintf(outhuibian, "XCHG %s, %s \n", myreg32shunxu[0], myreg32shunxu[jicunqi32]);
        }
        return 1;
    }
    sprintf(outhuibian, "90到97系反汇编失败 \n", opcode);
    return -1;
}

int fanhuibiancx(char* erjinzhidata, char* outhuibian)
{
    unsigned char opcode = erjinzhidata[0];
    if (opcode == 0xc7) {
        unsigned char twoopcode = erjinzhidata[1];
        unsigned char firstmod = (twoopcode >> 6) & 0x03;
        unsigned char firstreg = (twoopcode >> 3) & 0x07;
        unsigned char firstrm = twoopcode & 0x07;
        return fanhuibianc7(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
    }
    if ((opcode >= 0xc2 && opcode <= 0xc3) || (opcode >= 0xca && opcode <= 0xcb)) {
        int size = 1;
        int retindex = opcode & 0xF - 2;
        char truetwosizezijie[20] = { 0 };
        if (opcode == 0xc2 || opcode == 0xca) {
            toolgettrue32byte(erjinzhidata + 1, truetwosizezijie, 2);
            size = 3;
        }
        if (opcode == 0xca || opcode == 0xcb) {
            retindex = opcode & 0xF - 8;
        }
        sprintf(outhuibian, "%s  %s\n", retshunxu[retindex], truetwosizezijie);
        return size;
    }
    sprintf(outhuibian, "cx系反汇编失败 opcode:%x\n", opcode);
    return -1;
}

int fanhuibian8x(char* erjinzhidata, char* outhuibian)
{
    unsigned char opcode = erjinzhidata[0];
    if ((opcode >= 0x80 || opcode <= 0x83) || (opcode >= 0x88 && opcode <= 0x8d)) {

        unsigned char twoopcode = erjinzhidata[1];
        unsigned char firstmod = (twoopcode >> 6) & 0x03;
        unsigned char firstreg = (twoopcode >> 3) & 0x07;
        unsigned char firstrm = twoopcode & 0x07;
        if (opcode >= 0x80 && opcode <= 0x83) {
            return fanhuibian80to83(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
        switch (opcode) {
        case 0x88: {
            return fanhuibian88(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
        case 0x89: {
            return fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
        case 0x8a:
        {
            return fanhuibian8a(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
        case 0x8b:{
            return fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
        case 0x8d:{
            return fanhuibian8d(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
    }
    }

    sprintf(outhuibian, "8x系反汇编失败 opcode:%x\n", opcode);
    return -1;
}

int fanhuibianc7(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    const char** myreg32shunxu = NULL;
    int sub=toolcheckkeyjuedingreg(myreg32shunxu, 2);
    char truemovadd[20] = { 0 };
    char truemovadd2[20] = { 0 };
    if (firstmod == 0x3) {
        toolgettrue32byte(erjinzhidata + 2, truemovadd, 4-sub);
        sprintf(outhuibian, "MOV  %s , %s\n", reg32shunxu[firstrm], truemovadd);
        return 6-sub;
    }
    if (firstrm != 0x4) {
        if (firstmod == 0) {
            if (firstrm == 5) {
                toolgettrue32byte(erjinzhidata + 2, truemovadd, 4);
                toolgettrue32byte(erjinzhidata + 6, truemovadd2, 4-sub);
                sprintf(outhuibian, "MOV DWORD  PTR DS:[%s] , %s\n", truemovadd, truemovadd2);
                return 10-sub;
            }
            toolgettrue32byte(erjinzhidata + 2, truemovadd, 4-sub);
            sprintf(outhuibian, "MOV DWORD PTR DS:[%s] , %s\n", reg32shunxu[firstrm], truemovadd);
            return 6-sub;
        }
        else {
            int ret = toolgetmovadd(erjinzhidata + 2, truemovadd, firstmod);
            toolgettrue32byte(erjinzhidata + 2+ret, truemovadd2, 4 - sub);
            sprintf(outhuibian, "MOV DWORD PTR DS:[%s%s] , %s\n", reg32shunxu[firstrm], truemovadd, truemovadd2);
            return 6 + ret-sub;
        }
    }

    int ret = toolgetmoverjistr(erjinzhidata + 2, truemovadd, firstmod);
    toolgettrue32byte(erjinzhidata + 2 + ret, truemovadd2, 4 - sub);
    sprintf(outhuibian, "MOV DWORD PTR DS:[%s] , %s\n", truemovadd, truemovadd2);
    return 7 + ret - sub;
}

int fanhuibian88(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    //注释参见fanhuibian8b函数
    if (firstmod == 0x3) {
        sprintf(outhuibian, "MOV  %s , %s\n", reg8to16shunxu[firstrm], reg8to16shunxu[firstreg]);
        return 2;
    }
    if (firstrm != 0x4) {
        if (firstmod == 0) {
            if (firstrm == 5) {
                char truemovadd[20] = { 0 };
                toolgettrue32byte(erjinzhidata + 2, truemovadd, 4);
                sprintf(outhuibian, "MOV BYTE/WORD  PTR DS:[%s] , %s\n", truemovadd, reg8to16shunxu[firstreg]);
                return 6;
            }
            sprintf(outhuibian, "MOV BYTE/WORD PTR DS:[%s] , %s\n", reg32shunxu[firstrm], reg8to16shunxu[firstreg]);
            return 2;
        }
        else {
            char truemovadd[20] = { 0 };
            int ret = toolgetmovadd(erjinzhidata + 2, truemovadd, firstmod);
            sprintf(outhuibian, "MOV BYTE/WORD PTR DS:[%s%s] , %s\n", reg32shunxu[firstrm], truemovadd, reg8to16shunxu[firstreg]);
            return 2 + ret;
        }
    }
    char tmpstr[30] = { 0 };
    int ret = toolgetmoverjistr(erjinzhidata + 2, tmpstr, firstmod);
    sprintf(outhuibian, "MOV BYTE/WORD PTR DS:[%s] , %s\n", tmpstr, reg8to16shunxu[firstreg]);
    return 3 + ret;
}

int fanhuibian89(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    //注释参见fanhuibian8b函数
    const char** myreg32shunxu = NULL;
    toolcheckkeyjuedingreg(myreg32shunxu, 0);
    if (firstmod == 0x3) {
        sprintf(outhuibian, "MOV  %s , %s\n", reg32shunxu[firstrm], myreg32shunxu[firstreg]);
        return 2;
    }
    if (firstrm != 0x4) {
        if (firstmod == 0) {
            if (firstrm == 5) {
                char truemovadd[20] = { 0 };
                toolgettrue32byte(erjinzhidata + 2, truemovadd, 4);
                sprintf(outhuibian, "MOV DWORD  PTR DS:[%s] , %s\n", truemovadd, myreg32shunxu[firstreg]);
                return 6;
            }
            sprintf(outhuibian, "MOV DWORD PTR DS:[%s] , %s\n", reg32shunxu[firstrm], myreg32shunxu[firstreg]);
            return 2;
        }
        else {
            char truemovadd[20] = { 0 };
            int ret = toolgetmovadd(erjinzhidata + 2, truemovadd, firstmod);
            sprintf(outhuibian, "MOV DWORD PTR DS:[%s%s] , %s\n", reg32shunxu[firstrm], truemovadd, myreg32shunxu[firstreg]);
            return 2 + ret;
        }
    }
    char tmpstr[30] = { 0 };
    int ret = toolgetmoverjistr(erjinzhidata + 2, tmpstr, firstmod);
    sprintf(outhuibian, "MOV DWORD PTR DS:[%s] , %s\n", tmpstr, myreg32shunxu[firstreg]);
    return 3 + ret;
}

int fanhuibian8a(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    //注释参见fanhuibian8b函数
    if (firstmod == 0x3) {
        sprintf(outhuibian, "MOV  %s , %s\n", reg8to16shunxu[firstreg], reg8to16shunxu[firstrm]);
        return 2;
    }
    if (firstrm != 0x4) {
        if (firstmod == 0) {
            if (firstrm == 5) {
                char truemovadd[20] = { 0 };
                toolgettrue32byte(erjinzhidata + 2, truemovadd, 4);
                sprintf(outhuibian, "MOV  %s , BYTE/WORD  PTR DS:[%s] \n", reg8to16shunxu[firstreg], truemovadd);
                return 6;
            }
            sprintf(outhuibian, "MOV  %s , BYTE/WORD PTR DS:[%s] \n", reg8to16shunxu[firstreg], reg32shunxu[firstrm]);
            return 2;
        }
        else {
            char truemovadd[20] = { 0 };
            int ret = toolgetmovadd(erjinzhidata + 2, truemovadd, firstmod);
            sprintf(outhuibian, "MOV  %s , BYTE/WORD PTR DS:[%s%s] \n", reg8to16shunxu[firstreg], reg32shunxu[firstrm], truemovadd);
            return 2 + ret;
        }
    }
    char tmpstr[30] = { 0 };
    int ret = toolgetmoverjistr(erjinzhidata + 2, tmpstr, firstmod);
    sprintf(outhuibian, "MOV  %s , BYTE/WORD PTR DS:[%s] \n", reg8to16shunxu[firstreg], tmpstr);
    return 3 + ret;
}

int fanhuibian8b(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    //反汇编mov,这是32为,规则是reg ,reg/rm ,下面两句代码是检查是否存在66前缀，存在则只替换寄存器，取多少内存不受影响
    //传入opcode后紧跟的一位字节解析出的三部分
    const char** myreg32shunxu = NULL;
    toolcheckkeyjuedingreg(myreg32shunxu, 0);
    if (firstmod == 0x3) {
        //当mod是3，该模式是两个寄存器
        sprintf(outhuibian, "MOV  %s , %s\n", myreg32shunxu[firstrm], reg32shunxu[firstreg]);
        return 2;
    }
    if (firstrm != 0x4) {
        //rm不等于4时不需要新的解析码，另一个操作数被设为内存或寄存器，无运算
        if (firstmod == 0) {
            //mod为0时默认直接寄存器当作内存寻址没有偏移
            if (firstrm == 5) {
                //rm等于5时mod为0，这是特殊情况寄存器消失，用4字节当作地址
                char truemovadd[20] = { 0 };
                toolgettrue32byte(erjinzhidata + 2, truemovadd, 4);
                sprintf(outhuibian, "MOV  %s , DWORD PTR DS:[%s] \n", myreg32shunxu[firstreg], truemovadd);
                return 6;
            }
            //默认是一个寄存器
            sprintf(outhuibian, "MOV %s , DWORD PTR DS:[%s] \n", myreg32shunxu[firstreg], reg32shunxu[firstrm]);
            return 2;
        }
        else {
            //mod是1或者2，是寄存器和偏移的组合，偏移值是1，4字节
            char truemovadd[20] = { 0 };
            int ret = toolgetmovadd(erjinzhidata + 2, truemovadd, firstmod);//该函数自动根据mod判断取多少字节偏移
            sprintf(outhuibian, "MOV  %s , DWORD PTR DS:[%s%s] \n", myreg32shunxu[firstreg], reg32shunxu[firstrm], truemovadd);
            return 2 + ret;
        }
    }
    //rm是4，这种情况就是寄存器的复杂运算，需要一个新的解析字节
    char tmpstr[30] = { 0 };
    int ret = toolgetmoverjistr(erjinzhidata + 2, tmpstr, firstmod);
    sprintf(outhuibian, "MOV  %s , DWORD PTR DS:[%s] \n", myreg32shunxu[firstreg], tmpstr);
    return 3 + ret;
}

int fanhuibian8d(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    //该反汇编为lea操作，该操作和89mov完全相同，只需替换字符串即可完成解析
    int ret=fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
    toolreplaceanystr(outhuibian, (char*)"DWORD PTR DS:", strlen("DWORD PTR DS:"), (char*)"");
    toolreplaceanystr(outhuibian, (char*)"MOV", strlen("MOV"), (char*)"LEA");
    return ret;
}

int fanhuibian80to83(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    //该函数用于反汇编运算符指令如cmp,and等，下面两个数组分别存储立即数和偏移
    char lijishu[20] = { 0 };
    char neicun[20] = { 0 };
    const char** myreg32shunxu = NULL;
    int sub=toolcheckkeyjuedingreg(myreg32shunxu, 2);
    unsigned char opcode = erjinzhidata[0];
    int getsize = 4 - sub;
    int pianyi = 4;

    if (opcode == 0x83)getsize = 1;//83，80，82指令使用一位立即数
    if (opcode == 0x80|| opcode ==0x82) {
        //80，82指令完全相同，并且在寄存器模式下使用16位寄存器
        getsize = 1;
        myreg32shunxu = reg16shunxu;
    }

    if (firstmod == 3|| firstmod == 0) {
        //3是寄存器模式，0模式没有偏移，这两个模式都只需要取一次内存（立即数）
        toolgettrue32byte(erjinzhidata + 2, lijishu, getsize);
        if(firstmod == 3)sprintf(outhuibian, "%s   %s  , %s \n", yunsuanshunxu[firstreg], myreg32shunxu[firstrm], lijishu);
        else sprintf(outhuibian, "%s    DWORD PTR DS:[%s]  , %s \n", yunsuanshunxu[firstreg], reg32shunxu[firstrm], lijishu);
        return 2 + getsize;
    }
    else {
        //2模式取偏移4字节，1模式取偏移1字节
        if (firstmod == 1)pianyi = 1;
        toolgettrue32byte(erjinzhidata + 2, neicun, pianyi);
        toolgettrue32byte(erjinzhidata + 2+ pianyi, lijishu, getsize);
        sprintf(outhuibian, "%s   DWORD PTR DS:[%s+%s]  , %s \n", yunsuanshunxu[firstreg],reg32shunxu[firstrm], neicun, lijishu);
        return 2+pianyi+ getsize;
    }
    return -1;
}

int fanhuibianyunsuanopcode(char* erjinzhidata, char* outhuibian)
{
    unsigned char opcode = erjinzhidata[0];

    unsigned char twoopcode = erjinzhidata[1];
    unsigned char firstmod = (twoopcode >> 6) & 0x03;
    unsigned char firstreg = (twoopcode >> 3) & 0x07;
    unsigned char firstrm = twoopcode & 0x07;

    int yunsuanindex = 0;
    int ret = 0;
    switch (opcode) {
    case 0x03:
        ret = fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 0;
        break;
    case 0x05:
        return fanhuibian05(erjinzhidata, outhuibian);
    case 0x09:
        ret=fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex =1 ;
        break;
    case 0x0B:
        ret=fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 1;
        break;
    case 0x21:
        ret=fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        break;
    case 0x23:
        ret=fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        break;
    case 0x29:
        ret= fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 3;
        break;
    case 0x2B:
        ret=fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 3;
        break;
    case 0x31:
        ret=fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 6;
        break;
    case 0x33:
        ret=fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 6;
        break;
    case 0x39:
        ret = fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 7;
        break;
    case 0x3B:
        ret = fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 7;
        break;
    case 0x85:
        ret = fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        yunsuanindex = 9;
        break;
    default:
        break;
    }
    toolreplaceanystr(outhuibian, (char*)"MOV", 3, (char*)yunsuanshunxu[yunsuanindex]);
    return ret;
}

int fanhuibian70to7F(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];

    if (opcode >= 0x70 && opcode <= 0x7f) {
        int sub=toolcheckkeyjuedingjmpaddresswei(curjizhi,1);
        int jmpaddress = curjizhi + 2 + erjinzhidata[1];
        if (sub) {
            jmpaddress = (short)jmpaddress;
        }
        int jmpindex = opcode & 0xF;
        sprintf(outhuibian, "%s  %x\n", jmpshunxu[jmpindex], jmpaddress);
        return 2;
    }
    sprintf(outhuibian, "70到7f系反汇编失败 opcode:%x\n",opcode);
    return -1;
}

int fanhuibianEx(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];
    if ((opcode >= 0xe0 && opcode <= 0xe3)|| opcode== 0xeb) {
        toolcheckkeyjuedingjmpaddresswei(curjizhi,0);
        int loopaddress = curjizhi + 2 + erjinzhidata[1];
        int loopindex = opcode & 0xF;
        if (opcode == 0xeb) {
            sprintf(outhuibian, "%s  %x\n", calljmpshunxu[3], loopaddress);
            return 2;
        }
        sprintf(outhuibian, "%s  %x\n", loopshunxu[loopindex], loopaddress);
        return 2;
    }
    if (opcode >= 0xe8 && opcode < 0xeb) {
        char truezijie[20] = { 0 };
        int calljmpx = 0;
        int sub=toolcheckkeyjuedingjmpaddresswei(curjizhi, 2);
        toolgettrue32byte(erjinzhidata + 1, truezijie, 4-sub);

        sscanf(truezijie, "%x", &calljmpx);

        int factaddress = curjizhi + calljmpx + 5;
        if (sub) {
            factaddress = (short)factaddress;
        }
        int calljmpindex = (opcode & 0xF) - 0x8;

        if (calljmpindex == 0x2) {
            char twotruezijie[20] = { 0 };
            factaddress = factaddress + 2;
            toolgettrue32byte(erjinzhidata + 5-sub, twotruezijie, 2);
            sprintf(outhuibian, "%s  %s:%x\n", calljmpshunxu[calljmpindex], twotruezijie, factaddress);
            return 7-sub;
        }
        sprintf(outhuibian, "%s  %x\n", calljmpshunxu[calljmpindex], factaddress);
        return 5-sub;
    }

    sprintf(outhuibian, "EX系反汇编失败 opcode:%x\n",opcode);
    return -1;
}

int fanhuibian05(char* erjinzhidata, char* outhuibian)
{
    unsigned char opcode = erjinzhidata[0];
    char tmp[20] = { 0 };

    const char** myreg32shunxu = NULL;
    int sub = toolcheckkeyjuedingreg(myreg32shunxu, 2);
    toolgettrue32byte(erjinzhidata + 1, tmp, 4 - sub);

    if (opcode == 0x5) {
        sprintf(outhuibian, "%s  EAX,  %s \n", yunsuanshunxu[0],tmp);
        return 5 - sub;
    }

    sprintf(outhuibian, "03反汇编失败 \n");
    return -1;
}

int fanhuibiana0toa3(char* erjinzhidata, char* outhuibian)
{
    //反汇编从a0,a3
    unsigned char opcode = erjinzhidata[0];
    char tmp[20] = { 0 };

    const char** myreg32shunxu = NULL;
    toolcheckkeyjuedingreg(myreg32shunxu, 0);
    toolgettrue32byte(erjinzhidata + 1, tmp, 4);

    switch (opcode)
    {
    case 0xa0:
        sprintf(outhuibian, "MOV  %s , BYTE/WORD PTR DS:[%s] \n", reg32shunxu[0], tmp);
        break;
    case 0xa1:
        sprintf(outhuibian, "MOV  %s , DWORD PTR DS:[%s] \n", myreg32shunxu[0], tmp);
        break;
    case 0xa2:
        sprintf(outhuibian, "MOV   BYTE/WORD PTR DS:[%s] , %s\n", tmp, reg32shunxu[0]);
        break;
    case 0xa3:
        sprintf(outhuibian, "MOV  DWORD PTR DS:[%s] , %s \n", tmp,myreg32shunxu[0]);
        break;
    default:
        sprintf(outhuibian, "a0到a3反汇编失败 \n");
        return -1;
    }
    return 5;
}

int fanhuibianff(char* erjinzhidata, char* outhuibian)
{
    //该函数用于反汇编ff,该操作码是一组，当中的reg从寄存器替换为操作字符
    //其余解析规则和mov相同
    unsigned char twoopcode = erjinzhidata[1];
    unsigned char firstmod = (twoopcode >> 6) & 0x03;
    unsigned char firstreg = (twoopcode >> 3) & 0x07;
    unsigned char firstrm = twoopcode & 0x07;

    int ret=fanhuibian89(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
    //替换操作码，清除，后内容
    toolreplaceanystr(outhuibian, (char*)"MOV", strlen("MOV"), (char*)(ffopcodecaozuofanyi[firstreg]));
    toolClearAfterChar(outhuibian, ',');
    return ret;
}


int fanhuibian40to5F(char* erjinzhidata, char* outhuibian)
{
    const char** myreg32shunxu = NULL;
    toolcheckkeyjuedingreg(myreg32shunxu,0);
    char opcode = erjinzhidata[0];
    if (opcode >= 0x40 && opcode <= 0x4f) {
        int jicunqi32 = opcode & 0xF;
        if (jicunqi32 <= 0x7) {
            sprintf(outhuibian, "INC  %s \n", myreg32shunxu[jicunqi32]);
        }
        else {
            jicunqi32 = jicunqi32 - 0x8;
            sprintf(outhuibian, "DEC  %s \n", myreg32shunxu[jicunqi32]);
        }
        return 1;
    }
    if (opcode >= 0x50 && opcode <= 0x5f) {
        int jicunqi32 = opcode & 0xF;
        if (jicunqi32 <= 0x7) {
            sprintf(outhuibian, "PUSH  %s \n", myreg32shunxu[jicunqi32]);
        }
        else {
            jicunqi32 = jicunqi32 - 0x8;
            sprintf(outhuibian, "POP  %s \n", myreg32shunxu[jicunqi32]);
        }
        return 1;
    }
    sprintf(outhuibian, "40到50系反汇编失败 opcode:%x\n", opcode);

    return -1;
}

