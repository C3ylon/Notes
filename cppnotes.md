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

C++和C不同之处:

```C++
// 以下为在C++中的用法，都正确
void func1(int ...) { }
void func2( ... ) { }
```

1. C++在定义函数时可以有匿名形参，而C不能。(unnamed prototyped parameters not allowed when body is present)
2. C++在表示可变参数的`...`前可以没有参数，而C至少要有一个参数。
3. C++在表示可变参数的`...`前可以没有逗号，而C必须要有逗号。

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

   1. 该函数每个实参的匹配都不劣于其他可行函数需要的匹配
   2. 至少有一个实参的匹配优于其他可行函数需要的匹配

   > 从实参到形参的类型转化分为以下五个等级:
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
3. 其他类的成员函数。(需先声明其他类的不完全类型，然后声明本类的友元函数，然后定义其他类的完全类型，再在完全类型中声明友元函数，最后再定义该友元函数)

```C++
#include<stdio.h>

class clb;
class cla {
public:
    void prb(clb &b);
};

class clb {
    friend void cla::prb(clb &b);
    int b = 1;
public:
    void pra(cla &a);
};

void cla::prb(clb &b) {
    printf("%d\n", b.b);
}

int main () {
    cla a;
    clb b;
    a.prb(b);
    return 0;
}
```

某个友元函数的其他重载函数如果没有单独做友元声明则依然无法访问私有成员。

直接定义在类内的非成员友元函数是隐式`inline`，具有外部链接属性。即使在类内直接定义友元函数，也必须单独提供函数声明才能使函数可见。

```C++
class cl {
public:
    friend void func1() { printf("in func1\n"); }
    void func2() { func1(); } // error: use of undeclared identifier 'func1'.
    void func1();
    void func3() { func1(); } // 先声明后使用，正确
};

/************************************************/

#include<stdio.h>

class cl {
public:
    friend void func1() { printf("in func1\n"); }
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

`explicit`构造函数只能用于直接初始化，不能用于使用`=`的拷贝初始化。

尽管`explicit`构造函数不会被用于隐式的类类型转换，但是仍然可以用`static_cast<>`进行显示转换。

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
