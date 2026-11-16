# Python Scope

## 作用域

### 总览

+ **作用域在编译期静态确定**：编译器扫描整个函数体，只要某个名称在函数体内**任意位置**被绑定，且没有 `global`/`nonlocal` 声明，它在**整个函数内**都是局部变量。
+ 绑定操作包括：赋值、增强赋值（`+=` 等）、`for` 循环目标、`with ... as`、`except ... as`、`import`、`def`、`class`、`:=`、`del`。
+ 只有四种结构会产生新作用域：**模块**、**函数**（含 `lambda`）、**类**、**推导式/生成器表达式**。`if`/`for`/`while`/`with`/`try` **不会**产生新作用域。

  ```python
  for i in range(5):
      pass
  print(i)  # 4，for 没有新作用域
  ```

+ 名称按 **LEGB** 顺序查找，找到即停止：

  | 缩写 | 含义 | 说明 |
  | --- | --- | --- |
  | L | Local | 当前函数的局部作用域 |
  | E | Enclosing | 外层函数作用域（由内向外逐层） |
  | G | Global | 当前模块的全局作用域 |
  | B | Built-in | 内置作用域（`builtins` 模块，如 `len`、`print`） |

### L：局部作用域（Local）

+ 定义在函数内的变量名称会出现在 `locals()` 返回的字典里。

  ```python
  def fn():
      a = 1
      print(locals())
  fn()
  # {'a': 1}
  ```

+ 函数内的 `locals()` 返回的是**快照**，修改这个字典不会改变真正的局部变量。（`globals()` 返回的则是模块的真实字典，修改会生效。）

+ 同一个名称只要在函数内被绑定过，就是局部变量，**即使绑定语句在读取之后**：

  ```python
  g = 1
  def fn():
      print(g)  # UnboundLocalError：下面的 g = 2 使 g 成为局部变量
      g = 2
  fn()
  ```

  `+=` 同理：

  ```python
  g = 1
  def fn():
      g += 1  # UnboundLocalError：+= 是绑定操作
  fn()
  ```

### E：外层函数作用域（Enclosing）

+ 函数内可以嵌套函数定义。内层函数中，如果某名称**只被读取、从未绑定**，它就是**自由变量（闭包变量）**，由内外层函数共同使用。自由变量会出现在内层函数的 `locals()` 和 `__closure__` 中。

  ```python
  def fn():
      i = 1
      def gn():
          print(i)          # 只读，i 是自由变量
          print(locals())   # {'i': 1}
      gn()
      print(gn.__closure__)
      # (<cell at 0x...: int object at 0x...>,)
  fn()
  ```

+ 如果该名称在内层函数体内**任意位置被绑定**，它就是内层函数自己的局部变量，与外层同名变量无关；在绑定之前读取会抛出 `UnboundLocalError`。

  ```python
  def outer():
      x = 1
      def inner():
          print(x)  # UnboundLocalError
          x = 2
      inner()
  outer()
  ```

+ 在内层函数中使用 `nonlocal` 声明名称，可以让内层函数**重新绑定**最近的外层**函数**作用域（不含全局和类作用域）中的同名变量。该变量依然是自由变量，因此仍会出现在 `locals()` 中。

  ```python
  def fn():
      i = 1
      not_use = 2
      def gn():
          nonlocal i
          print(locals())
      gn()
      # {'i': 1}，not_use 未被引用，不在其中

      j = 1
      def hn():
          j = 2
          def kn():
              nonlocal j  # 绑定到 hn 中的 j，而不是 fn 中的 j
              j = 3
          kn()
          print(j)
      hn()
      # 3
      print(j)
      # 1
  fn()
  ```

+ `nonlocal` 的约束：
  + 声明的名称必须已在外层函数作用域中存在绑定，否则是 `SyntaxError: no binding for nonlocal 'x' found`，它也不能指向全局变量。
  + 在当前函数内且声明语句之前，不能出现对该名称的读写操作。

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

+ 闭包捕获的是**变量**而不是**值**，所以存在延迟绑定：

  ```python
  fs = [lambda: i for i in range(3)]
  print([f() for f in fs])  # [2, 2, 2]

  fs = [lambda i=i: i for i in range(3)]  # 用默认参数在定义时固定值
  print([f() for f in fs])  # [0, 1, 2]
  ```

### G：全局作用域（Global）

+ 全局严格说是**模块级**作用域，每个模块有自己的全局作用域（非函数内也非类内）。定义在模块级的变量名称会出现在 `globals()` 返回的字典里。

  ```python
  a = 1
  b = {}
  print(globals())
  # {... 'a': 1, 'b': {}}
  ```

+ 函数内**仅读取**全局变量不需要任何声明。要在函数内**重新绑定**全局变量，需要使用 `global` 声明。声明后，赋值作用于全局变量（全局中尚不存在时会创建它），该名称也不会出现在 `locals()` 中。

  ```python
  g = 1
  def fn():
      global g
      g = 2
      print(locals())  # {}
  fn()
  print(g)  # 2
  ```

+ 在当前函数内且 `global` 声明语句之前，不能出现对该名称的读写操作。

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

### B：内置作用域（Built-in）

+ 内置名称（`len`、`print`、`list` 等）定义在 `builtins` 模块中，是查找的最后一站。
+ 它们可以被其他作用域的同名变量**遮蔽**，要避免用内置名称做变量名：

  ```python
  list = [1, 2]
  list(range(3))  # TypeError: 'list' object is not callable
  ```

### 特殊作用域

#### 类作用域

+ 类体在执行 `class` 语句时执行**一次**。其中定义的是**类属性**，被所有实例共享。

  ```python
  class Cl:
      l = []
  a = Cl()
  a.l.append('a')
  b = Cl()
  print(b.l)
  # ['a']
  ```

+ `a.l.append(...)` 是修改类属性指向的对象；而 `a.l = []` 会创建同名**实例属性**并遮蔽类属性，不影响其他实例。
+ 在类体内，`locals()` 返回的就是类的命名空间本身。
+ **类作用域对嵌套的函数不可见**：方法内要访问类属性，必须写 `self.x` 或 `Cl.x`。

  ```python
  class C:
      x = 1
      def f(self):
          return x  # NameError
  ```

+ 类体内的推导式同样看不到类变量（最外层的可迭代对象除外，它在类作用域中求值）：

  ```python
  class C:
      n = 2
      xs = [1, 2, 3]
      ys = [x * n for x in xs]  # NameError: n
  ```

#### 推导式作用域

+ 在 Python 3 中，列表/集合/字典推导式和生成器表达式都有自己的作用域，其中的循环变量不会泄漏到外部。（Python 2 的列表推导式会泄漏。）

  ```python
  i = 0
  l = [i for i in range(5)]
  print(i)  # 0
  ```

+ 例外：海象运算符 `:=` 会把变量绑定到推导式所在的外层作用域。

  ```python
  [y := i for i in range(3)]
  print(y)  # 2
  ```

### 附：默认参数的求值时机

这不是作用域本身的问题，但常和作用域一起被问到。

+ 默认值表达式在执行 `def` 语句时**只求值一次**，之后每次调用共用同一个对象。默认值是可变对象时，会在多次调用间共享状态。

  ```python
  def fn(l=[]):
      l.append('a')
      print(l)
  fn()
  # ['a']
  fn()
  # ['a', 'a']
  ```

+ 惯用写法：

  ```python
  def fn(l=None):
      if l is None:
          l = []
      l.append('a')
      return l
  ```
