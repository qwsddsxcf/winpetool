#pragma once
#include<windows.h>
int fabnhuibianmain(int jizhi,char* erjinzhidata,int size, HWND dadjubing, int id);
int apifanhuibianchoseopcodesizefun(char* erjinzhidata, char* outhuibian, int curjizhi);

int apifanhuibianonebyteopcodechose(char* erjinzhidata,char* outhuibian, int curjizhi);
int apifanhuibianonebyteopcode40to7f(char* erjinzhidata, char* outhuibian, int curjizhi);
int apifanhuibianonebyteopcode80tobf(char* erjinzhidata, char* outhuibian, int curjizhi);
int apifanhuibianonebyteopcodec0toff(char* erjinzhidata, char* outhuibian, int curjizhi);



int apifanhuibiantwobyteopcodechose(char* erjinzhidata, char* outhuibian, int curjizhi);

