#include "反汇编引擎.h"
#include"单字节opcode反汇编.h"
#include "双字节opcode反汇编.h"
#include"反汇编tool.h"
#include "../pe静态操作/pe基础信息展示.h"
#include<stdio.h>
#pragma warning(disable : 4996)
int fanhuibiankeyqianzui66 = 0;
int fabnhuibianmain(int cjizhi,char* erjinzhidata,int size,HWND dadjubing,int id)    
{
    int pianyi = 0;
    int jizhi = cjizhi;
   
    for (; pianyi < size;)
    {
        char opcode = erjinzhidata[pianyi];
        char tmp[70] = {0};
        char yuanshi16data[50] = { 0 };
        int returnpianyi= apifanhuibianchoseopcodesizefun(erjinzhidata+pianyi, tmp,jizhi);

        if (returnpianyi == -1) {
           showpefanhuibian(dadjubing, id, jizhi, yuanshi16data, tmp);
           return -1;
        }

        for (size_t k = 0; k < returnpianyi; k++)
        {
            sprintf(yuanshi16data+k*3, "%02X ", (unsigned char)erjinzhidata[pianyi+k]);
        }
        showpefanhuibian(dadjubing, id, jizhi, yuanshi16data, tmp);
     
        pianyi = pianyi + returnpianyi;
        jizhi = jizhi + returnpianyi;
    }
    return 1;
}
int apifanhuibianchoseopcodesizefun(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];
    int duankey = toolcheckisduanopcode(opcode);
    if (opcode == 0xf) {
        return apifanhuibiantwobyteopcodechose(erjinzhidata, outhuibian, curjizhi);
    }
    else if (opcode == 0x66) {
        fanhuibiankeyqianzui66 = 1;
        return apifanhuibianchoseopcodesizefun(erjinzhidata+1, outhuibian, curjizhi+1)+1;
    }
    else if (duankey){
        int ret= apifanhuibianchoseopcodesizefun(erjinzhidata + 1, outhuibian, curjizhi + 1) + 1;
        toolreplacestrifisduanopcode(duankey, outhuibian);
        return ret;
    }
    else if (toolcheckisnullmeanopcode(opcode)) {
        return 1;
    }
    else {
        return  apifanhuibianonebyteopcodechose(erjinzhidata, outhuibian, curjizhi);
    }
 
}


int apifanhuibianonebyteopcode40to7f(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];
    if (opcode >= 0x40 && opcode <= 0x5f) {
        return fanhuibian40to5F(erjinzhidata, outhuibian);
    }
    if (opcode >= 0x70 && opcode <= 0x7F) {
        return fanhuibian70to7F(erjinzhidata, outhuibian, curjizhi);
    }
    sprintf(outhuibian, "暂不支持的反汇编code:%x,反汇编结束 \n", opcode);
    return -1;
}

int apifanhuibianonebyteopcode81tobf(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];
    if (opcode >= 0x81 && opcode <= 0x8F) {
        return fanhuibian8x(erjinzhidata, outhuibian);
    }
    if (opcode >= 0x90 && opcode <= 0x97) {
        return fanhuibian90to97(erjinzhidata, outhuibian);
    }
    if (opcode >= 0xb0 && opcode <= 0xbf) {
        return fanhuibianB0toBf(erjinzhidata, outhuibian);
    }
    sprintf(outhuibian, "暂不支持的反汇编code:%x,反汇编结束 \n", opcode);
    return -1;
}

int apifanhuibianonebyteopcodec0toff(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];
    if (opcode >= 0xC0 && opcode <= 0xCF) {
        return fanhuibiancx(erjinzhidata, outhuibian);
    }
    if (opcode >= 0xe0 && opcode <= 0xeF) {
        return fanhuibianEx(erjinzhidata, outhuibian, curjizhi);
    }
    sprintf(outhuibian, "暂不支持的反汇编code:%x,反汇编结束 \n", opcode);
    return -1;
}

int apifanhuibianonebyteopcodechose(char* erjinzhidata, char* outhuibian,int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];

    if (opcode >= 0x40 && opcode <= 0x7f) {
        return apifanhuibianonebyteopcode40to7f(erjinzhidata, outhuibian, curjizhi);
    }
    if (opcode >= 0x81 && opcode <= 0xbf) {
        return apifanhuibianonebyteopcode81tobf(erjinzhidata, outhuibian, curjizhi);
    }
    if (opcode >= 0xc0 && opcode <= 0xff) {
        return apifanhuibianonebyteopcodec0toff(erjinzhidata, outhuibian, curjizhi);
    }
  
    sprintf(outhuibian, "暂不支持的反汇编code:%x,反汇编结束 \n", opcode);
	return -1;
}


int apifanhuibiantwobyteopcodechose(char* erjinzhidata, char* outhuibian, int curjizhi)
{
    unsigned char opcode = erjinzhidata[0];
    if (opcode == 0xf) {
        unsigned char twoopcode = erjinzhidata[1];
        if (twoopcode >= 0x80 && twoopcode <= 0x8F) {
            return twofanhuibian80to8f(erjinzhidata, outhuibian, curjizhi);
        }
    }
    sprintf(outhuibian, "暂不支持的反汇编code:%x,反汇编结束 \n", opcode);
    return -1;
}