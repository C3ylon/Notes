# CPP_NOTES

输入输出流：`cin`,`cout`,`cerr`,`clog`。

***

cpp标准库头文件通常不带后缀。

***

标准输入与标准输出的文件重定向：在程序中使用cin与cout时，若使用命令行`foo.exe <infile >outfile`可以完成从infile文件读取数据并将数据输出到outfile文件。

***

类型`char`和类型`signed char`并不完全等同。`char`实际上表现为带符号或不带符号的形式由编译器决定。因此如果需要在算数表达式中使用`char`，则必须要明确指定是`signed char`还是`unsigned char`。

***

有符号类型数据的溢出是**未定义**的。对于无符号类型数据，如果所赋值超出其表示范围，则其值为所赋值对该无符号类型所能表示数值总数取模后的余数。

> 无符号和有符号类型混合运算时会先把有符号类型转换为无符号类型再进行运算。

***

1. 整型字面值：

    + 十进制字面值对应 `int`, `long`, `long long`中可容纳值的最小宽度类型(默认为带符号数)。
    + 八进制和十六进制字面值对应上述三种类型加上三种`unsigned`类型共六种类型中可容纳值的最小宽度类型(不确定是否带符号)。
    + 类型short没有与之对应的字面值。

2. 浮点型字面值默认为`double`。表现为小数或以科学计数法表示的指数(指数部分用E或e分隔)。

   > 指定字面值的类型:
   >
   > + 整型: 加后缀`U`指定为`unsigned`，加后缀`L`指定可容纳值的最小宽度类型为`long`，加后缀`LL`指定为`long long`。
   > + 浮点型: 加后缀`F`指定为`float`，加后缀`L`指定为`long double`。
   > + 字符型和字符串型: 加前缀`L`指定为`wchart_t`，加前缀`u8`指定编码为`utf-8`，类型仍为`char`。
   >
   >   加前缀`u`指定为`char16_t`表示Unicode 16字符，加前缀`U`指定为`char32_t`表示Unicode 32字符。

***

初始化不等同于赋值。初始化的含义是创建变量时赋予其一个初始值，而赋值的含义是把对象的当前值擦除，以一个新的值来替代。

> 如果定义变量时没有指定初始值，则变量被**默认初始化**，一种例外是定义在函数体内部的内置类型变量将**不被初始化**。

初始化一个int类型变量的四种方式:

1. `int a = 0;`
2. `int a(0);`
3. `int a = { 0 };`
4. `int a { 0 };`

使用等号的都是拷贝初始化，未使用等号的都是直接初始化。

第三种是*Copy-list-initialization*，第四种是*Direct-list-initialization*，都是C++11引入的新的初始化形式，默认为列表初始化，当初始值存在丢失信息的风险则编译器会报错。比如`int a = { 3.14 };` 和`int a { 3.14 }` 都会报错，而换用前两种则不会。

如果初始化时采用了大括号的形式，但是提供的值又不满足列表初始化的条件，编译器会按照第二种初始化的形式来处理。

在声明类的成员变量并为其指定默认值时不能使用小括号初始化的方式。

***

全局作用域本身没有名字，当作用域操作符的左侧为空时即为向全局作用域请求获取作用域操作符右侧名字对应的变量。

***

对于全局作用域的const对象，仅在该文件内有效。如果想在多个文件之间共享const对象，需要在变量的定义之前加上extern关键字。比如：

```C++
// foo1.h 该处为声明
extern const int a;
// foo1.cpp 该处为定义
extern const int a = 1;
```

> C语言中const对象在所有文件内都有效。

***

```C++
double a = 3.14;
const int &b = a;
```

此处的const引用在汇编层面会创建一个临时变量，并将a的值转化为b变量对应的类型之后存放在临时变量的位置。故，b若为普通引用则从逻辑上来说要修改的话会修改到临时变量的值，而不会修改到a的值，因此编译器会报错，b必须为const引用。

```C++
const int &a = 3.14;
// 正确。const引用会创建temp副本，可以直接绑定引用。
```

const引用类型的变量如果绑定的不是字面量，而是另一个变量名，则当绑定对象的值改变时该const引用变量的值也会随之改变。(此处const只能保证不能通过该变量名写入，不能保证通过该变量名读取时值不变。类比于普通变量，如果为const类型且没有`volatile`限定符的话，即使在两次读取操作之间可能存在写入操作，经过编译器优化之后两次读取的值通常是不变的)

***

指针中的顶层const和底层const:

**top-level const**表示指针本身是常量，**low-level const**表示指针所指的对象是常量。

> 常量引用都是底层const。

***

C++11开始加入限定符`constexpr`，在认定某变量是常量表达式时才可使用该限定符，否则编译器会报错。

```C++
constexpr int a = 10;
constexpr int b = a + 1;
// 以上变量a, b都是常量表达式
constexpr int c = get_c();
// 仅当函数get_c()是一个constexpr函数时才是一条正确的语句
```

`constexpr`会将所定义的 *对象* 置为顶层const (具有引用类型的某个变量不被视作一个对象，因此`constexpr`不能用于引用变量的声明)。对于指针而言，其所指的对象是否为常量并不由指针本身的限定符`constexpr`所决定。

```C++
constexpr int *p = nullptr;
// p是一个常量指针，指向int
constexpr const int *q = nullptr;
// q是一个常量指针，指向const int
```

***

auto关键字:

+ auto用于声明指针变量时忽略顶层const，保留底层const，`const auto`中的const作用于顶层，即声明的指针本身。
+ auto用于声明引用时保留底层const，`const auto`中的const用于表示常量引用。
+ 一个auto语句里同时声明多个变量时需要保证这些变量基础类型一致。(比如`auto a = 10, const b = 1;` 就是一种错误的声明方式，因为`int`和`const int`不属于同一种基础类型)

> ```C++
> const int arr[3][4];
> auto a = arr; // a类型为const int (*)[4]
> const auto b = arr; // b类型为const int (* const)[4]
>
> const int i = 1;
> auto &j = i;
> const auto &k = i;
> // 此处j和k的类型都是一样的，都是const int
>
> const int a = 1;
> auto b = 2, c = &a;
> // 在c处报错 : 'auto' type is "const int *" for this entity, but was previously implied to be "int"
> ```

***

与`auto`一样，`decltype`也是C++11新引入的可推导类型的关键字，但是特性略有区别。

1. `decltype`会保留顶层const。
  
   > ```C++
   > const int *const a = nullptr;
   > auto b = a; // b的类型是 const int*
   > decltype(a) c = nullptr; // c的类型是 const int* const
   > ```

2. 对于引用类型的变量，`decltype`得到的类型为该引用类型。对于引用类型的变量参与运算的表达式，`decltype`得到的类型为表达式结果的类型。
  
   > ```C++
   > const int i, &r = i;
   > decltype(r) a = i; // a的类型为 const int&
   > decltype(r + 0) b; // b的类型为 int
   > // 注意表达式运算之后失去了const限定符
   > ```

3. 若表达式得到的结果可为左值，则`decltype`得到的类型都为引用。

   + 如果表达式的内容是解引用操作，`decltype`得到的类型为引用类型。

     > ```C++
     > const int i = 0, *p = &i;
     > decltype(*p) = i; // a的类型为 const int&
     > ```

   + 变量名两边加上括号可以看作一个特殊的表达式，该表达式可以作为左值。

     > ```C++
     > const int i = 0;
     > decltype(i) a = i; // a的类型是 const int
     > decltype((i)) b = i; // b的类型是 const int&
     > // decltype((variable)) 的结果永远是引用。
     > ```

4. `decltype`对数组变量名得到的类型仍为数组，而`auto`对数组变量名得到的类型为指针。(`auto`加上引用的形式可以确保得到的类型仍为数组)

   > ```C++
   > const int a[5] = {};
   > auto b(a); // b的类型为 const int*
   > decltype(a) c = {}; // c的类型为 const int [5]
   > auto &d(a); // d的类型为 const int (&)[5]
   > ```

***

```C++
const int a[3][4];
auto b = a[3];
auto &c = a[3];
// b的类型为 const int*
// c的类型为 const int (&)[4]
```

由上可知，使用范围for语句处理多维数组时，至少要保证除了最内层循环以外的其他所有循环变量部分使用引用类型。如下:

```C++
const int a[3][4] = {};
for(auto &i : a)
    for(auto j : i) {
    }
// 此处如果i不声明为引用的形式则i的类型为 const int*，在下一层循环时遍历i会报错
```

***

```C++
string line;
while(getline(std::cin, line)) {
    std::cout << line << endl;
}
```

每次调用`getline`时从输入流中读取内容直到换行符为止，但是该换行符不会存入到string对象中去。

***

创建自定义类型数组的时候，每个数组成员都会被默认初始化。

***

求值顺序、优先级、结合性是三个不同的概念。

> 三目运算符`? :`结合性是R to L。
>
> ```C++
> constexpr int a = 1 ? 0 : 1 ? 0 : 1; // a的值为0
> constexpr int b = (1 ? 0 : 1) ? 0 : 1; // b的值为1
> ```

形如`f() + g() * h() + j()`的表达式，不能确保四个函数调用的求值顺序(即使乘法优先级比加法更高，也不能确保会先进行g()和h()函数调用)。

明确规定了求值顺序的四种运算符: `&&`，`||`，`: ?`，`,`。

***

取模运算: 设m, n都为正数，则`(-m)%n = -(m%n)`, `m%(-n) = m%n`

***

`true`和`false`如果与非布尔类型的值相比较则会被分别转化为1和0来进行比较。比如`2 == true`的结果就是`false`。

***

复合赋值运算符(`+=`等)左侧运算对象只求取一次值，而普通的赋值运算符(`=`)会求取两次值。如下:

```C++
#include <stdio.h>

int i = 1;

int &func() {
    printf("i : %d\n", i);
    return i;
}

int main() {
    func() = func() + 1;
    // 显示两次1
    func() += 1;
    // 显示2
    return 0;
}
```

***
