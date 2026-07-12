# Python

## Python中的变量

## 核心要点

Python中的所有名称都是绑定在实体对象上。整数、浮点数、字符串、函数等，都具有实体的对象。一个变量重新赋值相当于把这个变量重新绑定到一个新的实体对象上。不同于C语言中一个变量名称对应固定的内存空间地址，Python中的变量名称可以任意绑定到具有不同内存空间地址的实体对象上。

## 作用域

Python中的变量名称作用域可以划分为三个部分：

+ 全局（非函数内也非类内）

  定义在全局的变量名称会出现在`globals()`函数返回的字典里。

  ```python
  a = 1
  b = {}
  print(globals())
  # {... 'a': 1, 'b': {}}
  ```

+ 函数内

  定义在函数内的变量名称会出现在`locals()`函数返回的字典里。

  ```python
  def fn():
      a = 1
      print(locals())
  fn()
  # {'a': 1}
  ```

  在函数内使用关键字`global`声明变量名称时，可以将该变量名称绑定到同名的全局变量名称上。此时该变量名称不会出现在`locals()`函数返回的字典里。

  ```python
  g = 1
  def fn():
      global g
      print(locals())
  fn()
  # {}
  ```

  注意当在函数内使用关键字`global`声明变量名称时，在当前函数内且声明语句前的代码中不能出现对该变量名称的读写操作。

  ```python
  g = 1
  def fn():
      # 在声明 global g 前的读操作：
      # print(g)
      # SyntaxError: name 'g' is used prior to global declaration
      # 在声明 global g 前的写操作：
      # g = 2
      # SyntaxError: name 'g' is assigned to before global declaration
      global g
  fn()
  ```

  函数内可以嵌套函数的定义。在内层函数中使用与外层函数中同名的变量名称时，如果在内层函数中第一次出现时是：

  + 读场景：视为将该变量名称作为闭包变量来处理，内层函数和外层函数共同使用。
  + 写场景：视为内层函数作用域内独立的变量名称，此后使用该变量名称与外层函数无关。

  在内层函数内使用关键字`nonlocal`声明变量名称时，可以将该变量名称绑定到同名的最内层外围局部变量名称上。此时该变量名称仍会出现在`locals()`函数返回的字典里，因为该变量是作为闭包变量被内层函数和外层函数共同使用。

  ```python
  def fn():
      i = 1
      not_use = 2
      def gn():
          nonlocal i
          print(locals())
      gn()
      # {'i': 1}
      print(gn.__closure__)
      # (<cell at 0x1051ba890: int object at 0x100b3a318>,)

      j = 1
      def hn():
          j = 2
          def kn():
              nonlocal j
              j = 3
          kn()
          print(j)
      hn()
      # 3
      print(j)
      # 1
  fn()
  ```

  注意当在内层函数内使用关键字`nonlocal`声明变量名称时，在当前函数内且声明语句前的代码中不能出现对该变量名称的读写操作。

  ```python
  def fn():
      a = 1
      def gn():
          # 在声明 nonlocal a 前的读操作：
          # print(a)
          # SyntaxError: name 'a' is used prior to nonlocal declaration
          # 在声明 nonlocal a 前的写操作：
          # a = 2
          # SyntaxError: name 'a' is assigned to before nonlocal declaration
          nonlocal a
  fn()
  ```

  函数默认参数绑定的默认值只会在定义函数时首次初始化，其后不会再初始化。

  ```python
  def fn(l=[]):
      l.append('a')
      print(l)
  fn()
  # ['a']
  fn()
  # ['a', 'a']
  ```

+ 类内

  类内成员变量名称绑定的默认值只会在定义类时首次初始化，其后不会再初始化

  ```python
  class cl:
      l = []
  a = cl()
  a.l.append('a');
  b = cl()
  print(b.l)
  # ['a']
  ```

***

Python的整除为向下取整。取余公式`x % y = x - ((x // y) * y)`。

```python
print(10 // -3) # -4
print(10 % -3)  # -2
```

***

| 进制 | 全称 | 类型简写 | 前缀 |
| :---: | :---: | :---: | :---: |
| 二进制 | binary | `bin` | `0b` |
| 八进制 | octal | `oct` | `0o` |
| 十六进制 | hexdecimal | `hex` | `0x` |

***

内置运算函数：`pow()` `abs()` `round()`

> `round()`为四舍五入，正好为5时向偶数取整。
>
> 取整对应到的还有`math.ceil()`，`math.floor()`，`math.trunc()`,分别对应向上取整、向下取整、向零取整。其中，向零取整还可以直接用`int()`。
> > `-3.7 // 1`的值是 -4.0，而`math.floor(-3.7)`的值是 -4。

***

在Unix系统中第一行加入`#!/usr/bin/env python`就可以直接执行`foo.py`。此时可以删除文件扩展名`.py`。

***

字符串行尾加上反斜杠会使**反斜杠**和**换行符**一起被转义,即忽略掉反斜杠和换行符。

***

表示Unicode字符串的三种方法：

+ `'\u'` 解析1字节或2字节
+ `'\U'` 解析4字节
+ `'\N{cat}'` 解析括号内的emoji

***

比之于直接使用`'xxx'.encode()`和`b'xxx'.decode()`更通用的处理方式是`bytes(s, encoding="utf-8")`和`str(s, encoding="utf-8")`。这样不论是处理二进制字符串还是普通字符串都不会出错。

***

`bytes`是不可变序列，如果需要可变则使用`bytearray`。需要注意的是修改`bytearray`的某一项时需要使用`ord(b'x')`这样的语句取得二进制字符的十进制序号。

***

Python内置**容器**：

+ 序列：`列表`、`元组`、`字符串`
+ 映射：`字典`
+ 既不是序列也不是映射：`集合`

> 适用于所有序列的操作：索引、切片、相加、相乘、成员资格检查、迭代 *(iteration)*

***

较新版本python中可以实现用关键字`in`查询字符串子串。比如`"aaa" in "abcaaadef"`其结果为`True`

***

列表的切片替代操作：

若有`a = [1, 2, 3, 4]`, 则:

+ `a[1:2] = []`结果为`[1, 3, 4]`
+ `a[1:1] = ["a", "b"]`结果为`[1, 'a', 'b', 2, 3, 4]`
+ `a[1:1] = "ab"`结果为`[1, 'a', 'b', 2, 3, 4]`
+ `a[1:1] = ["ab"]`结果为`[1, 'ab', 2, 3, 4]`
+ `a[1:2] = "ab"`结果为`[1, 'a', 'b', 3, 4]`

列表的切片替代不会改变该列表在内存中储存的地址。

***

拼接列表有两种方式：

1. 需要创建副本的拼接：

    ```python
    a = [1, 2, 3]
    b = [4, 5, 6]
    a = a + b
    ```

2. 无需创建副本的拼接：

    ```python
    # 一 列表自带的extend()方法
    a.extend(b)
    # 二 前述列表的切片替代操作
    a[len(a):] = b
    ```

***

函数参数：

```python
def fn(a, /, b, d1=1, *args, c, d2=3, **kwargs):
    pass
```

+ a 是**仅限位置参数**(**positional-only parameter**)，出现在`/`符号前。`/`符号用于形参列表中的特性是从 Python 3.8 开始引入。调用函数时对应的实参只能通过位置参数的形式传入，而不能通过关键字参数的形式传入。
+ b 是**位置参数**(**positional parameter**)。调用函数时对应的实参既可以通过位置参数的形式传入，又可以通过关键字参数的形式传入。
+ d1 是位置参数列表对应的**默认参数**(**default parameter**)。这一类默认参数必须在所有仅限位置参数以及位置参数的最后。即允许`fn(a, d1=1, /)`和`fn(a, /, b, d1=1)`，不允许`fn(a, d1=1, /, b)`
+ args 是**可变位置参数**(**variable positional parameter**)。如果仅有一个`*`号是不能接收可变参数列表的，必须要有`args`这样的参数名称才可以接收可变参数列表。
+ c 是**关键字参数**(**keyword-only parameter**)。调用函数时对应的实参只能通过关键字参数的形式传入，而不能通过位置参数的形式传入。
+ d2 是关键字参数列表对应的**默认参数**(**default parameter**)。这一类默认参数必须在所有关键字参数的最后，在可变关键字参数之前。
+ kwargs 是**可变关键字参数**(**variable keyword parameter**)。不能仅有`**`号，必须要有`kwargs`这样的参数名称。

实参传递形式：

+ **positional argument**: 位置实参
+ **keyword argument**: 关键字实参

```python
fn(positional_argument, keyword_argument=0)
```

***

格式化输出:

| 替换字段名 | 转换标志 | 冒号 | 填充字符 | 对齐方式 | 正号 | 对齐数 | 逗号分位符 | 小数位数 | 类型说明 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

> + **替换字段名**可以使用索引或用句点来获取属性和方法等。比如：
>
>     ```Python
>     >>> import math
>     >>> f"{math.pi}"
>         '3.141592653589793'
>     ```
>
> + **转换标志**包括`!s`、`!a`、`!u`，分别对应指定用`str`、`repr`、`ascii`进行转换。
> + **对齐方式**有`<`居左、`>`居右、`^`居中三种。此外可以在此位置使用`=`使正负符号和数字之间放置**填充字符**（若未指定**填充字符**则默认为空格）
> + **正号**可以使正数带正号。
> + 对于**小数位数**来说如果没有把**类型说明**指定为`f`，则会截断字符串取得对应的长度。比如：
>
>     ```Python
>     >>> from math import pi
>     >>> f"{pi!s:.3}"
>         '3.1'
>     >>> f"{pi:.3}"
>         '3.14'
>     >>> f"{pi:.3f}"
>         '3.142'
>     ```
>
> + **逗号分位符**在**对齐数**和**小数位数**之间
>
>    ```Python
>    >>> from math import pi
>    >>> "{:20,.2f}".format(pi*10000)
>        '           31,415.93'
>    ```

综合使用：

```Python
>>> improt math
>>> f"{math.pi*10000:@^+60,.3f}"
    '@@@@@@@@@@@@@@@@@@@@@@@@+31,415.927@@@@@@@@@@@@@@@@@@@@@@@@@'
>>> f"{math.pi*10000:@=+60,.3f}"
    '+@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@31,415.927'
```

***

迭代时可用到的内置函数：

+ 并行迭代：`zip()`
+ 迭代时获取索引：`enumerate()`
+ 反向迭代：`reversed()` （不影响原地址的可迭代对象）

***

`for...else`和`while...else`配对的代码块中，`else`代码块内的语句在循环体没有执行`break`语句时执行。

***

闭包（`nonlocal`关键字与`global`关键字对应，用于修改非全局的外部作用域变量; `globals()`和`locals()`内建函数分别用于输出全局变量和局部变量，以字典形式返回）

> `vars()`不带参的情况下与`locals()`作用相同，带参时返回参数（通常是类或类的实例）的属性和方法的字典。

```Python
def f1(a):
    def f2(b):
        print(locals())
        nonlocal a
        a=4
        print(locals())
        return a*b
    return f2

a = f1(3)
print(a(4))
```

执行结果：

```Python
{'b': 4, 'a': 3}
{'b': 4, 'a': 4}
16
```

***

python循环中不包含域的概念。比如：

```Python
l = []
for i in range(3):
    def f(x):
        return x * i
    l.append(f)
i = 9
print(l[0](5))
# 结果为45
```

修正方式是使用闭包：

```Python
l = []
for i in range(3):
    def f(i):
        def f2(x):
            return x * i
        return f2
    l.append(f(i))
i = 9
print(l[0](5))
# 结果为0
```

***

在类定义中对所有**以两个下划线起始**的名称都进行转换，转换为`_类名__原始名称`

***

`@property` 主要用到了__get__()这一魔术方法，将普通的类转变为描述符（descriptor）

***

`a, b = c, d`操作的原理是先暂存元组`(c, d)`，然后解包暂存的元组，将值从左至右依次赋值给`a`和`b`。因此若使用`a[i], a[a[i]] = a[a[i]], a[i]`互换`a[i]`和`a[a[i]]`两处的值，则会导致`a[i]`先被赋值，使得`a[a[i]]`索引出错。正确的写法是`a[a[i]], a[i] = a[i], a[a[i]]`。

比如：

```Python
>>> a=[1, 2, 3, 4, 5]
>>> a[a[1]], a[1] = a[1], a[a[1]]
>>> a
[1, 3, 2, 4, 5]
>>> a=[1, 2, 3, 4, 5]
>>> a[1], a[a[1]] = a[a[1]], a[1]
>>> a
[1, 3, 3, 2, 5]
```

***

Python中的字符串是不可变对象。因此拼接多个字符串时，使用`.join()`比使用`+=`效果好得多，前者是O(N)复杂度，后者是O(N*N)复杂度。

```python
import timeit

def benchmark(title, stmt, setup="", number=10):
    """统一打印格式"""
    t = timeit.timeit(stmt=stmt, setup=setup, number=number)
    print(f"[{title}]  {t/number*1000_000_000:.3f} ns per run")

benchmark(
    "s += 'a' * 1000 (循环 1000 次)",
    """
for _ in range(1000):
    s += "a"
""",
    "s = ''"
)

benchmark(
    "''.join(['a'] * 1000)",
    "''.join(['a'] * 1000)"
)

# [s += 'a' * 1000 (循环 1000 次)]  43379.993 ns per run
# [''.join(['a'] * 1000)]  4059.996 ns per run

```

***

python 中的运算符优先级顺序：

| 优先级（高→低） |                    运算符                    |
| :-------------: | :------------------------------------------: |
|        ①        |  `*`, `/`, `//`, `%`（乘、除、整除、取余）   |
|        ②        |              `+`, `-`（加、减）              |
|        ③        | 比较运算符：`<`, `>`, `<=`, `>=`, `!=`, `==` |
|        ④        |                    `not`                     |
|        ⑤        |                    `and`                     |
|        ⑥        |                     `or`                     |

***

创建多维列表时如果使用 `lst = [[[0]*n]*n]*n` 的形式，会导致最内层的一维列表引用被复制多份，体现出的结果就是当执行 `lst[x][y][z] = val` 时，`lst[ANY][ANY][z] == val`。

正确的创建多维列表的方式是使用列表推导式 `lst = [[[0 for _ in range(n)] for _ in range(n)] for _ in range (n)]`。

列表推导式的等效形式：

```python
lst = [[0 for _ in range(9)] for _ in range(9)]
# 等效于：
lst = []
for _ in range(9):
    lst2 = []
    for _ in range(9):
        lst2.append(0)
    lst.append(lst2)
```

***

Python 中的整数是堆上的对象。

对于 [-5, 256] 这个区间上所有的整数，由于在代码中会经常用到，因此在解释器初始化时就会在堆上创建这些整数的对象。这个机制称为小整数缓存机制(*small integer caching*)。

查看某个变量绑定的对象的地址可以用内置函数`id()`。如果`id(var_a) == id(var_b)`，那么`(var_a is var_b) == True`。

```python
# 在 Python 交互式解释器中执行：
>>> a = -5
>>> b = -5
>>> a is b
True
>>> a = -6
>>> b = -6
>>> a is b
False
>>> a = 256
>>> b = 256
>>> a is b
True
>>> a = 257
>>> b = 257
>>> a is b
False
# 如果不是在交互式解释器中执行，那么由于全部代码会被优化成 Python 虚拟机的字节码
# 因此显示都为 True
```

***

Python 闭包中捕获的变量的值并不会在闭包创建时就固定下来。

```python
def fn():
    v = 1
    def inner():
        print(v)
    v = "v is str"
    return inner

inner = fn()
print(inner.__closure__)
# (<cell at 0x0000019247A4B940: str object at 0x0000019247A1BDF0>,)
inner()
# v is str
```

由上面的代码可以看到，闭包`inner`中捕获到的变量`v`在闭包创建完毕之后类型从`int`变成了`str`，调用闭包函数时打印出的`v`的值是更新后的值。

```python
def fn(n):
    i = 1
    def inner():
        print(i, n)
    n = str(n)
    return inner

inner = fn(2)
print(inner.__closure__)
# (<cell at 0x0000024723469C30: int object at 0x00000247233000F0>,
#  <cell at 0x000002472346A1A0: str object at 0x0000024723415930>)
```

当外层函数中的某个变量（不论是外层函数的入参还是外层函数定义的局部变量）被闭包捕获时，该变量就会从 **fast local** 变成 **cell variable**。外层函数访问该变量时，也会多一次间接访问，即先从 cell 中查找该变量的地址，再通过查找到的地址来访问变量。

以上代码可以类比理解为：

```python
cell_of_inner = []

def fn(n):
    cell_of_inner.append(1)
    cell_of_inner.append(n)
    def inner():
        print(cell_of_inner[0], cell_of_inner[1])
    cell_of_inner[1] = str(cell_of_inner[1])
    return inner

inner = fn(2)
inner() # 1 2
```

***

Python 装饰器：

```python
def decorator(fn):
    def inner(*args, **kargs):
        print("before decorate")
        fn(*args, **kargs)
        print("after decorate")
    return inner

@decorator
def fn1(a, b):
    print(a, b)

def fn2(a, b):
    print(a, b)
fn2 = decorator(fn2)
# @decorator 可以看作是一个语法糖
# fn1 fn2完全等效
```

以上实现有一个问题，`fn1.__name__ == "inner"`，`fn2.__name__ == "inner"`。

为了避免这种情况，可以使用`functools.wraps`。

```python
from functools import wraps

def decorator(fn):
    @wraps(fn)
    def inner(*args, **kargs):
        print("before decorate")
        fn(*args, **kargs)
        print("after decorate")
    return inner

@decorator
def fn1(a, b):
    print(a, b)

def fn2(a, b):
    print(a, b)
fn2 = decorator(fn2)

print(fn1.__name__) # fn1
print(fn2.__name__) # fn2
```

`functools.wraps`的实现可以类比理解为：

```python
def wraps(fn):
    def inner(fn2):
        def inner2(*args, **kargs):
            fn2(*args, **kargs)
        inner2.__name__ = fn.__name__
        return inner2
    return inner

def decorator(fn):
    @wraps(fn)
    def inner(*args, **kargs):
        print("before decorate")
        fn(*args, **kargs)
        print("after decorate")
    return inner

@decorator
def fn1(a, b):
    print(a, b)

def fn2(a, b):
    print(a, b)
fn2 = decorator(fn2)

print(fn1.__name__) # fn1
print(fn2.__name__) # fn2
```

当`@`后出现与`def`命名的名称相同的名称时，可以看作`def`定义的一个临时函数对象还没有绑定名称，然后用`@`后的表达式来调用这个临时函数对象，最后调用的结果绑定到`def`命名的名称上。

```python
class MyPro:
    def __init__(self, fget):
        self.fget = fget

    def __get__(self, instance, owner):
        return self.fget(instance)

    def setter(self, fset):
        self.fset = fset
        return self

    def __set__(self, instance, value):
        self.fset(instance, value)

class cl:
    def __init__(self, i):
        self.i = i

    @property
    def x(self):
        return self.i ** 2

    @x.setter
    def x(self, i):
        self.i = i + 1

    @MyPro
    def y(self):
        return self.i ** 2

    @y.setter
    def y(self, i):
        self.i = i + 1

a = cl(2)
print(a.x) # 4
a.x = 3
print(a.x) # 16

b = cl(2)
print(b.y) # 4
b.y = 3
print(b.y) # 16
```

内置类`property`定义在 builtins.pyi 文件中，其内部只声明了两个`__get__`函数：

```python
# 1. 直接通过类来访问
@overload
def __get__(self, instance: None, owner: type, /) -> Self: ...
# 2. 通过类的实例对象来访问
@overload
def __get__(self, instance: Any, owner: type | None = None, /) -> Any: ...
# 3. 缺失，没有非 overload 修饰的真正实现
```

原因是 .pyi 文件作用是提供给 IDE 和静态类型检查工具作为说明，而 Python 解释器在运行代码时会直接调用底层的 C 语言模块来执行对应的逻辑。在 .pyi 文件中所有的函数体都用`...`代替，表示实现被省略。

***

Python 中的**索引**：

+ 当通过索引**读取**时：

  `obj[key]`等价于`obj.__getitem__(key)`。调用的魔术方法原型是`__getitem__(self, key)`。

+ 当通过索引**写入**时：

  `obj[key]`等价于`obj.__setitem__(key, value)`。调用的魔术方法原型是`__setitem__(self, key, value)`。

其中，`key`可以是任何类型的对象：整数、元组、列表、字符串、自定义类型，只要`obj.__getitem__(key)`或`obj.__setitem__(key, value)`函数支持对应的操作即可。

当`key`是以`,`分隔的多个元素时，会自动组合成`tuple`传入`__getitem__()`或`__setitem__()`的参数中。即`obj[a, b]`等效于`obj[(a,b)]`。

```python
class MySeq:
    def __getitem__(self, key):
        print("key:", key)

    def __setitem__(self, key, value):
        print("key:", key, end=" ")
        print("value:", value)

s = MySeq()
s[1, 2]                 # key: (1, 2)
s[(1, 2)]               # key: (1, 2)
s[[1, 2], ['a', 'z']]   # key: ([1, 2], ['a', 'z'])
s[1, 2] = ['a', 'z']    # key: (1, 2) value: ['a', 'z']
```

***

Python的切片（**slice**）本质上是对序列类型的一种通用协议支持，而不是字符串/列表的特例语法。

`slice(start, stop, step)`是一个真正的内建对象：

+ `var[start:stop]`在解释器内部会被翻译为`var.__getitem__(slice(start, stop, None))`
+ `var[start:stop:step]`在解释器内部会被翻译为`var.__getitem__(slice(start, stop, step))`

> 在Python中，`:`作为切片符号使用时，只会在下标语法（*subscript*）中生效。
即只有在`[]`符号里`:`才有切片的语义。

自定义类型实现切片的一个示例：

```python
class MySeq:
    def __getitem__(self, key):
        if isinstance(key, slice):
            print("slice args: ", key.start, key.stop, key.step)
        else:
            print("key:", key)

s = MySeq()
s["a":"z":"step"]           # slice args: a z step
s[slice("a", "z", "step")]  # slice args: a z step
s["a":"z"]                  # slice args: a z None
s["a":"z", 1:9]             # key: (slice('a', 'z', None), slice(1, 9, None))
s["a"]                      # key: a
```

***

在Python里，`with`是上下文管理器(*context manager*)语法糖，其核心作用是在一段代码前后，自动执行`__enter__()`和`__exit__(...)`，即使发生异常也能保证执行退出的收尾逻辑。

```python
with ContextManager as mng:
    body
```

逻辑上等价于：

```python
mng = ContextManager.__enter__()
try:
    body
finally:
    ContextManager.__exit__(...)
```

任何对象只要实现了以下两个方法，就能用于`with`：

+ `__enter__(self)`
+ `__exit__(self, exc_type, exc_val, exc_tb)`

***

在Python中`yield`是实现生成器(*Generator*)的核心关键字。

`yield`的作用是将一个普通的函数变成一个**生成器函数**。当函数执行到`yield`时会向调用者返回一个值，并暂停在该处，保留所有的局部变量状态。

定义一个函数`fn()`，里面有`yield`关键字，当执行`foo = fn()`时，Python解释器发现函数体里有 `yield`，就不会执行函数体里的任何一行代码，而是创建并返回一个生成器对象(**Generator Object**)。

只有当显式调用`next(foo)`，或者在`for`循环中迭代`foo`时，函数的代码才会真正开始运行。

```python
def fn():
    print("in yield fn")
    yield 0
    yield 1
    yield 2
    yield 3

y = fn()
print("===========")
print(next(y))
print("===========")
for i in y:
    print(i)
# ===========
# in yield fn
# 0
# ===========
# 1
# 2
# 3
```

`yield from`是在 Python 3.3 中引入的，主要用于**委托生成器**。

```python
def fn():
    lst1 = [1, 2, 3]
    for i in lst1:
        yield i
    # 等价写法
    lst2 = ['A', 'B', 'C']
    yield from lst2

y = fn()
for i in y:
    print(i, end="") # 123ABC
```

Python中的`os.walk()`函数是理解`yield`关键字非常好的示例。

```python
# top: 遍历的起始处
# topdown: 是否自顶向下进行遍历
# onerror: walk函数执行过程中发生异常调用的回调函数
# followlinks: 是否需要跟踪符号链接(symlink)
def walk(top, topdown=True, onerror=None, followlinks=False):
    top = fspath(top)
    dirs = []
    nondirs = []
    walk_dirs = []
    try:
        scandir_it = scandir(top)
    except OSError as error:
        if onerror is not None:
            onerror(error)
        return

    with scandir_it:
        while True:
            try:
                try:
                    entry = next(scandir_it)
                except StopIteration:
                    break
            except OSError as error:
                if onerror is not None:
                    onerror(error)
                return

            try:
                is_dir = entry.is_dir()
            except OSError:
                is_dir = False

            if is_dir:
                dirs.append(entry.name)
            else:
                nondirs.append(entry.name)

            if not topdown and is_dir:
                if followlinks:
                    walk_into = True
                else:
                    try:
                        is_symlink = entry.is_symlink()
                    except OSError:
                        is_symlink = False
                    walk_into = not is_symlink

                if walk_into:
                    walk_dirs.append(entry.path)

    if topdown:
        yield top, dirs, nondirs

        islink, join = path.islink, path.join
        for dirname in dirs:
            new_path = join(top, dirname)
            # Issue #23605: os.path.islink() is used instead of caching
            # entry.is_symlink() result during the loop on os.scandir() because
            # the caller can replace the directory entry during the "yield"
            # above.
            if followlinks or not islink(new_path):
                yield from walk(new_path, topdown, onerror, followlinks)
    else:
        for new_path in walk_dirs:
            yield from walk(new_path, topdown, onerror, followlinks)
        yield top, dirs, nondirs
```

***

从3.7开始，`dict`的遍历顺序严格等同于键的插入顺序。

```python
a = { 'a' : 1, 'b' : 2 ,'c' : 3 }
print(a)
b = { 'c' : 3, 'b' : 2 ,'a' : 1 }
print(b)

# {'a': 1, 'b': 2, 'c': 3}
# {'c': 3, 'b': 2, 'a': 1}
```

***

`and`和`or`两侧的类型可以不一致。

+ `A and B`表达式的值:
  + 当`bool(A) is False`时返回`A`
  + 当`bool(A) is True`时返回`B`
+ `A or B`表达式的值:
  + 当`bool(A) is False`时返回`B`
  + 当`bool(A) is True`时返回`A`

```python
a = {}
b = set()
c = a and b
d = a or b
print(c, d) # {} set()
```

***
