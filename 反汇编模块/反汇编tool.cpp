#include"反汇编tool.h"
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
    //当需要第三个字节进行更细化的解析时调用，该函数传入要解析第三字节位置，返回解析了多少偏移
    //并不累加返回当前第三字节
    unsigned char twoopcode = erjinzhidata[0];
    unsigned char firstmod = (twoopcode >> 6) & 0x03;
    unsigned char firstreg = (twoopcode >> 3) & 0x07;
    unsigned char firstrm = twoopcode & 0x07;

    char tmppianyi[20] = { 0 };
        if (beforemod == 0) {
            //根据上一个的字节及解析的mod值，决定是否需要偏移
            if (firstrm == 5) {
                //当上一个字节的md为0 并且 该rm 满足为ebp时规则改变，ebp为空，且该模式下没有偏移变为4字节偏移
                toolgettrue32byte(erjinzhidata + 1, tmppianyi, 4);
                if (firstreg == 4) {
                    sprintf(tmpstr, "%s", tmppianyi);
                    return 4;
                }
                sprintf(tmpstr, "%s*%d+%s", reg32shunxu[firstreg], toolgetnummovetrue(firstmod),tmppianyi);
                return 4;
            }
            //当reg为esp时为空，该规则保持不变
            if (firstreg == 4) {
                sprintf(tmpstr, "%s", reg32shunxu[firstrm]);
                return 0;
            }
            sprintf(tmpstr, "%s+%s*%d", reg32shunxu[firstrm],reg32shunxu[firstreg], toolgetnummovetrue(firstmod));
            return 0;
        }
        else {
            //当上一个字节的mod不为0时，该字节的rm为ebp时正常解析，不为空
            int ret = toolgetmovadd(erjinzhidata + 1, tmppianyi, beforemod);
            //当reg为esp时为空，该规则保持不变
            if (firstreg == 4) {
                sprintf(tmpstr, "%s%s", reg32shunxu[firstrm], tmppianyi);
            }
            sprintf(tmpstr, "%s+%s*%d%s", reg32shunxu[firstrm], reg32shunxu[firstreg], toolgetnummovetrue(firstmod), tmppianyi);
            return ret;
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
int toolreplaceanystr(char* srcstr,char* deletestr,int strlength,char* valuestr)
{
    std::string csrcstr = srcstr;
    size_t pos = csrcstr.find(deletestr);
    if (pos != std::string::npos) {
        {
            csrcstr.replace(pos, strlength, valuestr);
            strcpy(srcstr, csrcstr.c_str());
            return 1;
        }
    }
    return 0;
}
int toolClearAfterChar(char* srcstr, char delimiter)
{
    //该函数用于将字符串的指定字符后的内容滞空
    // 查找分隔符的位置
    char* foundPos = strchr(srcstr, delimiter);

    if (foundPos != nullptr) {
        // 在分隔符位置处截断字符串
        *foundPos = '\0';
        return 1;
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

int toolcheckisyunsuanopcode(unsigned char opcode)
{
    const char opcodes_array[] = { 0x31, 0x33, 0x29, 0x2B, 0x09, 0x0B, 0x21, 0x23, 0x85, 0x39, 0x3B ,0x05,0x03};
    for (size_t i = 0; i < 13; i++)
    {
        if (opcode == (unsigned char)opcodes_array[i])return 1;
    }
    return 0;
}
