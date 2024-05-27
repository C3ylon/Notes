# GoogleTest

## 基本概念

使用 GoogleTest 的核心是编写断言语句。

断言结果有以下三种：

1. 成功(*success*)：程序的行为符合预期。
2. 非致命失败(*nonfatal failure*)：断言失败，不会中断当前函数。
3. 致命失败(*fatal failure*)：断言失败，直接中断当前函数。

为产生相应断言结果可以使用以下两种断言宏函数：

1. `EXPECT_*`：在断言失败时会产生 **nonfatal failure**。
2. `ASSERT_*`：在断言失败时会产生 **fatal failure**。

具体的断言宏函数：

1. 一元比较：

   |         ASSERT          |         EXPECT          |      Verifies      |
   | :---------------------: | :---------------------: | :----------------: |
   | ASSERT_TURE(condition)  | EXPECT_TURE(condition)  | condition is true  |
   | ASSERT_FALSE(condition) | EXPECT_FALSE(condition) | condition is false |

2. 二元比较（缩写可以类比为汇编语言中的`JXX`条件跳转指令）：

   1. 值比较

      |        ASSERT         |        EXPECT         |  Condition   |
      | :-------------------: | :-------------------: | :----------: |
      | ASSERT_EQ(val1, val2) | EXPECT_EQ(val1, val2) | val1 == val2 |
      | ASSERT_NE(val1, val2) | EXPECT_EQ(val1, val2) | val1 != val2 |
      | ASSERT_LT(val1, val2) | EXPECT_EQ(val1, val2) | val1 < val2  |
      | ASSERT_LE(val1, val2) | EXPECT_EQ(val1, val2) | val1 <= val2 |
      | ASSERT_GT(val1, val2) | EXPECT_EQ(val1, val2) | val1 > val2  |
      | ASSERT_GE(val1, val2) | EXPECT_EQ(val1, val2) | val1 >= val2 |

   2. C风格字符串比较

      `ASSERT_STR_*` || `EXPECT_STR*`

      > `std::string`的比较使用`ASSERT_*`或`EXPECT_*`。

   3. 浮点比较

      + `ASSERT_FLOAT_*`|| `EXPECT_FLOAT_*`
      + `ASSERT_DOUBLE_*` || `EXPECT_DOUBLE_*`

> Tips:
>
> + 通常 `EXPECT_*` 是首选，因为它们允许在测试中报告多个失败。但是，如果当相关断言失败时继续没有意义，则应该使用 `ASSERT_*` 。
>
> + 由于失败的 `ASSERT_*` 会立即从当前函数返回，可能会跳过其后的清理代码，因此可能会导致**内存泄漏**。

任何可以流式传输到 `ostream` 的内容都可以流式传输到断言宏。要提供自定义失败消息，只需使用 `<<` 运算符将其流式传输到宏中。如下示例：

```C++
ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

for (int i = 0; i < x.size(); ++i) {
  EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
}
```
