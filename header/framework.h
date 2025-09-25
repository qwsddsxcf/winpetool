// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
// Windows 头文件
#include <windows.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <psapi.h>
#include <stdint.h>
#include <string>
#include <tlhelp32.h>
#include "tool.h"
#include "文件操作.h"
#include "注入代码.h"
#include "新增区段.h"
#include "导出表操作.h"
#include "重定位表解析.h"
#include "重定位表操作.h"
#include "导入表解析.h"
#include "导入表操作.h"
#include "资源表解析.h"
#include "导出表解析.h"
#include "ui初始化.h"
#include "进程信息展示.h"
#include "消息处理函数.h"
#include "pe功能总控.h"
#include "pe基础信息展示.h"
#include "resource.h"
#include<commctrl.h>
#include <commdlg.h>
