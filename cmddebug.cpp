#include "cmddebug.h"
#include <conio.h>
#include <Windows.h>
CmdDebug::CmdDebug()
{

}


// 在MFC 中打开控制台
// 引用头文件io.h 和 fcntl.h
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

// 初始化控制台窗口
void InitConsoleWindow()
{
    #if 1
    AllocConsole();
        freopen("CONOUT$", "w+t", stdout);
    #else
    AllocConsole();
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    int hCrt = _open_osfhandle((long)handle, _O_TEXT);
    FILE * hf = _fdopen(hCrt, "w");
    *stdout = *hf;
#endif
}


