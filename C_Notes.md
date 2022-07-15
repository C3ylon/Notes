# **C NOTES**

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

***

printf函数里面，`\n`是将光标移至下一行，`\r`是将光标移至本行行首，`\b`是将光标前移一位（`\b`不能再回退至上一行）。有输出之后会覆盖掉光标后面的内容。比如`printf("123456\b\b\bab"); `的输出就是123ab6。

***

字节模型: 
+ win32 ILP32模型（即int，long，pointer都为4字节）
+ win64 LLP64模型（即long long，pointer为8字节，long还是为4字节）
+ 类unix系统64位 LP64模型（即long，pointer都为8字节）

| Data Type | ILP32 | LP32 | ILP64 | LP64 | LLP64 |
| :---: | :---: | :---: | :---: | :---: | :---: |
| 宏定义 | - | - | - | \_\_LP64\_\_ | \_\_LLP64\_\_ |
| 平台 | Win32 API / Unix和Unix类的系统（Linux, Mac OS X) | Win16 API | \\ | Unix和Unix类的系统（Linux, Mac OS X) | Win64 API |
| char | 8 | 8 | 8 | 8 | 8 |
| short | 16 | 16 | 16 | 16 | 16 |
| int | 32 | 16 | 64 | 32 | 32 |
| long | 32 | 32 | 64 | 64 | 32 |
| long long | 64 | 64 | 64 | 64 | 64 |
| pointer | 32 | 32 | 64 | 64 | 64 |

***

32位天堂之门：

```C
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <Windows.h>
typedef unsigned long long QWORD;
int main(int argc, char* argv[], char* envp[])
{
    unsigned char func[] =
    {
        0x55, 0x89, 0xE5, 0x81, 0xE4, 0xF0, 0xFF, 0xFF, 0xFF,
        //================进入x64==========================
        0x6A, 0x33,                                         //push 0x33
        0xE8, 0x00, 0x00, 0x00, 0x00,                       //call $0
        0x83, 0x04, 0x24, 0x05,                             //add dowrd ptr [esp], 5
        0xCB,                                               //retf
        //================减法函数=========================
        0x53, 0x48, 0x8B, 0x45, 0x08, 0x48, 0x8B, 0x5D, 0x10,
        0x48, 0x2B, 0xC3, 0x5B, 0x50,
        //================退出x64==========================
        0xE8, 0x00, 0x00, 0x00, 0x00,                       //call $0
        0xC7, 0x44, 0x24, 0x04, 0x23, 0x00, 0x00, 0x00,     //mov dword ptr [rsp + 4], 0x23
        0x83, 0x04, 0x24, 0x0D,	                            //add dword ptr [rsp], 0xD 
        0xCB,                                               //retf
        //================进入x32==========================
        0x58, 0x5A,
        0x89, 0xEC, 0x5D, 0xC3
    };
    DWORD oldpro = 0;
    VirtualProtect((LPVOID)func, 1, PAGE_EXECUTE_READWRITE, &oldpro);
    QWORD ans = ((QWORD(*)(QWORD, QWORD))func)(0x23456789ABCDEFFF, 0x1111111111111111);
    printf("%llx\n", ans);
    VirtualProtect((LPVOID)func, 1, oldpro, &oldpro);
    return 0;
}
```

***

位移操作(shl, shr, sar)有操作范围, 不论是32位程序还是64位程序，位移范围都在一字节内，即如shr eax, cl。

***

GCC 编码格式及编码转换：

编码场景：

+ 源码文件的编码格式

    `-finput-charset` 该选项指定GBK，UTF-8等编码格式后由GCC调用host的字符转换功能将源文件的编码格式转换为GCC内部编码格式然后再进行处理。

+ 源码中字符在目标文件中的编码

    可以理解为在源码中定义的字符串在内存中（当然是先编译进目标文件的）是如何被表示的。又要分为两类:窄字符和宽字符。

    + `-fexec-charset`

        即常用的 char 类型字符，每个字符占用1个字节，例如：char str = "字符串"。

    + `-fwide-exec-charset`

        即 wchar_t 类型字符，每个字符占用2个字节，例如：wchar_t wstr = L"字符串"。

> `-finput`和`-fexec`两个选项可以理解为vscode编码选项中的`Reopen with Encoding `和`Save with Encoding `。
>
> 即`-finput`后指定的编码格式要与源码文件原本的编码格式保持一致，`-fexec`后指定的编码格式是希望储存在编译出的程序二进制文件中的编码格式。

***

添加<Python.h>时遇到的坑：gcc后加上`-I xxxx\Python\Python310\include` 指令，用以明确路径。

***

```C
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

unsigned char buff[1024];
unsigned char tmp[6] = { 0x41, 0x42, 0x43, 0x44, 0x45, 0x46 };
int main()
{
    FILE *fp = fopen("test.txt", "rb+");
    if(fp)
    {
        printf("fp start addr: %p\n", _ftelli64(fp));
        fwrite(tmp, 1, 1, fp);
        //_fseeki64(fp, 0, SEEK_CUR);
        fread(buff, 1, 1, fp);
        fclose(fp);
    }
}
```

此处如果没有`fseek()`来重置某个标志，`fwrite`之后再`fread`的话，`fread`就会清除掉文件中对应大小的内容(就像是用"wb"来打开某个文件)。而如果是`fread`之后再`fwrite`的话，会出现`fwrite`无法成功写入的情况，但是会正确返回预计写入的大小，并且`ftell()`查看指针位置也会发现指针后移了对应的距离(就像是用"rb"来打开某个文件)。尚不清楚具体的原理，需要进一步的逆向分析。

***

mingw创建dll时声明可以在不同进程中共享同一内存地址的变量的方法: 

```C
HMODULE ghModule __attribute__((section(".shr"), shared));
HHOOK ghHook __attribute__((section(".shr"), shared));
int gStatus __attribute__((section(".shr"), shared));
```

即用`__attribute__((section(".shr"), shared))` 跟在变量名后面来修饰。
如果用vs平台编译可以用：

```C
#pragma data_seg(".shr")
int gData;
#pragma data_seg()
#pragma comment(linker,"/SECTION:.shr,RWS")
```

***

调用API函数时一定要注意[out]属性的指针所对应变量的字节宽度。比如

```C
BOOL ReadProcessMemory(
  [in]  HANDLE  hProcess,
  [in]  LPCVOID lpBaseAddress,
  [out] LPVOID  lpBuffer,
  [in]  SIZE_T  nSize,
  [out] SIZE_T  *lpNumberOfBytesRead
);
```

对于该函数来说，如果第五个参数传入了一个int类型的变量的指针，就会导致该变量所在内存的后四字节被覆盖(64位程序中)，编译器如果在这后四字节分配了其他的变量那么将会导致不可预估的后果。

***

对gcc编译器来说，

`(unsigned int)0xffffffff == (int)-1`

但是

`(unsigned char)0xff != (char)-1`

```
00000000004015A2             | mov eax,dword ptr ss:[rbp-4]
00000000004015A5             | movzx edx,al                 unsigned char
00000000004015A8             | mov eax,dword ptr ss:[rbp-8]
00000000004015AB             | movsx eax,al                 signed char
00000000004015AE             | cmp edx,eax
```

所以在使用字节宽度为1的变量时要注意其是否为**unsigned**。

***

若在a.c文件中定义 `char c[10]; `在b.c文件中声明 `extern char *c; `则会出现定义与声明矛盾的错误。因为: 假设声明正确，则在a.c中执行`printf("%p\n", c); `会直接打印出标识符c所在的地址，而在b.c中执行`printf("%p\n", c); `则会打印出标识符c所在地址与一个指针长度相等的内存单元的内容，两者相悖。故在b.c中应声明为`extern char c[]; `或`extern char c[10] `。

***

```C
#include <stdio.h>

int main()
{
    int a = 123;
    int *b = &a;
    int(*c)[] = (void*)&b;
    printf("%p %p %d %d\n", c, *c, **(int**)*c, **(int**)c);
    return 0;
}
```

这段代码中`c`和`*c`的值是一样的，`**(int**)*c`和`**(int**)c`才会打印出a的值。因为c解引用之后类型为`int []`，是一个数组，c解引用后的值为数组的地址值而不会再从该地址的内存单元取值，因此c如果在第一次解引用时不转换类型则需要3次解引用才能真正取到a的值。

***

struct结构体的标签在单独的命名空间内。比如定义 `struct tag{…}; `再定义 `typedef int tag; `这样并不冲突。`struct tag a` 和 `tag b` 依然可以正确表示两种不同类型的变量。

***

`const`关键字在`*`右边表示指针本身是常量，在`*`左边表示指针所指的对象是常量。比如`char * const *a; `表示指针`a`所指的对象是常量，也即`*a`这个指针本身是常量。

***

C语言链接属性有三种，`external`，`internal`，`none`。全局变量和函数声明的链接属性默认为`external`，其余皆为`none`。

改变默认链接属性的方式有两种：

1. `extern`作用于默认链接属性为`none`的对象上，可以将链接属性改为`external`（若对象链接属性已经为`internal`，则再用`extern`声明不会再改变对象的链接属性）

2. `static`作用于默认链接属性为`external`的对象上，可以将链接属性改为`internal`（若作用于默认链接属性为`none`的对象上则不会更改链接属性，而是将对象的储存类型更改为静态变量）。

***

C语言变量作用域：

1. 代码块作用域（即局部变量）;
2. 文件作用域（即全局变量）;
3. 原型作用域（函数声明中的变量名不能重复）;
4. 函数作用域（同一个函数内的goto跳转标签名不能重复）。

***

几个比较绕的定义：

```C
int *(*(**c[10])(int**))(int, short);
int (*func(char a, short b))(int,long)
{
    void;
}
int (*f(char a, short b))[3]
{
    void;
}
```

+ `c`是一个数组，数组的成员是指向一个对象的指针的指针，该对象是一个函数，接受一个`int**`类型的参数，返回值是一个指针，该指针指向一个接受`int`和`short`两个参数且返回值为`int*`的函数。
+ `func`是一个函数，接受`short a`和`short b`两个参数，返回值是一个指针，该指针指向一个接受`int`和`long`两个参数且返回值为`int`的函数。
+ `f`是一个函数，接受`char a`和`short b`两个参数，返回值是一个指针，该指针指向一个`int [3]`类型的数组。

由以上推出，指针的星号最好和变量名靠在一起写。

***

c实现默认参数的一种宏定义方式: 
```C
#include <stdio.h>

#define TESTARG(a1, a2) ((#a1[0])?(a1+0):a2)

int main()
{
    int a = TESTARG(,3);
    printf("%d\n", a);
    return 0;
}
```

***

```C
#include <stdio.h>

int main(int argc, char *argv[])
{
    int a[4][3];
    printf("%d\n", (int)(a[3] - a[1]));
    return 0;
}
```

注意该段代码输出的值是6而不是2。`a[1]`和`a[3]`两个地址的差值除以的量不是`3 * sizeof(int)`而是`sizeof(int)`。因此`a[3] - &a[1][2]`不会出现语义上的错误。`a[3]`的类型是`int [3]`即`int []`，`&a[1][2]`的类型是`int *`,两个类型兼容。

***

结构体中定义一个长度为零的数组用处是alloc结构体内存时可以自定义数组的长度，即实现结构体内的变长数组。

```C
struct node {
    int a;
    char b;
    char c[]; // 或char c[0];
}
```

若`a`地址为0，则`b`地址为4，`c`地址为5(注意若`c`的类型为`short`则仍会受到内存对齐的影响，即`c`的相对地址为a+6)。`sizeof(struct node)`的值为8。

在结构体中除了可变数组之外的最后一个变量无多余内存对齐时分配内存可以使用:

```C
struct node *p = malloc(sizeof(struct node) + sizeof_arr); // sizeof_arr即此处需要定义的可变数组的大小
```

***

C语言里整除为绝对值的向下取整，取余的计算公式是`x % y = x - ((x / y) * y)`。比如`10 % -3 = 1`。

> Python的整除为向下取整。取余公式同上，但相对应的是`10 % -3 = -2`。

***

C中的宏分为两类: 

+ 对象宏 `object-like macro`: 
    
    ```C
    #define 宏名 替换列表 换行符
    ```

+ 函数宏 `function-like macro`: 

    ```C
    #define 宏名([标识符列表]) 替换列表 换行符
    // 注意宏名和表示参数的括号之间不能有空白符，否则会把函数宏误判为对象宏
    // 另注：
    // #define f\
    // (a) printf("[+]"#a"\n");
    // 以上换行方式是合法的，需要反斜杠另起的下一行起始不为空白符。
    ```

宏的操作符: 

+ Stringizing操作符 `#`

    > 注意点: 
    >
    > 1. 字符串化操作符只能对函数宏的参数使用。
    > 2. 参数前后的空白符都会被忽略，中间的空白符会被压缩为一个，注释会被忽略并变成一个空白符。

+ Concatenation操作符 `##`

`...`用来接收任意数量的参数，`__VA_ARGS__`用来代替前者接收的参数。如: 

```C
#define f(a, ...) a __VA_ARGS__
f(1, 2, 3, 4)

// 1 2, 3, 4
```

***

## 宏的展开

### 一、对象宏的展开

当展开全程仅有对象宏时是最简单的情况，需要满足以下两点: 

1. 对象宏的展开是**深度优先**的递归展开。
2. 在展开过程中产生的宏名称不一定都可以展开。
    > 具体来说即是首先建立一个初始为空集的限定集，然后在递归展开的过程中: 
    >
    > + 向下层展开时: (*展开后*)把展开的宏名称加入到限定集中
    > + 向上层返回时: (*返回后*)把当前层的宏名称从限定集中移除
    >
    > 在展开时不去展开限定集中的宏名称

```C
#define a b
#define b c
#define c d
#define d b c end

a
// b c end
```

### 二、函数宏的展开

当展开过程中出现函数宏时，相对于展开全过程中只有对象宏的情况处理思路大致相同，但是还有几点需要特殊处理的地方: 

1. 不能展开的宏名称
    > + 当函数宏的参数连接着`#`(*Stringizing*)或连接着`##`(*Concatenation*)时不能展开该位置的参数
    > + 当函数宏名称未连接着括号时不能展开该宏名称
    >
    > ```C
    > #define f(x) g x #x FOO_##x
    > #define g() sth
    > #define A expanded
    >
    > f(A)
    > // g expanded "A" FOO_A
    > ```
2. 参数的展开时机
    > 先将所有参数原封不动替换到函数宏表达式中的各个位置，然后再深度优先递归展开所有参数(不去展开连接着`#`或`##`的参数)，待参数全部完全展开之后，该函数宏才算向下一层展开完毕。此时在限定集中加入该函数宏的宏名称，然后开始下一层的展开
    >
    > ```C
    > #define f(x) g(x)
    > #define g(x) FOO_##x
    > #define A expanded
    >
    > f(A)
    > // FOO_expanded
    > ```
3. 参数的限定集选取
    > 参数的限定集即为该函数宏展开前的限定集
    >
    > ```C
    > #define f(x) x #x
    >
    > f(f(f(x)))
    > // x "x" "f(x)" "f(f(x))"
    > /*=====================*/
    > #define f(x) g(f(0),g(1, 2)) x
    > #define g(x, y) x y
    >
    > f(3)
    >// f(0) 1 2 3
    > ```
4. 如果某支路完全展开后的最后一个对象为不带括号的函数宏名称，那么如何处理这种情况
    > + 若该函数名称在限定集中，则将会被标记为不被展开。如果可以和接下来的括号再结合为一个函数宏，那么仍然不会展开该函数宏
    > + 若该函数名称未在限定集中，则将会被标记为可能能够展开。如果可以和接下来的括号再结合为一个函数宏，则新结合形成的函数宏限定集为其括号所在那一层的限定集，如果满足宏名称不在这个限定集中那么将展开此函数宏
    > ```C
    > #define f() f2
    > #define f2() f
    >
    > f()()()
    > // f2
    > /*=====================*/
    > #define f(x) g() x()
    > #define g() z()
    > #define z() g
    >
    > f(g())
    > // g g()
    > ```

黑魔法 宏的延迟展开

```C
#define EMPTY
#define f(func) func EMPTY
#define g() expanded
f(g)()

// g ()
// 若要完整解析，则需要再套一层函数宏，使得"f(g)()"的展开为某函数宏参数的展开
#define EMPTY
#define f(func) func EMPTY
#define g() expanded
#define EXPAND(a) a
EXPAND(f(g)())

// expanded
```

几个展开示例: 

```C
#define FOO_ 0
#define FOO_1 1
#define CAT(x, y) x ## y
CAT(FOO_, 1)

-> 1
//********************
#define FOO_ 0
#define FOO_1 1
#define PRIMITIVE_CAT(x, y) x ## y
#define CAT(x, y) PRIMITIVE_CAT(x, y)
CAT(FOO_, 1)

-> 01
//********************
#define f(a) a #a
#define ax MOV
f(ax)

-> MOV "ax"
//********************
#define f(a) a #a
f(f(a))

-> a "a" "f(a)"
//********************
#define f() ff
#define ff() aa
f()()

-> aa
//********************
#define f() f
f()()

-> f()
//********************
#define f() a()
#define a() b()
#define b() c() d()
#define c() a() b()
f()

-> a() b() d()
//********************
#define f(x) x a
#define a(x) x b
#define b(x) x c
#define c(x) a(x) b(x) a
f(i)(j)(k)(l)(m)

-> i j k l b l c m b
//********************
#define FOO_(x) FOO_1(x)
#define FOO_1(x) FOO_2(x) + 1
#define FOO_2(x) FOO_1(x) - 1
#define BAR(x) FOO_ ## x (12) FOO_2
BAR(1) (5)
//********************
-> FOO_1(12) - 1 + 1 FOO_2(5) + 1 - 1
#define cat(a,b) a ## b
#define xcat(x, y) cat(x, y)
xcat(xcat(1, 2), 3)

-> 123
//********************
#define f a b
#define a g
#define b ()
#define g() expanded
#define h(a) a
h(f)

-> expanded
```

***

在声明结构体时: 

```C
struct tag_a {
    int a;
    float b;
    struct tag_b {
        short c;
        char d;
    };
};
```

若定义`struct tag_a a = { 0 };`则用`a.c`可以直接访问到`struct tag_b`中的成员

```C
struct tag_a {
    int a;
    float b;
    struct tag_b {
        short c;
        char d;
    } bb;
};
```

若定义`struct tag_a a = { 0 };`此时必须用`a.bb.c`才可以访问到`struct tag_b`中的成员

```

***
