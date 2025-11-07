
// mywin32apichengxu.cpp : 定义应用程序的入口点。
//

//#include "framework.h"
#include "mywin32apichengxu.h"
#include <windows.h>
#include <stdlib.h>
#include "消息处理模块/消息处理函数.h"

#define MAX_LOADSTRING 100
_In_ HINSTANCE hAppInstance;



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	hAppInstance = hInstance;
	INT_PTR dialogResult = DialogBox(
		hInstance,  // handle to module					
		MAKEINTRESOURCE(IDD_DIALOG1_main),   // dialog box template					
		NULL,      // handle to owner window					
		mainfun  // dialog box procedure					
	);

  
}





