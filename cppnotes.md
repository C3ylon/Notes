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

> 如果定义变量时没有指定初始值，则变量被**默认初始化**，定义在函数体内部的内置类型变量和类的内置类型且类内未提供初始值的成员将**不被初始化**。

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
const int a = 1;
// foo2.cpp包含foo1.h，foo2可以使用变量名a
// 或者也可直接定义 extern const int a = 1;
// 上行定义在C中会报警告: 'a' initialized and declared 'extern'，但是对于C++来说这么使用没有问题
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

`constexpr`修饰的函数也可以不返回一个常量值。当`constexpr`修饰的函数用在需要常量表达式的上下文中时，由编译器负责检查该函数是否返回一个常量值。比如:

```C++
constexpr int func(int a) {
    return a * 3;
}
// 当调用该函数时，传入的实参为常数则返回常量值，该函数会被内联为一段直接得到值的汇编代码。
// 若传入的实参不为常数，则汇编代码类似普通的函数调用，返回的值也不为常量值。
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
> // 此处j和k的类型都是一样的，都是const int &
>
> const int a = 1;
> auto b = 2, &c = a;
> // 在c处报错 : 'auto' type is "const int" for this entity, but was previously implied to be "int"
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
     > decltype(*p) a = i; // a的类型为 const int&
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

C++和C不同之处:

```C++
// 以下为在C++中的用法，都正确
void func1(int ...) { }
void func2( ... ) { }
```

1. C++在定义函数时可以有匿名形参，而C不能。(unnamed prototyped parameters not allowed when body is present)
2. C++在表示可变参数的`...`前可以没有参数，而C至少要有一个参数。
3. C++在表示可变参数的`...`前可以没有逗号，而C必须要有逗号。

在C中: `struct`和`union`里可以用一个语句同时定义一个类型名(`enum`，`struct`，`union`)并用该类型声明一个变量，定义的变量名只在结构或联合里可见，但定义的类型名可见范围与该结构或联合本身的可见范围相同。而在C++中: 嵌套的类型名仅在该结构或联合内部可见。

> 为了C++与C的兼容性，尽量不要在`struct`或`union`内部定义新的嵌套结构。

***

```C++
const string &func() {
    return "str";
}
```

在这段函数中有string对象创建和销毁的代码，该处返回等效于`const string tmp = "str"; return tmp;`。因此此处是返回了局部变量的引用，属于未定义行为。

***

重载函数的匹配:

1. 确定候选函数:

   1. 需与调用处的函数同名。
   2. 声明需在调用处可见。(函数声明也可在函数内部，只不过作用域会局限在该函数内部)

2. 确定可行函数:

   1. 形参数量与调用处实参数量匹配。
   2. 每个形参的类型与调用处实参类型相同，或能由实参类型转化得到。

3. 确定最佳匹配。有且只有一个函数满足，否则会报错无匹配函数或二义性调用。

   1. 该函数**每个参数**的匹配都不劣于其他可行函数需要的匹配
   2. **至少有一个参数**的匹配优于其他可行函数需要的匹配

   > 从实参到形参的匹配程度由高至低分为以下五个等级:
   >
   > 1. 精确匹配(实参多或缺顶层const都在该类型)
   > 2. 实参缺底层const
   > 3. 类型提升(比如重载函数有两个，形参类型别是`int`和`short`，实参类型是`char`，由于整型提升因此会匹配形参类型为`int`的函数)
   > 4. 算术类型转换(所有算术类型转换的级别都一样，比如`int`向`unsigned int`的转换并不比`int`向`double`的转换优先级高)
   > 5. 类类型转换

用重载函数对某指针赋值遵循以上匹配规则。

***

`constepxr`函数和定义在类内部的函数都是隐式的`inline`函数。

***

构造函数不能被声明为`const`。当创建一个类的`const`对象时，直到构造函数执行完成，对象才能真正获得`const`属性。

***

没有任何参数的构造函数->**默认构造函数**。

编译器隐式定义的默认构造函数->**合成的默认构造函数**(synthesized default constructor)

***

构造函数初始值列表 (constructor initialize list) : 从构造函数参数右小括号后的冒号起，至代码块左大括号之间的代码。

此时构造函数的形参变量名可以和类成员变量名相同。因为在赋值的时候，小括号外的变量名一定是类成员名，而小括号内的变量名由于作用域屏蔽的关系会定位到构造函数形参的变量名，因此不存在命名冲突。

```C++
class cl {
    int a;
    cl(int a) : a(a) {}
    // 或 : cl(int a) { this->a = a; }
    // 或 : cl(int a) { cl::a = a; }
}
```

上述代码中，`cl(int a) : a(a) {}`与`cl(int a) { this->a = a; }`两种构造函数不能等同。前者直接初始化成员变量，后者先对成员变量默认初始化再进行赋值。示例如下:

```C++
#include<stdio.h>

class cla {
public:
    int a;
    cla() { printf("in default\n"); }
    cla(int a) : a(a) { }
};

class clb {
public:
    cla a;
    clb(cla a) : a(a) { }
    // clb(cla a) { clb::a = a; }
};

int main () {
    clb a(cla(1));
    // 当使用构造函数初始值列表时，不会打印"in default"
    // 当采用先初始化后赋值的方式时，会打印"in default"
    return 0;
}
```

> 如果某一类的成员中有引用或const或不具备默认构造函数的另一类对象，那么在定义该类的构造函数时就不能用先对成员变量默认初始化再进行赋值的方式。
>
> 对于`constexpr`构造函数，也需要用初始值列表来初始化成员变量。
>
> 因此除了效率问题之外，也有必须在某些类的构造函数中使用初始值列表的情况。应当尽量优先使用构造函数初始值列表。

构造函数初始值列表进行初始化的顺序**并不是**按照逗号从左至右的结合性依次初始化，而是与成员变量在类定义中出现的顺序一致。

```C++
class cl {
    int i;
    int j;
public:
    cl(int val) : j(val), i(j) { }
};
// warning: field 'j' will be initialized after field 'i'
// 最好不要随意更改初始值列表变量顺序。
```

如果把某个构造函数中原本是初始值列表的地方替换为调用另一个构造函数，则该构造函数即为C++11新引入的委托构造函数。

> 当一个构造函数委托给另一个构造函数时，受委托的构造函数的初始值列表和**函数体**会被依次执行。

```C++
#include<stdio.h>

class cl {
    int a;
public:
    cl(int a) : a(a) { printf("in 1  "); }
    cl() : cl(1) { printf("in 2  "); }
    cl(float a) : cl() { printf("in 3 : %f %d", a, cl::a); }
};

int main () {
    cl a(3.14f);
    //printf : in 1  in 2  in 3 : 3.140000 1
    return 0;
}
```

***

友元不是类的成员，也不受其所在区域访问说明符的约束(`public`, `private`等)。友元的声明仅仅指定了访问的权限，因此友元函数的声明非一个通常意义上的函数声明。

可以声明为友元的对象:

1. 普通的非成员函数。
2. 其他类。
3. 其他类的成员函数。(需先声明本类的不完全类型，然后声明其他类的成员函数，然后定义本类的完全类型，再在完全类型中声明友元函数，最后再定义该友元函数)

```C++
#include<stdio.h>

class cla;
class clb {
public:
    void pra(cla &a);
};

class cla {
    friend void clb::pra(cla &a);
    int a = 1;
};

void clb::pra(cla &a) {
    printf("%d\n", a.a);
}

int main () {
    cla a;
    clb b;
    b.pra(a);
    return 0;
}
```

某个友元函数的其他重载函数如果没有单独做友元声明则依然无法访问私有成员。

直接定义在类内的非成员友元函数是隐式`inline`，具有外部链接属性。如果提前将友元函数声明为`static`，则具有内部链接属性。

> ```C++
> class cl {
>     friend void func() { }
> };
> static void func();
> // error: 'void func()' was declared 'extern' and later 'static'
> /**************************************************************/
> static void func();
> class cl {
>     friend void func() { }
> };
> // 此时func的链接属性为internal
> ```

友元声明不具有传递性(友元的友元不是友元)，也不会继承(父类的友元不是友元，友元的父类不是友元)

即使在类内直接定义友元函数，也必须单独提供函数声明才能使函数可见。

```C++
#include<stdio.h>

class cl {
    friend void func1() { printf("in func1\n"); }
public:
    void func2();
};

void func1(); // 单独声明，使func1可见。且func1不属于cl域内。
void cl::func2() { func1(); }

int main () {
    cl a;
    a.func2();
    return 0;
}
```

gcc/clang/msvc都有的关于友元的问题：

```C++
#include<stdio.h>

class a {
    friend void func1(const a&) { }
public:
    void func2() { func1(a()); }
};

int main () {
    a a;
    a.func2();
    return 0;
}
// 以上可以成功编译
#include<stdio.h>

class a {
    friend void func1() { }
public:
    void func2() { func1(); }
};

int main () {
    a a;
    a.func2();
    return 0;
}
// 以上不能成功编译
// error: use of undeclared identifier 'func1'
```

***

+ 类内定义的函数->隐式`inline`
+ 类内声明`inline`函数->显式`inline`(无需再在类外定义函数的地方添加`inline`)
+ 类内声明且不带`inline`的函数->能在类外定义为`inline`函数

无需在声明和定义的地方同时说明`inline`。

***

`mutable`修饰的成员变量即使是`const`类对象的成员依然可以修改。

```C++
void SomeClass::Const_mem_func() const {
    ++mutable_var;
}
// 即使是const成员函数也能改变mutable修饰的成员变量
```

`mutable`不能用于修饰引用类型和`const`类型的成员变量。

***

类的成员函数的返回值类型和参数列表的形参类型必须确保在使用前可见，而定义在类内的函数体中用到的类成员变量可以定义在该函数后。

> 需要特别注意的是，如果某个类成员先使用了外层作用域中的某个类型名，则不能再在类中重新定义该类型名。
>
> ```C++
> typedef int a;
> 
> class cl {
> public:
>     a b;
>     typedef int a;
> // warning: declaration of 'typedef int cl::a' changes meaning of 'a'
> // 即使重新定义的类型相同也是不合规则的。应当把typedef语句放在类内最前方。
> };
> ```

***

函数参数若为普通引用则需要接收一个lvalue的实参，因此该实参若是临时变量则会报错。若为`const`引用，则可以接收一个非lvalue的实参，此时可以传递临时变量。

***

只需一个实参就可以调用的构造函数定义一条从该参数类型向类类型隐式转换的规则。

类类型的隐式转换只允许一步。

```C++
#include<stdio.h>
#include<string>

using std::string;

class cla {
public:
    string s;
    int a;
    cla(const string &s, int a = 0) : s(s), a(a) { printf("in cla default\n"); }
    // const引用，可以用临时变量赋值
};

class clb {
    cla a;
public:
    clb (const cla &a) : a(a) { printf("%s\n", this->a.s.c_str()); }
    // const引用，可以用临时变量赋值
};

int main () {
    clb(string("abc"));
    // 注意如果是拷贝初始化 clb tmp = string("abc"); 将报错no viable conversion from 'string' to 'clb'
    clb(cla("def"));
    // clb("ghi"); 错误，类类型隐式转换只允许一步，不能从char *到string再到cla
    clb(static_cast<cla>("jkl"));
    clb((cla)"mn");
    // 以上两种显示转换都转换了两步，从char *到string再到cla
    return 0;
}
```

取消类类型的隐式转换可以将对应构造函数声明为`explicit`，该关键字只对只需一个实参就可以调用的构造函数有效。

只能在类内声明构造函数时使用`explicit`关键字，在类外定义函数时不能重复使用。(类比`inline`, `friend`, `static`)

`explicit`构造函数只能用于直接初始化，不能用于拷贝初始化。(`explicit`拷贝构造函数不影响其他非`explicit`构造函数在拷贝初始化时的调用)

> 拷贝初始化的情况包括:
>
> 1. 使用`=`的初始化
> 2. 非引用类型的函数参数初始化
> 3. 返回值为非引用类型的函数返回时会对临时变量拷贝初始化
> 4. 按值抛出或捕获异常时

尽管`explicit`构造函数不会被用于隐式的类类型转换，但是仍然可以用`static_cast<>`进行显示转换。

***

理解直接初始化和拷贝初始化的一个区别：

比如有A，B，C三种自定义类型，类型B可由类型C隐式转换得到。某类的两个构造函数分别接受A，B两种类型。

在直接初始化时，会依次比较能否按照类型A，B调用构造函数。如果传参是类型C，则由于隐式转换，会调用接受类型B的构造函数。

在拷贝初始化时，构造函数的类型候选集就限定在A，B两种类型中，如果传参类型不符合A，B两种类型则无法调用构造函数，因此无法通过类型C调用构造函数。

若此时用`explicit`修饰接受类型A的构造函数，则在拷贝初始化时会将类型A从构造函数的类型候选集中移除，因此无法通过类型A调用构造函数。而在直接初始化时由于仍会依次比较每个构造函数的参数类型，因此不受影响。

***

类的非`const`静态成员变量不能类内初始化。(如下变量a)

类的`const`静态成员变量可以在类内初始化，也可以在类外定义并初始化。(只能选择一个地方赋初始值，如下变量b和c)

`static`只用于静态成员变量在类定义中的声明，而不用于该变量自身的定义，因此需要在类外定义该变量。(类比于`extern`)

> 类中的`static`对于成员变量相当于只声明不定义，而`friend`对于友元函数相当于(即使有定义也)不声明。

```C++
class cl {
public:
    static int a;
    static const int b;
    static const int c = 3;
    static constexpr int d = 4;
    // constexpr修饰的变量必须在声明时赋一个常量值。
    static const int e = 5;
};

int cl::a = 1;
const int cl::b = 2;
const int cl::c;
constexpr int cl::d;

int main () {
    const int &f= cl::a;
    // const int &g= cl::e; error: undefined reference to `cl::e'
    return 0;
}

```

静态成员可用于普通成员不能用的场景:

+ 静态成员变量可以是不完全类型(比如在类内声明一个该类的静态对象，又如在cl类内声明`static int a[];`，在类外定义`int cl::a[10];`)
+ 静态成员变量可以作为该类成员函数的默认实参

***

与类有关的变量名/函数名/类型名的查找优先级关系:

类外定义类的静态成员变量时，变量类型名可以是类的私有类型名。

类外定义类的成员函数时，函数返回值类型、函数参数类型以及函数大括号内的类型名和变量名，都可以是类的私有名称，且都优先从类域内查找，再从类外查找。

```C++
class cl {
    typedef int int_;
    static int_ init_a() { printf("init a\n"); return 1; }
    static int_ a; // 私有的静态成员变量也可以在类外定义。
};

cl::int_ cl::a = init_a();
// 从类名作用域定义的变量名或函数名开始，这条定义语句的剩余部分都在类的作用域之内，因此可以直接调用类的私有成员函数。
// 即使全局作用域内有init_a()同名函数，此处也会调用cl域内的函数，除非显式说明作用域::init_a()
// int a = cl::init_a(); error: 'init_a' is a private member of 'cl'
/******************************************************************/

class cl {
    using int_ = int;
public:
    int_ func(int_ a);
};

auto cl::func(int_ a) -> int_ { return a; }
// 尾置的返回类型声明也在cl的域内，即使int_是私有的类型名也可以使用。
// cl::int_ cl::func(int_ a) { return a; } 也可行，因为是类成员函数的定义，所以可以访问到私有的类型名
// 此时从形参类型名到大括号内出现的非形参变量名都会优先从类的作用域内查找，然后才在全局作用域查找
// 如果直接用cl::int_在类外定义变量则会报错。
/******************************************************************/

#include<stdio.h>

class cl {
    using int_ = int;
    int_ p = 1;
public:
    int_ func(int_ a);
};

int p = 2;
using int_ = float;

cl::int_ cl::func(int_ a) {
    int_ res = 0; res = a + p; return res;
    // int_和p都优先从cl域内查找。
}

int main () {
    cl a;
    printf("%d\n", a.func(10));
    // 打印11。
    return 0;
}
```

***

有默认参数的函数应该在函数声明时带上默认值，而在函数定义时不带默认值。

```C++
#include<stdio.h>

void func(int a = 1);
void func(int a = 1) {
    printf("%d\n", a);
}
// error: default argument given for parameter 1 of 'void func(int)'
// 正确 : void func(int a) {
int main () {
    func();
    return 0;
}
```

***

`lambda`函数体中如果包含除了`return`之外的任何语句，则默认返回void。若需返回其他类型，需要在形参列表后加上尾置返回类型。

***

`delete []`作用于某类对象的数组指针时会按从后至前的顺序依次析构，最后再释放内存。

***

+ 在一个构造函数中，成员变量的初始化是在函数体执行之前完成的，且按照其在类中定义的顺序进行初始化。
+ 在一个析构函数中，成员变量的销毁是在函数体执行之后完成的，且按照初始化顺序的逆序进行销毁。

***

```C++
class cl {
    int a;
public:
    cl(int a = 0) : a(a) { }
};

cl a(cl(0));  // 无歧义
cl a(cl());   // 存在歧义，cl()可以是匿名创建的临时cl对象
              // 也可以是函数a声明中cl (*) ()类型的形参
cl a((cl())); // 加上括号可消除歧义，此处(cl())明确指代匿名创建的临时cl对象

```

***

模板中`<>`的`>`导致的大于号或右移操作运算符歧义问题

```C++
#include<stdio.h>

template<int N>
class cl {
    int arr[N];
    public:
    void sz() {
        printf("N : %d\n", N);
    }
};

template<typename T>
void fn(T &a) {
    a.sz();
}

template<int N>
void fn2() {
    printf("in fn2 N : %d\n", N);
}

int main() {
    cl<2>a;
    fn<cl<(4>>1)>>(a);
    // fn<cl<4>>1>>(a); 错误，会解析为(fn<cl<4>>) 1>>(a);
    fn2<2<1>();
    fn2<2>=1>();
    fn2<(2>1)>();
    // fn2<2>1>(); 错误，会解析为(fn2<2>) 1>();
    return 0;
}
```

***

在模板定义中模板参数列表不能为空。

***

在实例化函数模板时，可以不必在尖括号内指明全部模板参数值，可以只指明前一部分，然后利用自动推导得出后一部分的值。

> 指明的值称为显式模板实参。

```C++

#include<stdio.h>

template<typename T, T M>
void func(const char(&p)[M]){
    printf("%p\n", static_cast<const void*>(p));
}
int main() {
    func<int>("123");
    // 自动推导出模板参数M的值为4
    return 0;
}
```

***

类模板的成员函数只有在被使用到时才会实例化。这一特性使得即使某一特定类型不能符合该类模板的所有成员函数的操作要求，也可以实例化该类型的类模板，只是不去使用那些不符合操作要求的成员函数即可。

***

函数模板实例的使用优先级与链接顺序有关。

```C++
// 1.cpp
#include<stdio.h>

template<typename T>
void func(T a) {
    printf("1 : %d\n", a);
}

void func();
int main() {
    func(5);
    func();
    return 0;
}
```

```C++
// 2.cpp
#include <stdio.h>

template<typename T>
void func(T a) {
    printf("2 : %d\n", a);
}

void func() {
    func(5);
}
```

如上，使用`g++ .\1.cpp -c`，`g++ .\2.cpp -c`分别得到`1.o`和`2.o`后：

+ 如果使用`g++ .\1.o .\2.o -o .\main.exe`，则程序输出结果为`1 : 5\n1 : 5\n`
+ 如果使用`g++ .\2.o .\1.o -o .\main.exe`，则程序输出结果为`2 : 5\n2 : 5\n`

> C++11中新加入了显式实例化。
>
> 一个显式实例化包含实例化声明和实例化定义。实例化声明形式为`extern template declaration`，实例化定义形式为`template declaration`。
>
> 实例化声明可能有多个，实例化定义有且只有一个。
>
> 如果在1.cpp加入声明`extern template void func(int);`，在2.pp中加入定义`template void func(int);`，则函数模板的实例化采用的是2.cpp中的函数模板。
>
> 执行`g++ .\1.cpp .\2.cpp -o main.exe`，得到的程序输出为`2 : 5\n2 : 5\n`
>
> (或也可声明为`extern template void func<int>(int);`，定义为`template void func<int>(int);`)
>
> 类模板的实例化定义会实例化该类模板的所有成员，因此显式实例化一个类模板时，必须确保类模板的所有成员符合操作要求。

***

在类模板外定义类模板的成员时，需注意从类名作用域定义的变量名或函数名开始才表示剩余部分都在类的作用域之内，此时不用指定模板实参，而在进入类的作用域之前都需要提供模板实参。

```C++
#include<stdio.h>

template<typename T>
class cl {
    public:
    const cl * func() const;
};


// 类外定义类模板成员函数时返回值类型需要提供模板实参
template<typename T>
const cl<T> *cl<T>::func() const {
    return this;
}

int main() {
    cl<int> a;
    printf("%p\n", static_cast<const void*>(a.func()));
    return 0;
}
```

***

C++11开始可以将模板类型参数声明为友元。

```C++
template<typename T>
class cl {
    friend T;
    // friend int;
};
```

> 友元声明的对象通常是函数或类，但是为了能用基础类型实例化上述类，也可以将基础类型作为友元声明的对象。
>
> ```C++
> // non-class friend type 'int' is a C++11 extension
> ```

***

`using`比`typedef`泛用性更广的一点体现在，`using`可以创建类模板的别名，而`typedef`不可以。

```C++
template <typename T, typename U>
class cl {
};

template <typename T>
using clt = cl<T, T>;
clt<int> a;

// template <typename T>
// typedef cl<T, T> U;
// error: a typedef cannot be a template.
```

注意模板声明不能用于局部作用域，因此如上用`using`创建类模板的类型别名时也不可在局部作用域中。

> ```C++
> // template declarations are only permitted at global, namespace, or class scope.
> ```

***

C++11开始可以为函数模板参数提供默认实参，在其之前只能为类模板参数提供默认实参。

> 即使一个类模板的所有模板参数都提供了默认实参，在使用类模板时都必须在类模板后接上一对尖括号。
>
> ```C++
> template <typename T = int, typename U = int>
> class cl {
> };
> 
> int main() {
> 
>     // cl a; 
>     // error: use of class template 'cl' requires template arguments
>     cl<> a;
>     return 0;
> }
> ```

***

形如`T &`形式的函数模板只能接受左值实参，无法自动推导出能够接受右值实参的形式，除非显式传入模板参数。

为了便于理解，可以缩小语言标准的有效范围，明确以下两点：

对于`T`/`const T`、`const T &`、`T &`、`const T &&`、`T &&`(以及`auto`/`const auto`、`const auto &`、`auto &`、`const auto &&`、`auto &&`)这五种形式：

+ 只有`auto &&`和`T &&`这种形式才能自动推导出带`const`的类型，其他形式自动推导出的类型都不会带`const`。
+ 只有`auto &&`和`T &&`这种形式才能自动推导出带`&`的类型，其他形式自动推导出的类型都不会带`&`。所有形式自动推导出的类型都不会带`&&`。

```C++
template <typename T>
void func(T &) { }
template <typename T>
void func2(const T &) { }

int main() {
    // func(1)
    // 错误，不能隐式推导类型T为const int
    func<const int>(1);
    // 正确，显式提供模板参数
    func2(1);
    // 正确，隐式推导类型T为int
    // ===========================
    const int a = 1;
    func(a);
    // 正确，隐式推导类型T为const int
    return 0;
}
```

***

模板匹配中遇到的问题：

```C++
#include <stdio.h>

template <typename T>
void forward(T &a) {
    printf("in func1\n");
}

template <typename T>
void forward(T &&a) {
    printf("in func2\n");
}

template void forward<int&>(int&);
```

上述代码在进行显式实例化模板的操作时，由于推导出的函数原型出现重复，理应出现报错的情况。在gcc 13.2编译下确实会报错` error: ambiguous template specialization 'forward<int&>' for 'void forward(int&)' `，但是在Clang17.0.1和msvc v19.37编译下都无任何报错提示。

```C++
#include <stdio.h>

template <typename T>
T && forward(T &a) {
    printf("in func1\n");
    return (T &&)a;
}

template <typename T>
T && forward(T &&a) {
    printf("in func2\n");
    return (T &&)a;
}

template int& forward<int&>(int&);
```

上述代码相较于前一段只修改了返回值类型，但是现在用GCC编译器可以成功编译，实例化后的函数会匹配到第二个函数模板。而Clang和msvc编译出的代码会匹配到第一个函数模板。

```C++
int main() {
    int a = 1;
    forward<int&>(a);
    return 0;
}
```

若将上述代码添加于前一段代码之后，则此处函数调用所使用的实例，无论是GCC还是Clang还是msvc编译出的代码都是使用第一个函数模板实例化出的函数。

综上，在代码编写中，应当尽量避免出现函数模板实例化之后的函数原型出现重复的情况，否则函数匹配的情况是未知的，既有可能匹配向重复的原型中任意一个函数实现，也有可能无法匹配到任何函数实现。

*据说*模板特例化程度排序是`const T&` > `T&` > `const T&&` > `T&&`，在显式传入模板实参后，如果得到的函数原型相同，则会按照以上优先级顺序进行模板选择。

如下代码，由上至下依次注释每个函数模板，理应依次显式1，2，3，4。在gcc和msvc中确实如此，但是在clang17.0.1下，注释掉第一个之后会出现二义性调用报错，clang将第二个模板和第三个模板视为同等程度的特例化。

```C++
#include <iostream>
using namespace std;

struct st {
    int a;
};

template <typename T>
void fun(const T&) {
    std::cout << "1" << std::endl;
}

template <typename T>
void fun(T&) {
    std::cout << "2" << std::endl;
}
template <typename T>
void fun(const T&&) {
    std::cout << "3" << std::endl;
}

template <typename T>
void fun(T&&) {
    std::cout << "4" << std::endl;
}

int main() {
    fun<const st&>(st{1});
    return 0;
}
```

***

函数模板在自动推导模板参数的时候可以运用引用折叠。

```C++
template <typename T>
void fn(T &&) {
};

int main() {
    int a = 0;
    fn(a);
    // void fn<int&>(int&)
}
```

类模板在进行特化模板选择的时候，不会运用引用折叠。

```C++
#include<stdio.h>
template <typename T>
struct rm {
    static void pr(){ printf("1\n"); }
};

template <typename T>
struct rm<T&> {
    static void pr(){ printf("2\n"); }
};

template <typename T>
struct rm<T&&> {
    static void pr(){ printf("3\n"); }
};

int main() {
    using a = int&;
    using b = a&&;
    using c = rm<void>&&;
    rm<b>::pr();
    rm<c>::pr();
    // 打印2 3
    // 若注释掉第二个模板，则打印1 3而不是打印3 3
    return 0;
}
```

***

会触发引用折叠的地方：

+ 用`typedef`或`using`定义的引用类型别名声明引用变量时
+ 函数模板显式传入模板参数时
+ `T &&`函数模板自动推导模板参数类型时
+ `auto &&`自动推导类型时

> 类模板显式传入模板参数时是在匹配最优选择，此时**不会**触发引用折叠(否则不能实现`remove_reference`)
>
> 除了`T &&`形式以外的其他形式(如`T &`、`const T &`、`const T &&`)函数模板自动推导模板参数类型时**都不会**触发引用折叠
>
> 除了`auto &&`形式以外的其他形式(如`auto &`、`const auto &`、`const auto &&`)自动推导类型时**都不会**触发引用折叠

```C++
template<class T>
void fn(const T &&) { }
 
int main() {
    int a = 1;
    // fn(a);  错误，无法自动推导T类型为int &
    fn<int &>(a);
}
```

***

注意对于模板形参`const T &&`而言，如果实参T的类型是`int &`，则形参类型不是`const int &`，而是`int &`。

> 该模板形参可以写作`T const &&`这种形式，带入实参T的类型之后得到`int & const &&`，由于`&`类型默认顶层`const`，所以前式`const`相当于无效，再加上引用折叠，即得到`int &`。
>
> 在用`typedef`或`using`定义的引用类型别名声明引用变量时或函数模板显式传入模板参数时，可能会得到某个中间类型是有顶层`const`的引用类型，此时前者会给出warning，后者完全合法。
>
> 不能单独声明有顶层`const`的引用变量，如`int && const a = 1; `(error: 'const' qualifier may not be applied to a reference)。

***

显式实例化函数模板的时候不能出现一样的函数原型，但是在函数模板调用的时候可以有一样的函数原型，此时会选出最特例化的一个模板进行实例化，否则调用有歧义。

已知"更特例化"的情况：

+ `T &`比`T &&`更特例化(因为右值引用模板可以实例化为左值引用或右值引用，相对更泛用)
+ `T *`比`const T&`更特例化
+ 模板参数更少的模板更特例化
+ 非可变参数模板比可变参数模板更特例化

```C++
#include<stdio.h>
template <typename T>
void func(T&) {
    printf("1\n");
}

template <typename T>
void func(T&&) {
    printf("2\n");
}

// template void func<int&>(int&);
// 此处模板实例化会得到一样的函数原型，在gcc中报错，在clang和msvc中选择实例化第一个模板

int main() {
    int a = 1;
    func<int&>(a);
    // 函数调用时，虽然函数原型都相同，但第一个模板更特例化，因此会无歧义实例化第一个模板
    return 0;
}
```

***

C风格类型转换后得到的表达式一般意义上都是右值，`(T &)`形式的类型转换可以得到左值表达式。

允许左值向右值转换，但是不允许右值向非`const`说明的左值转换。

```C++
int a = 1;
int *p = &(int &)a;              // 正确
int *q = &(int &&)a;             // error: cannot take the address of an rvalue of type 'int'.
int b = (const int&)(int&&)a;    // 正确
int c = (int&)(int&&)a;          // error: '&' requires l-value.
const int *d = &(const int &)1;  // warning: temporary whose address is used as value of local variable 'd'
                                 // will be destroyed at the end of the full-expression.
```

***

`std::vector`在realloc扩容的时候，转移旧成员到新地址用的是`std::move_if_noexcept`，即如果成员的移动函数定义为了`noexcept`则调用移动函数，否则调用拷贝函数。

```C++
#include <stdio.h>
#include <utility>
class cl;
const cl & move_if_noexcept(cl & a) { return (cl&&)a; } 

class cl {
    public:
    cl() { 
        printf("normal init %p\n", (void*)this); 
    }
    cl(const cl &) {
        printf("copy init %p\n", (void*)this);
    }
    // remove noexcept
    cl(cl &&) {
        printf("move init %p\n", (void*)this);
    }
    ~cl() {
        printf("distruct %p\n", (void*)this);
    }

};

int main() {
    cl a;                              // normal init
    cl b = std::move(a);               // move init
    cl c = std::move_if_noexcept(a);   // copy init
    cl d = move_if_noexcept(a);        // copy init
    cl e = (const cl &)(cl &&)a;       // copy init
    return 0;
}
```

***

函数模板可以被另一个同名函数模板或非模板函数重载。在进行匹配时，如果函数调用处与一个非模板函数和若干个函数模板都达到最佳匹配，则选择非模板函数。

> 此处 "都达到最佳匹配" 指的是候选集中任一函数**每个参数**的匹配都不劣于其他函数参数的匹配程度。

```C++
#include <stdio.h>

void fn(char, char) { printf("1\n"); }

template <typename T>
void fn(short, T) { printf("2\n"); }

int main(void) {
    fn(char(1), short(1));
    // error: call of overloaded 'fn(char, short int)' is ambiguous.
    return 0;
}
```

***

+ `catch`接受的参数类型要能严格匹配到`throw`抛出的类型，否则会按函数调用栈逐级向上检查可以捕获异常的地方，如果异常最终也未被捕获则会触发运行时错误。

  > 此处 "严格匹配" 指的是第一级和第二级匹配(即精确匹配 / 接收方比抛出方多出底层`const`)，从第三级开始(即类型提升 / 算术类型转换 / 类类型转换)就不被允许匹配。

+ 类类型异常对象将在与其对应的`catch`结束处被销毁。(若`catch`结束处没有`throw; `)

  > 理解异常的生命周期可以把`throw` - `catch`理解为函数传参的过程，`throw`处即为传递实参，`catch`执行完时即为函数调用完成时。

```C++
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
int count = 0;
class cl {
    public:
    int a;
    int c;
    cl(int a = 0) : a(a), c(count++) {  }
    cl(const cl& a) : a(a.a), c(count++) { }
    ~cl() { printf("distruct %d\n", c); }
};

void fn3() {
    throw cl(3);
    // throw cl(3).a;
    // 若是 throw cl(3).a; 则此时临时变量cl(3)已经被销毁
}

void fn2() {
    try {
        fn3();
    } catch(const char *a) {
        printf("%s\n", a);
    } catch(const std::string &e) {
        std::cout << e.c_str() << "\n"; 
    }
}

void fn1() {
    try {
        fn2();
    } catch(int a) {
        printf("%d\n", a);
    } catch(cl a) {
        printf("%d\n", a.a);
        // 先销毁局部变量a，再销毁异常抛出处的cl(3)
        // 类比函数调用完时先销毁局部变量，再销毁函数调用处传入的临时变量实参
    }
}

int main() {
    fn1();
    return 0;
}
```

***

`catch`时捕获的异常参数类型若不为引用，对接受到的异常值做出任何修改之后再`throw`到上层调用处，上层调用处仍为最原始的值，相当于只是修改了临时变量。

```C++
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
int count = 0;
class cl {
    public:
    int a;
    int c;
    cl(int a = 0) : a(a), c(count++) {  }
    cl(const cl& a) : a(a.a), c(count++) { }
    ~cl() { printf("distruct %d\n", c); }
};

void fn3() {
    throw cl(3);
}

void fn2() {
    try {
        fn3();
    } catch(cl a) {
    // 若上行改为cl &a则最终输出2
        a.a = 2;
        throw;
    }
}

void fn1() {
    try {
        fn2();
    } catch(cl a) {
        printf("%d\n", a.a);
    // 输出3
    }
}

int main() {
    fn1();
    // 输出distruct 1\n3\ndistruct 2\ndistruct 0\n
    // 相当于以逗号连接的连续函数调用 fnx1(cl(3)), fnx2(cl(3));
    // 连续函数调用在整个语句执行完之后才销毁临时变量实参
    // 因此最外层捕获不再继续抛出时才会析构最初传入的临时变量
    return 0;
}
```

***

异常参数生命周期：

```C++
#include <stdio.h>
#include <iostream>
#include <string>
int count = 0;
class cl {
    public:
    int a;
    int c;
    cl(int a = 0) : a(a), c(count++) { printf("init %d\n", c); }
    cl(const cl& a) : a(a.a), c(count++) { printf("copy %d\n", c); }
    ~cl() { printf("distruct %d\n", c); }
};

void fn3() {
    cl a(3);
    bool cond;
    std::cin >> cond;
    if(cond) {
        throw a;
    }
    else {
        printf("%d\n", a.a);
    }
}

void fn2() {
    try {
        fn3();
    } catch(cl a) {
        printf("fn2 %d\n", a.a);
        throw;
    }
}

void fn1() {
    try {
        fn2();
    } catch(cl a) {
        printf("fn1 %d\n", a.a);
    }
}

int main() {
    fn1();
    return 0;
}
// 输出结果为: 
// init 0
// copy 1---------------将异常参数复制到一个既不在栈也不在堆的空间上
// distruct 0-----------执行到throw处类比于函数执行到return处，在将返回值赋予临时量后会析构该函数内的所有局部变量
// copy 2
// fn2 3
// distruct 2-----------若catch语块末有throw; 则不会析构最开始的异常参数
// copy 3
// fn1 3
// distruct 3
// distruct 1-----------若catch语块末没有throw; 则编译器会在此生成最初传入的异常参数的析构函数
```

***

`cin`，`cout`的效率提升办法:

+ `std::ios_base::sync_with_stdio(false);` 用于解绑`iostream`和`stdio.h`，使用此语句之后不能再混用这两个库中的函数。
+ `std::cin.tie(nullptr);` 用于解除每次`std::cin`前的强制`std::flush`操作，使用此语句之后可能会在执行`cin`时看不到本该`cout`出现的内容。
+ 用"\n"替代`std::endl`，注意最后需要手动添加`std::flush`，否则可能会无法显示全部内容。

执行以上操作后`cin`，`cout`与`scanf`，`printf`的效率基本不相上下，甚至更快，因为不用再在运行时解析字符串来确定参数数量和格式。

***

类模板和友元：

+ 一对一友好关系：

  ```C++
  #include <stdio.h>
  
  template <typename T>
  struct st1;
  template <typename T>
  void fn(const st1<T> &);
  template <typename T>
  struct st2;
  
  template <typename T>
  struct st1 {
      friend void fn<T>(const st1<T>&);
      friend st2<T>;
      st1(int a) : a(a) { }
      private:
      int a;
  };
  
  template <typename T>
  struct st2 {
      template <typename U>
      void fn(const st1<T> &);
  };
  
  template <typename T>
  template <typename U>
  void st2<T>::fn(const st1<T> &a) { printf("%d\n", a.a); }
  
  template <typename T>
  void fn(const st1<T> &a) {
      printf("%d\n", a.a);
  }
  
  int main() {
      st1<int> a(3);
      fn(a);
      st2<int> b;
      b.fn<float>(a);
      return 0;
  }
  ```

+ 一对多友好关系：

  ```C++
  #include <stdio.h>
  
  template <typename T>
  void fn(const T&);
  template <typename T>
  struct st2;
  
  template <typename T>
  struct st1 {
      template <typename U>
      friend void fn(const U&);
      template <typename U>
      friend struct st2;
      // 注意这里不能声明为 template <typename U> friend st2<U>;
      // error: friend type templates must use an elaborated type.
      st1(int a) : a(a) { }
      private:
      int a;
  };
  
  template <typename T>
  void fn(const T& a) {
      printf("%d\n", a.a);
  }
  
  template <typename T>
  struct st2 {
      template <typename U>
      void fn(const st1<U> &);
  };
  
  template <typename T>
  template <typename U>
  void st2<T>::fn(const st1<U> &a) { printf("%d\n", a.a); }
  
  int main() {
      st1<int> a(3);
      fn(a);
      st2<float> b;
      b.fn(a);
      return 0;
  }
  ```

***

在被声明为不抛出异常的函数中，`noexcept`等价于`throw()`。

`noexcept`应当出现在函数的所有声明语句和定义语句中，否则一次也不应当出现。可以在函数指针的声明中说明`noexcept`。

`noexcept`说明符需要在`const`之后，在类的构造函数的初始化列表、函数尾置返回类型、`overrride`和`final`说明符及虚函数的`=0`之前。

> ```C++
> //==============1==============
> cl(int a) noexcept : a(a) { }
>
> //==============2==============
> struct Base {
>     virtual void vfn();
> };
> 
> struct A : Base {
>     auto vfn() noexcept ->void override = 0;
> };
> ```

***

自定义范围表达式的实现要点：必须要有名为`begin`和`end`的成员函数，返回指针。

```C++
#include <iostream>

class cl2 {
    int a;
    public:
    cl2(int a = 0) : a(a) { }
    friend std::ostream &operator<<(std::ostream &, const cl2&);
};

template <typename T>
class cl {
public:
    T *a;
    int size;
    cl(int n) : size(n) { a = new T[size]; for(int i = 0; i < size; i++) a[i] = T(i); }
    const T *begin() const { return a; }
    const T *end() const { return a + size; }
    cl(const cl&) = delete;
    cl operator=(const cl&) = delete;
    ~cl() { delete[] a; }
};

std::ostream & operator<<(std::ostream &o, const cl2& a) {
    std::cout << a.a;
    return o;
}

int main () {
    cl<cl2> a(4);
    for(const auto &i : a) {
        std::cout << i << "\n";
    }
    std::cout << std::flush;
    return 0;
}
```

***

特化的函数模板只有在源模板被选中的时候才会考虑是否被选择。如下，虽然fn2相对来说是最适配的一个模板，但是由于fn1和fn3之间选择了fn3，因此不会再进一步匹配到fn2。

```C++
#include <iostream>

using std::cout;
using std::endl;

template <typename T>
void fn(T) { cout << "fn1" << endl; }

template <>
void fn(int *) { cout << "fn2" << endl; }

template <typename T>
void fn(T *) { cout << "fn3" << endl; }

int main() {
    int *p = nullptr;
    fn(p);
    // fn3
    return 0;
}
```

***

列表初始化器的一种实现方式：

```C++
#include <iostream>

using std::cout;
using std::endl;

template <typename T>
class Initializer {
private:
    T* _List;
    size_t len;

public:
    Initializer()
        : _List(nullptr), len(0) {}
    
    template <typename... _Rest>
    Initializer(T _first, _Rest... _rest)
        : _List(new T[sizeof...(_rest) + 1]{_first, _rest...}), 
          len(sizeof...(_rest) + 1) {}

    Initializer<T>& operator=(Initializer<T> &&) = delete;
    // move operator [Implement your own if you want.]

    Initializer<T>& operator=(const Initializer<T>& ) = delete;
    // copy operator [Implement your own if you want.]

    const T operator[] (size_t index) const noexcept
        { return _List[index]; }
    T& operator[] (size_t index) noexcept
        { return _List[index]; }

    const T* begin() const noexcept
        { return _List; }
    T* begin() noexcept
        { return _List; }

    const T* end() const noexcept
        { return &_List[len]; }
    T* end() noexcept
        { return &_List[len]; }

    size_t length() const noexcept
        { return len; }

    ~Initializer() noexcept
        { _List != nullptr ? delete[] _List : void(0); }
};

void func(Initializer<int> list) {
    for(auto i : list) {
        cout << i <<endl;
    }
}

int main() {
    func({ 1, 2, 3 });
    return 0;
}
```

***

未出现在函数形参声明列表末尾的函数参数包不能自动推导对应的模板参数包类型。

```C++
template<typename T, typename U, typename ...Args>
void fn(T, Args..., U) { }

int main () {
    // fn<int, int>(1, 2, 3);
    // 错误，不能自动推导模板参数包Args的类型
    // note: candidate function [with T = int, U = int, Args = <>] not viable.
    fn<int, int, int>(1, 2, 3); // 正确
    return 0;
}
/****************************************************************************/
template<typename T, typename ...Args, typename U>
void fn(T, Args..., U = 0) { }

int main () {
    // fn<int, int, int>(1, 2, 3);
    // 错误，不能自动推导模板参数U的类型
    // 第一个int是T的类型，第二个和第三个int是模板参数包Args的类型
    // 模板参数包在模板参数列表中间，因此无法显式指定模板参数U的类型
    fn<int, int, int>(1, 2, 3, 4); // 正确，自动推导出U的类型为int
    return 0;
}
/****************************************************************************/
template<typename R1, typename ...A1, typename R2, typename ...A2>
void fn(R1(*)(A1...), R2(*)(A2...)) { }

int main () {
    using T1 = void(*)(int);
    using T2 = double(*)(char *, float);
    T1 p1 = nullptr;
    T2 p2 = nullptr;
    fn(p1, p2);
    // void fn<void, int, double, char *, float>(void (*)(int), double (*)(char *, float))
    // 无法显式指定模板实参，只能在使用模板函数的时候自动推导模板参数类型
    return 0;
}
/****************************************************************************/
template<typename T, typename U>
struct st;

template<typename R1, typename ...A1, typename R2, typename ...A2>
struct st<R1(*)(A1...), R2(*)(A2...)> {
    
};

using T1 = void(*)(int);
using T2 = double(*)(char *, float);

st<T1, T2> a;
// 类模板偏特化选择。其源模板不需要完整实现，只需要声明即可。
// 普通的类模板不能把模板参数包置于模板形参列表中间位置。
// (普通的类模板)template parameter pack must be the last template parameter.
```

***

参数包展开的两种形式：

```C++
#include <iostream>

void f(...) {

}
template <typename T>
int g(T &&a) {
    std::cout << a << " ";
    return 0;
}

template <typename ...Args>
void fn(Args &&...args) {
    f(1 + g(args)...); // 或 f((g(args) + 1)...);
    // 参数包可以在函数实参/模板实参中展开
    // 展开后为 f(1 + g(arg_1), 1 + g(arg_2), ... 1 + g(arg_n));
    // 输出顺序由编译器对函数传参的求值顺序确定
    std::cout << std::endl;
    int a[sizeof...(args)] = { (g(args), 0)... };
    // 参数包可以在括号初始化器内展开
    // 展开后为 int a[] = { (g(arg_1), 0), (g(arg_2), 0), ... (g(arg_n), 0) }
    // 此时输出顺序确定，输出结果为 1 2 3.3 string
    (void)a;
    std::cout << std::endl;
}

int main() {
    fn(1, 2, 3.3, "string");
    return 0;
}

/*********************************************************/
// print在C++中的实现: 
template <typename T>
int print_rest(T &&arg) {
    std::cout << " " << arg;
    return 0;
}

template <typename First, typename ...Rest> 
void print(First &&first, Rest &&...rest) {
    std::cout << first;
    int unused[] = { 0, print_rest(rest)... };
    // zero size arrays are an extension [-Wzero-length-array]
    (void)unused;
    std::cout << std::endl;
}

void print() {
    std::cout << std::endl;
}
```

***

嵌套模板的用法：

```C++
#include <iostream>
#include <vector>

// 注意声明模板的模板类型形参时，在C++17前只能用class不能用typename
// 因为在C++11引入别名模板(alias template)之前，只有类模板能作为模板的模板类型实参
template <template<typename, typename> class V, typename T, typename A>
void fn(V<T, A> &v) {
    // This can be "typename V<T, A>::value_type",
    // but we are pretending we don't have it
    T temp = v.back();
    v.pop_back();
    std::cout << temp << std::endl;
}

int main() {
    std::vector<int> a = { 1, 2, 3 };
    fn(a);
    return 0;
}
```

实际用到嵌套模板的地方：控制容器输出格式、实现策略模式等。

```C++
#include <iostream>
#include <vector>
#include <deque>
#include <list>

template<typename T, template<class,class...> class C, class... Args>
std::ostream& operator <<(std::ostream& os, const C<T,Args...>& objs)
{
    os << __PRETTY_FUNCTION__ << '\n';
    // gcc/clang extention.
    os << '[';
    if (objs.empty())
        goto end;

    for(auto i = objs.begin(); ; ) {
        os << *i;
        if (++i == objs.end())
            break;
        os << ", ";
    }
end:
    os << ']';
    return os;
}

int main()
{
    std::vector<float> vf { 1.1, 2.2, 3.3, 4.4 };
    std::cout << vf << '\n';
    std::list<char> lc { 'a', 'b', 'c', 'd' };
    std::cout << lc << '\n';
    std::deque<int> di { 1, 2, 3, 4 };
    std::cout << di << '\n';
    return 0;
}
// 输出
// std::ostream& operator<<(std::ostream&, const C<T, Args ...>&) [with T = float; C = std::vector; Args = {std::allocator<float>}; std::ostream = std::basic_ostream<char>]
// [1.1, 2.2, 3.3, 4.4]
// std::ostream& operator<<(std::ostream&, const C<T, Args ...>&) [with T = char; C = std::__cxx11::list; Args = {std::allocator<char>}; std::ostream = std::basic_ostream<char>]
// [a, b, c, d]
// std::ostream& operator<<(std::ostream&, const C<T, Args ...>&) [with T = int; C = std::deque; Args = {std::allocator<int>}; std::ostream = std::basic_ostream<char>]
// [1, 2, 3, 4]
```

***

需要单独标注`template`和`typename`的地方：

+ `T::template fn<>();` 或 `Class<T>::template fn<>();`
+ `typename T::type var;` 或 `typename Class<T>::type var;`

***

模板元做类型推断时可以循环递推。如标准库中的`std::remove_all_extents`，以及如下去除所有层级指针的模板：

```C++
#include <iostream>
#include <typeinfo>

template <class T>
struct rm_const {
    typedef T type;
};

template <class T>
struct rm_const<const T> {
    typedef T type;
};

template <class T>
struct rm_pointer {
    typedef T type;
};

template <class T>
struct rm_pointer<T*> {
    typedef typename rm_pointer<typename rm_const<T>::type >::type type;
};



int main() {
    std::cout << typeid(rm_pointer<int**const**>::type).name();
    // i
    return 0;
}
```

***
