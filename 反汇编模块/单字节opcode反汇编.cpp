#include"反汇编tool.h"
#include "单字节opcode反汇编.h"
#include<stdio.h>
#include <vcruntime_string.h>
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
const char* yunsuanshunxu[] = {"ADD", "OR","ADC:","SBB","AND","SUB","XOR","CMP"};

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
    if ((opcode == 0x81 || opcode == 0x83)||(opcode >= 0x88 && opcode <= 0x8b)) {
     
        unsigned char twoopcode = erjinzhidata[1];
        unsigned char firstmod = (twoopcode >> 6) & 0x03;
        unsigned char firstreg = (twoopcode >> 3) & 0x07;
        unsigned char firstrm = twoopcode & 0x07;
        switch (opcode) {
        case 0x81: {
            return fanhuibian81and83(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
        case 0x83: {
            return fanhuibian81and83(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
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
        case 0x8b:
            return fanhuibian8b(erjinzhidata, outhuibian, firstmod, firstreg, firstrm);
        }
    }



    sprintf(outhuibian, "8x系反汇编失败 opcode:%x\n", opcode);
    return -1;
}

int fanhuibian88(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
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
    const char** myreg32shunxu = NULL;
    toolcheckkeyjuedingreg(myreg32shunxu, 0);
    if (firstmod == 0x3) {
        sprintf(outhuibian, "MOV  %s , %s\n", myreg32shunxu[firstrm], reg32shunxu[firstreg]);
        return 2;
    }
    if (firstrm != 0x4) {
        if (firstmod == 0) {
            if (firstrm == 5) {
                char truemovadd[20] = { 0 };
                toolgettrue32byte(erjinzhidata + 2, truemovadd, 4);
                sprintf(outhuibian, "MOV  %s , DWORD  PTR DS:[%s] \n", myreg32shunxu[firstreg], truemovadd);
                return 6;
            }
            sprintf(outhuibian, "MOV %s , DWORD PTR DS:[%s] \n", myreg32shunxu[firstreg], reg32shunxu[firstrm]);
            return 2;
        }
        else {
            char truemovadd[20] = { 0 };
            int ret = toolgetmovadd(erjinzhidata + 2, truemovadd, firstmod);
            sprintf(outhuibian, "MOV  %s , DWORD PTR DS:[%s%s] \n", myreg32shunxu[firstreg], reg32shunxu[firstrm], truemovadd);
            return 2 + ret;
        }
    }
    char tmpstr[30] = { 0 };
    int ret = toolgetmoverjistr(erjinzhidata + 2, tmpstr, firstmod);
    sprintf(outhuibian, "MOV  %s , DWORD PTR DS:[%s] \n", myreg32shunxu[firstreg], tmpstr);
    return 3 + ret;
}

int fanhuibian81and83(char* erjinzhidata, char* outhuibian, unsigned char firstmod, unsigned char firstreg, unsigned char firstrm)
{
    const char** myreg32shunxu = NULL;
    int sub=toolcheckkeyjuedingreg(myreg32shunxu, 2);
    unsigned char opcode = erjinzhidata[0];
    int getsize = 4 - sub;
    if (opcode == 0x83)getsize = 1;
    char lijishu[20] = { 0 };
    if (firstmod == 3) {
        toolgettrue32byte(erjinzhidata + 2, lijishu, getsize);
        sprintf(outhuibian, "%s   %s  , %s \n", yunsuanshunxu[firstreg], myreg32shunxu[firstrm], lijishu);
        return 2 + getsize;
    }
    else {
        char neicun[20] = { 0 };
        toolgettrue32byte(erjinzhidata + 2, neicun, 4);
        toolgettrue32byte(erjinzhidata + 6, lijishu, getsize);
        sprintf(outhuibian, "%s   DWORD PTR DS:[%s]  , %s \n", yunsuanshunxu[firstreg],neicun, lijishu);
        return 6 + getsize;
    }

    return -1;
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

