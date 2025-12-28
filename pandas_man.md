# Pandas 库使用笔记

## 导入包

```python
# 默认缩写 pd
import pandas as pd;
# DataFrame 是 pandas 的核心
df = pd.DataFrame()
# 设置显示选项，使 print(df) 时 DataFrame 不会省略显示，便于查看完整数据
pd.set_option('expand_frame_repr', False)
```

## 基础操作

新建列

```python
# 添加新的一列（默认创建在最右侧）
df['z']=[]
print(df)
# Empty DataFrame
# Columns: [z]
# Index: []
df['z'] = [1, 2, 3]
print(df)
#    z
# 0  1
# 1  2
# 2  3
df['a'] = [4, 5, 6]
print(df)
#    z  a
# 0  1  4
# 1  2  5
# 2  3  6

# =================

df = pd.DataFrame({'a': [1, 2, 3], 'b': [4, 5, 6], 'c': [7, 8, 9]})
print(df)
#    a  b  c
# 0  1  4  7
# 1  2  5  8
# 2  3  6  9

# =================
# 创建一个单列数据
sr = pd.Series(['a', 'b', 1, 2])
print(sr)
# 0    a
# 1    b
# 2    1
# 3    2
sr_with_idx = pd.Series(['a', 'b', 1, 2], index=['a_type', 'b_type', '1_type', '2_type'])
print(sr_with_idx)
# a_type    a
# b_type    b
# 1_type    1
# 2_type    2
```

列运算

```python
df['a'] = [1, 2, 3]
df['b'] = [4, 5, 6]
df['c'] = df['a'] * 2
df['d'] = df['a'] * df['b']
print(df)
#    a  b  c   d
# 0  1  4  2   4
# 1  2  5  4  10
# 2  3  6  6  18
print(df['b'] >= 5)
# 0    False
# 1     True
# 2     True

#注意 index 是原 df 被选中的行，而不是直接按从零开始递增
print(df[df['b'] >= 5])
#    a  b  c   d
# 1  2  5  4  10
# 2  3  6  6  18
```

文件交互

```python
import pandas as pd;
df = pd.DataFrame({'a': [1, 2, 3], 'b': [4, 5, 6], 'c': [7, 8, 9]})
print(df)
#    a  b  c
# 0  1  4  7
# 1  2  5  8
# 2  3  6  9
df.to_csv(r'./test.csv', index=False)
df = pd.read_csv(r'./test.csv', encoding="utf-8")
print(df)
#    a  b  c
# 0  1  4  7
# 1  2  5  8
# 2  3  6  9
```
