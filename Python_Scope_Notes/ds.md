# Python 作用域与名称解析（LEGB）

> 本文按 Python 3 说明。
> LEGB 是普通变量名的查找顺序：**Local → Enclosing → Global → Built-in**。
> 类体、推导式是特殊执行块，不能简单当成普通的 Enclosing 作用域。

## 0. 总览

| 层级 | 名称 | 含义 | 典型位置 |
| --- | --- | --- | --- |
| L | Local | 当前函数/推导式内部的局部名字 | 函数参数、函数内赋值 |
| E | Enclosing | 外层函数作用域，形成闭包 | 嵌套函数读取外层函数变量 |
| G | Global | 当前模块的全局命名空间 | 模块顶层变量 |
| B | Built-in | 内置命名空间 | `len`、`print`、`range` |

三条总规则：

1. 函数块内只要对某个名字发生过绑定，且没有 `global` / `nonlocal`，这个名字在整个函数块内都是局部变量。
2. 函数嵌套时，内层函数只读外层函数变量，会形成闭包；内层函数重新赋值，默认只是创建自己的局部变量。
3. 类体命名空间不会成为方法的闭包作用域；推导式在 Python 3 中有自己的隐式函数作用域。

---

## 1. L：Local，局部作用域

### 1.1 函数内的局部名字

在函数代码块中被绑定、且没有声明为 `global` / `nonlocal` 的名字，属于局部名字。

常见绑定方式包括：

- 函数参数
- 普通赋值：`a = 1`
- 增强赋值：`a += 1`
- `for` 目标：`for i in ...`
- `with ... as x`
- `except ... as e`
- `import x`
- `def x()`、`class X`
- `del x`

```python
def fn():
    a = 1
    print(locals())

fn()
# {'a': 1}
```

`if`、`for`、`while`、`try`、`with` 等语句不会创建新的函数作用域：

```python
def fn():
    if True:
        a = 1
    print(a)

fn()
# 1
```

### 1.2 `locals()`

`locals()` 返回当前局部命名空间的一个快照。

注意：

- 修改 `locals()` 返回的字典，不保证能改变真实局部变量。
- 在 CPython 中，函数块里的自由变量/闭包变量通常也会出现在 `locals()` 里；类块中则不会。
- 不要依赖 `locals()` 的副作用写代码。

### 1.3 局部变量未绑定就读取

只要函数块内出现过对某个名字的赋值，这个名字在整个函数块内就是局部变量：

```python
x = 1

def fn():
    print(x)  # UnboundLocalError
    x = 2

fn()
```

虽然 `print(x)` 在赋值之前，但因为函数块内后面有 `x = 2`，所以 `x` 在整个 `fn` 内都是局部变量，读取时还没有绑定，于是抛出 `UnboundLocalError`。

### 1.4 `except ... as e` 的小细节

```python
try:
    1 / 0
except Exception as e:
    print(e)

# 这里再访问 e 会 NameError
# print(e)
```

`except ... as e` 中的 `e` 在 `except` 块结束后会被删除，用于打破异常对象的引用循环。

---

## 2. E：Enclosing，外层函数作用域与闭包

### 2.1 内层函数读取外层函数变量

内层函数可以读取外层函数的变量，形成闭包：

```python
def outer():
    x = 1

    def inner():
        return x

    return inner

f = outer()
print(f())  # 1
```

### 2.2 判断局部变量还是闭包变量：看整个函数块，不看“第一次出现”

内层函数中的某个名字是局部变量还是闭包自由变量，由整个函数代码块决定：

> 只要在该函数块内对这个名字有绑定操作，且没有 `global` / `nonlocal`，这个名字在整个函数块内都是局部变量。
> 只有当函数块内完全没有绑定、只读取这个名字时，才按 LEGB 向外查找，可能成为闭包自由变量。

反例：

```python
def outer():
    x = 1

    def inner():
        print(x)  # 第一次出现是读
        x = 2     # 但这句话让 x 在整个 inner 里成为局部变量

    inner()

outer()
# UnboundLocalError
```

所以不能按“第一次出现是读还是写”来判断。
只要 `inner` 里有 `x = 2`，`x` 在整个 `inner` 里就是局部变量，不是闭包变量。

### 2.3 只读闭包与 `__closure__`

```python
def outer():
    x = 1

    def inner():
        return x

    return inner

f = outer()
print(f())              # 1
print(f.__closure__)    # (<cell ...>,)
```

闭包保存的是外层变量的 cell，而不是简单拷贝。外层变量之后发生变化，内层函数读取时会看到当前值：

```python
def outer():
    x = 1

    def inner():
        return x

    x = 2
    return inner

print(outer()())  # 2
```

### 2.4 `nonlocal`

`nonlocal x` 用来声明：`x` 引用最近的外层函数作用域中已经存在的绑定。

规则：

- 会跳过类体。
- 不能指向全局变量。
- 外层函数中必须已经存在同名绑定，否则编译期报错：
  `SyntaxError: no binding for nonlocal 'x' found`
- `nonlocal` 只能用于嵌套函数内部。
- 同一个名字不能同时被 `global` 和 `nonlocal` 声明。

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
    # (<cell ...>,)

fn()
```

`nonlocal` 修改的是外层函数的绑定：

```python
def fn():
    j = 1

    def hn():
        j = 2

        def kn():
            nonlocal j
            j = 3

        kn()
        print(j)  # 3

    hn()
    print(j)      # 1

fn()
```

`kn` 中的 `nonlocal j` 绑定的是 `hn` 的 `j`，不是最外层 `fn` 的 `j`。

### 2.5 `nonlocal` 声明前不能使用同名变量

```python
def fn():
    a = 1

    def gn():
        # print(a)
        # SyntaxError: name 'a' is used prior to nonlocal declaration

        # a = 2
        # SyntaxError: name 'a' is assigned to before nonlocal declaration

        nonlocal a

fn()
```

这是编译期错误，即使函数没有被调用也会报。

### 2.6 修改可变对象 vs 重新绑定名字

原地修改可变对象不是重新绑定名字，不需要 `nonlocal`：

```python
def outer():
    lst = []

    def inner():
        lst.append(1)

    inner()
    print(lst)  # [1]

outer()
```

但重新赋值或增强赋值属于绑定操作，默认会让名字变成内层局部变量：

```python
def outer():
    lst = []

    def inner():
        lst = [1]      # 局部变量
        # lst += [2]   # 也是绑定操作，未 nonlocal 时会 UnboundLocalError

    inner()
    print(lst)  # []

outer()
```

---

## 3. G：Global，模块全局作用域

### 3.1 模块顶层名字

在当前模块顶层绑定的名字，会进入该模块的 `globals()` 字典：

```python
a = 1
b = {}

print(globals())
# {... 'a': 1, 'b': {}}
```

模块顶层时，`locals()` 和 `globals()` 是同一个字典。

注意：

- 类体里赋值的类属性不会进入 `globals()`。
- 类名本身会进入 `globals()`。
- `globals()` 不包含 `len`、`print` 这类内置名字。

### 3.2 `global` 声明

在函数内使用 `global g`，表示这个代码块里的 `g` 一律按全局名字解析。

```python
g = 1

def fn():
    global g
    print(locals())

fn()
# {}
```

`global g` 不会让 `g` 出现在 `locals()` 里。

如果全局没有 `g`，只读会 `NameError`；赋值会创建全局 `g`：

```python
def fn():
    global g
    g = 1

fn()
print(g)  # 1
```

### 3.3 `global` 声明前不能使用同名变量

```python
g = 1

def fn():
    # print(g)
    # SyntaxError: name 'g' is used prior to global declaration

    # g = 2
    # SyntaxError: name 'g' is assigned to before global declaration

    global g

fn()
```

`global` 是编译期声明，作用于整个代码块；但在源码中不能先使用、后声明。
即使那段代码不可达，也会报 `SyntaxError`。

---

## 4. B：Built-in，内置作用域

Python 最后会到 builtins 命名空间中查找名字：

```python
def fn():
    return len([])

print(fn())  # 0
```

`len` 不在 `globals()` 里，也不在 `locals()` 里，而是从 builtins 找到。

内置名字可以被遮蔽：

```python
len = 1

# 此时 len([]) 会报 TypeError
# del len  # 删除后重新回到 builtins 的 len
```

`globals()` 里通常会有一个 `__builtins__` 键，但它不展开成一个个内置名字。

---

## 5. 特殊执行块：类体

类体是一个独立命名空间，但它不是方法或嵌套函数的 Enclosing 作用域。

### 5.1 类体只执行一次

类体在 `class` 语句执行时执行一次，类属性表达式只求值一次，结果存入类命名空间。

```python
class Cl:
    l = []

a = Cl()
a.l.append('a')

b = Cl()
print(b.l)
# ['a']
```

这里 `a.l.append('a')` 修改的是类属性指向的同一个 list，所以 `b.l` 也能看到。

如果实例自己赋值，则会创建同名实例属性，遮蔽类属性：

```python
a.l = []
print(a.l)  # []
print(b.l)  # ['a']
```

类属性不是每个实例自动复制的默认值。可变类属性会被所有实例共享，直到实例用 `self.l = ...` 创建自己的属性。

### 5.2 类体不是方法的闭包作用域

方法里不能直接裸写类属性名：

```python
class C:
    x = 1

    def m(self):
        return x  # NameError

# 应写成 self.x 或 C.x
```

类体可以读取外层函数变量：

```python
def outer():
    x = 1

    class C:
        y = x  # OK

    return C.y

print(outer())  # 1
```

方法也可以闭包类体外的外层函数变量，但看不到类体里的类属性：

```python
def outer():
    x = 1

    class C:
        x = 2

        def m(self):
            return x  # 拿到的是 outer 的 x，不是 C.x

    return C().m()

print(outer())  # 1
```

属性查找不是 LEGB。`self.x` / `obj.x` 走的是实例字典、类及基类的 MRO，而不是普通变量名查找。

---

## 6. 特殊执行块：推导式与生成器表达式

### 6.1 推导式有自己的作用域

Python 3 中，列表、集合、字典推导式以及生成器表达式都有独立的隐式函数作用域；循环变量不会泄漏到外层。

```python
i = 0
l = [i for i in range(5)]
print(i)  # 0

l = []
for i in range(5):
    l.append(i)
print(i)  # 4
```

列表推导式里的 `i` 只属于该推导式的隐式函数作用域，不会改写外层的 `i`。
Python 2 的列表推导式会泄漏循环变量，Python 3 不会。

### 6.2 类体中的推导式

类体中的推导式看不到类属性，除非该名字出现在最外层 iterable 中：

```python
class C:
    x = 1

    y = [x for _ in range(1)]   # NameError，推导式里看不到 C.x
    z = [1 for _ in range(x)]   # OK，range(x) 在类体中求值
```

### 6.3 海象运算符

推导式中的海象运算符 `:=` 会把目标绑定到包含推导式的外层作用域，而不是推导式内部的隐式函数作用域：

```python
result = [(y := x) for x in range(3)]
print(y)  # 2
```

类体等场景对海象运算符还有额外限制，这里不展开。

---

## 7. 附：默认参数求值时机

默认参数不是作用域问题，但很容易和“局部/闭包”混淆。

默认参数表达式在 `def` 执行时求值一次，结果保存在函数对象的 `__defaults__` 中；之后每次调用都会复用同一个默认对象，不会重新求值。

```python
def fn(l=[]):
    l.append('a')
    print(l)

fn()
# ['a']
fn()
# ['a', 'a']
```

默认表达式使用的是定义处的名字，不是调用处的名字：

```python
x = 1

def f(a=x):
    return a

x = 2
print(f())  # 1
```

可变默认参数通常建议改成：

```python
def fn(l=None):
    if l is None:
        l = []
    l.append('a')
    return l
```

---

## 8. 常见坑速查

1. **`UnboundLocalError`**
   函数块内只要后面有赋值，整个函数块内这个名字就是局部变量；赋值前读取会报错。

2. **`nonlocal` 找不到外层绑定**
   `nonlocal x` 要求外层函数中已经存在 `x`；全局变量不算，类属性也不算。

3. **类属性不是实例默认副本**
   可变类属性被所有实例共享；`self.x = ...` 才会创建实例属性。

4. **方法看不到类体里的类属性**
   方法里要写 `self.x` 或 `C.x`，不能裸写 `x`。

5. **推导式循环变量不泄漏**
   Python 3 中列表/集合/字典推导式和生成器表达式都有自己的作用域。

6. **`locals()` 是快照**
   不要依赖修改 `locals()` 来改变局部变量。

7. **`global` / `nonlocal` 是编译期声明**
   不能先使用、后声明；错误在编译期就会报。
