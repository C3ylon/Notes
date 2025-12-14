# Python

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
> 取整对应到的还有`math.ceil()`和`math.floor()`,分别对应向上取整和向下取整。
> > `-3.7 // 1`的值是 -4.0，而`math.floor(-3.7)`的值是 -4。

***

在Unix系统终第一行加入`#!/usr/bin/env python`就可以直接执行`foo.py`。此时可以删除文件扩展名`.py`。

***

`repr('xxx')`等效于`r'xxx'`的值两端加上引号，其类型仍为`str`。两者末尾都不能以反斜杠结尾，否则会报错。

***

字符串行尾加上反斜杠会使**反斜杠**和**换行符**一起被转义,即忽略掉反斜杠和换行符。

***

表示Unicode字符串的三种方法：

+ `\u` 解析1字节或2字节
+ `\U` 解析4字节
+ `\N{cat}` 解析括号内的emoji

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

格式化输出:

| 替换字段名 | 转换标志 | 冒号 | 填充字符 | 对齐方式 | 正号 | 对齐数 | 逗号分位符 | 小数位数 | 类型说明 |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|

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

查看某个变量绑定的对象的地址可以用内置函数`id()`。如果`id(var_a) == id(var_b)`，那么`var_a is var_b`。

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

以上实现有一个问题，`fn1.__name__ == "inner"`，`fn2.__name__ == inner`。

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

***

Python 中的**索引**：

`obj[key]`完全等价于`obj.__getitem__(self, key)`。其中，`key`可以是任何类型的对象：整数、元组、列表、字符串、自定义类型，只要`obj.__getitem__(self, key)`函数支持对应的操作即可。

当`key`是以`,`分隔的多个元素时，会自动组合成`tuple`传入`__getitem__()`的参数中。即`obj[a, b]`等效于`obj[(a,b)]`。

```python
class MySeq:
    def __getitem__(self, key):
        print("key:", key)

s = MySeq()
s[1, 2]                 # key: (1, 2)
s[[1, 2], ['a', 'z']]   # key: ([1, 2], ['a', 'z'])
```

***

Python 的切片（**slice**）本质上是对序列类型的一种通用协议支持，而不是字符串/列表的特例语法。

`slice(start, stop, step)`是一个真正的内建对象：

+ `var[start:stop]`在解释器内部会被翻译为`var.__getitem__(slice(start, stop, None))`
+ `var[start:stop:step]`在解释器内部会被翻译为`var.__getitem__(slice(start, stop, step))`

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
