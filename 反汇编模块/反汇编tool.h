#pragma once
int toolgettrue32byte(char* erjinzhidata, char* factzijie, int index);
int toolgetmovadd(char* erjinzhidata, char* outzijie, int index);
int toolgetmoverjistr(char* erjinzhidata, char* outzijie, unsigned char beforemod);
int toolgetnummovetrue(int zhishu);
int toolcheckkeyjuedingreg(const char**& myreg32shunxu,int value);
int toolcheckkeyjuedingjmpaddresswei(int &value,int ret);
int toolreplacestrforduanreg(char* srcstr, char* valuestr);
int toolreplaceanystr(char* srcstr, char* deletestr, int strlength, char* valuestr);
int toolClearAfterChar(char* srcstr, char delimiter);

int toolcheckisduanopcode(unsigned char opcode);
int toolreplacestrifisduanopcode(int num,char* outstr);
int toolcheckisnullmeanopcode(unsigned char opcode);

int toolgetperukoupianyi(char* memdata, int& pianyi, int& base);

int toolcheckisyunsuanopcode(unsigned char opcode);
