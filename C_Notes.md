# C NOTES
```C
VirtualProtectEx(hProcess, (LPVOID)addr, 1024, 4, &oldprotect);
printf("1 : %d\n", oldprotect);
//  WriteProcessMemory(hProcess, (LPCVOID)addr, buff, 4, 0);
VirtualProtectEx(hProcess, (LPVOID)addr, 1024, oldprotect, &oldprotect);
printf("2 : %d\n", oldprotect);
VirtualProtectEx(hProcess, (LPVOID)addr, 1024, 4, &oldprotect);
printf("3 : %d\n", oldprotect);
VirtualProtectEx(hProcess, (LPVOID)addr, 1024, oldprotect, &oldprotect);
printf("4 : %d\n", oldprotect);
```

VirtualProtectEx函数的最后一个参数切记不能填**0**，不然修改内存页属性时会出错。如果注释`WriteProcessMemory`函数之后输出为2，8，2，8。如果取消`WriteProcessMemory`函数的注释之后输出为2，4，2，4。结果很奇怪。

完整测试函数为：

```C
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>

int main(int argc, char* argv[], char* envp[])
{
	DWORD pid;
	HWND hWnd =  FindWindowW(NULL, L"Tutorial-i386");// Cheat Engine教程程序
	GetWindowThreadProcessId(hWnd, &pid);
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, NULL, pid);
	DWORD addr = 0x400000;
	DWORD oldprotect = NULL;
	unsigned char buff[4] = { 0 };

	VirtualProtectEx(hProcess, (LPVOID)addr, 1024, 4, &oldprotect);
	printf("1 : %d\n", oldprotect);
	// WriteProcessMemory(hProcess, (LPCVOID)addr, buff, 4, 0);
	VirtualProtectEx(hProcess, (LPVOID)addr, 1024, oldprotect, &oldprotect);
	printf("2 : %d\n", oldprotect);
	VirtualProtectEx(hProcess, (LPVOID)addr, 1024, 4, &oldprotect);
	printf("3 : %d\n", oldprotect);
	VirtualProtectEx(hProcess, (LPVOID)addr, 1024, oldprotect, &oldprotect);
	printf("4 : %d\n", oldprotect);
	return 0;
}
```

---

printf函数里面，`\n`是将光标移至下一行，`\r`是将光标移至本行行首，`\b`是将光标前移一位（`\b`不能再回退至上一行）。有输出之后会覆盖掉光标后面的内容。比如`printf("123456\b\b\bab"); `的输出就是123ab6。

---
字节模型: 
+ win32 ILP32模型（即int，long，pointer都为4字节）
+ win64 LLP64模型（即long long，pointer为8字节，long还是为4字节）
+ 类unix系统64位 LP64模型（即long，pointer都为8字节）

| DataType | ILP32 | LP32 | ILP64 | LP64 | LLP64 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| 平台 | Win32 API/Unix和Unix类的系统（Linux, Mac OS X) | Win16 API | Win16 API | Unix和Unix类的系统（Linux, Mac OS X) | Win64 API |

