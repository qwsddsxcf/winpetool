#include"·´»ã±àtool.h"
#include<stdio.h>
#include <string>
#include<windows.h>
extern const char* reg32shunxu[];
extern const char* reg16shunxu[];
extern const char* duanregshunxu[];
extern int fanhuibiankeyqianzui66;
#pragma warning(disable : 4996)
int toolgettrue32byte(char* erjinzhidata, char* outzijie, int index)
{
    for (size_t i = 0; i < index; i++)
    {
        sprintf(outzijie + i * 2, "%02x", (unsigned char)erjinzhidata[index - 1 - i]);
    }
    return 1;
}

int toolgetmovadd(char* erjinzhidata, char* addstr, int index)
{
    char tmpaddstr[20] = { 0 };
    int ret = 0;
    switch (index)
    {
    case 1:
        toolgettrue32byte(erjinzhidata, tmpaddstr, 1);
        ret = 1;
        break;
    case 2:
        toolgettrue32byte(erjinzhidata, tmpaddstr, 4);
        ret = 4;
        break;
    default:
        break;
    }
    sprintf(addstr, "+%s", tmpaddstr);
    return ret;
}

int toolgetmoverjistr(char* erjinzhidata, char* tmpstr, unsigned char beforemod)
{
    unsigned char twoopcode = erjinzhidata[0];
    unsigned char firstmod = (twoopcode >> 6) & 0x03;
    unsigned char firstreg = (twoopcode >> 3) & 0x07;
    unsigned char firstrm = twoopcode & 0x07;


    if (firstrm != 5) {
        if (firstreg == 4) {
            sprintf(tmpstr, "%s", reg32shunxu[firstrm]);
        }
        else {
            sprintf(tmpstr, "%s+%s*%d", reg32shunxu[firstrm], reg32shunxu[firstreg], toolgetnummovetrue(firstmod));
        }
        return 0;
    }
    else {
        char tmppianyi[20] = { 0 };
        if (beforemod == 0) {
            toolgettrue32byte(erjinzhidata + 1, tmppianyi, 4);
            if (firstrm == 4) {
                sprintf(tmpstr, "%s", tmppianyi);
            }
            sprintf(tmpstr, "%s*%d+%s", reg32shunxu[firstreg], toolgetnummovetrue(firstmod), tmppianyi);
            return 4;
        }
        else {
            int ret = toolgetmovadd(erjinzhidata + 1, tmppianyi, beforemod);
            if (firstrm == 4) {
                sprintf(tmpstr, "%s%s", reg32shunxu[firstrm], tmppianyi);
            }
            sprintf(tmpstr, "%s+%s*%d%s", reg32shunxu[firstrm], reg32shunxu[firstreg], toolgetnummovetrue(firstmod), tmppianyi);
            return ret;
        }
    }

    return 0;
}
int toolgetnummovetrue(int zhishu)
{
    int num = 1;
    for (size_t i = 0; i < zhishu; i++)
    {
        num = num * 2;
    }
    return num;
}

int toolcheckkeyjuedingreg(const char**& myreg32shunxu,int value)
{
    if (fanhuibiankeyqianzui66) {
        myreg32shunxu = reg16shunxu;
        fanhuibiankeyqianzui66 = 0;
        return value;
    }
    else {
        myreg32shunxu = reg32shunxu;
        return 0;
    }
}

int toolcheckkeyjuedingjmpaddresswei(int& value,int ret)
{
    if (fanhuibiankeyqianzui66) {
        value = value & 0xff;
        fanhuibiankeyqianzui66 = 0;
        return ret;
    }
    else {
        return 0;
    }
}

int toolreplacestrforduanreg(char* srcstr, char* valuestr)
{
    std::string csrcstr = srcstr;
    size_t pos = csrcstr.find("DS:");
    if (pos != std::string::npos) {
        {
            csrcstr.replace(pos, 3, valuestr); 
            strcpy(srcstr, csrcstr.c_str());
            return 1;
        }
    }
    return 0;
}

int toolcheckisduanopcode(unsigned char opcode)
{
    switch (opcode)
    {
    case 0x26:
        return 1;
    case 0x2e:
        return 2;
    case 0x36:
        return 3;
    case 0x3e:
        return 4;
    case 0x64:
        return 5;
    case 0x65:
        return 6;
    default:
        break;
    }

    return 0;
}

int toolreplacestrifisduanopcode(int num, char* outstr)
{
    return toolreplacestrforduanreg(outstr, (char*)duanregshunxu[num]);
}

int toolcheckisnullmeanopcode(unsigned char opcode)
{
    switch (opcode)
    {
    case 0x00:
        return 1;
    default:
        break;
    }

    return 0;
}

int toolgetperukoupianyi(char* memdata,int& pianyi,int &base)
{
    PIMAGE_DOS_HEADER pedosheader = (PIMAGE_DOS_HEADER)memdata;
    PIMAGE_NT_HEADERS32 pepeheader = (PIMAGE_NT_HEADERS32)(memdata + pedosheader->e_lfanew);
    PIMAGE_FILE_HEADER pefileheader = &pepeheader->FileHeader;
    PIMAGE_OPTIONAL_HEADER32 pekexuanheader = &pepeheader->OptionalHeader;

    pianyi = pekexuanheader->AddressOfEntryPoint;
    base = pekexuanheader->ImageBase;
    return 1;
}
