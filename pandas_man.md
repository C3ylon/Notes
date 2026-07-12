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
# dtype: object
sr_with_idx = pd.Series(['a', 'b', 1, 2], index=['a_type', 'b_type', '1_type', '2_type'])
print(sr_with_idx)
# a_type    a
# b_type    b
# 1_type    1
# 2_type    2
# dtype: object
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
s = df['b'] >= 5
print(type(s))
# <class 'pandas.core.series.Series'>
print(s)
# 0    False
# 1     True
# 2     True
# Name: b, dtype: bool

#注意 index 是原 df 被选中的行，而不是直接按从零开始递增
print(df[s])
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

信息查看

```python
# 查看统计信息使用 describe() 函数
# 如果查看的是 DataFrame 对象的统计信息，则返回的也是 DataFrame 对象
# 如果查看的是 Series 对象的统计信息，则返回的也是 Series 对象

df = pd.DataFrame({'a': [1, 2, 3], 'b': [4, 5, 6], 'c': [7, 8, 9]})
print(type(df['a'].describe()))
# <class 'pandas.core.series.Series'>
print(df['a'].describe())
# count    3.0
# mean     2.0
# std      1.0
# min      1.0
# 25%      1.5
# 50%      2.0
# 75%      2.5
# max      3.0
# Name: a, dtype: float64
print(type(df.describe()))
# <class 'pandas.core.frame.DataFrame'>
print(df.describe())
#          a    b    c
# count  3.0  3.0  3.0
# mean   2.0  5.0  8.0
# std    1.0  1.0  1.0
# min    1.0  4.0  7.0
# 25%    1.5  4.5  7.5
# 50%    2.0  5.0  8.0
# 75%    2.5  5.5  8.5
# max    3.0  6.0  9.0

# 直接打印表格数据总览使用 info() 函数
# 该函数内部会直接打印，返回 None
df['a'].info()
# <class 'pandas.core.series.Series'>
# RangeIndex: 3 entries, 0 to 2
# Series name: a
# Non-Null Count  Dtype
# --------------  -----
# 3 non-null      int64
# dtypes: int64(1)
# memory usage: 152.0 bytes
df.info()
# <class 'pandas.core.frame.DataFrame'>
# RangeIndex: 3 entries, 0 to 2
# Data columns (total 3 columns):
#  #   Column  Non-Null Count  Dtype
# ---  ------  --------------  -----
#  0   a       3 non-null      int64
#  1   b       3 non-null      int64
#  2   c       3 non-null      int64
# dtypes: int64(3)
# memory usage: 200.0 bytes
```

选取指定行/列的数据：

```python
df = pd.DataFrame({'a': [1, 4, 7], 'b': [2, 5, 8], 'c': [3, 6, 9]})

# 1.1 选取单列，返回 Series 类型
print(df['a'])
# 0    1
# 1    2
# 2    3
# Name: a, dtype: int64

# 1.2 选取多列，返回 DataFrame 类型
print(df[['a']])
#    a
# 0  1
# 1  2
# 2  3
print(df[['a', 'c']])
#    a  c
# 0  1  7
# 1  2  8
# 2  3  9

# 2.1 选取单行，返回 Series 类型
print(df.loc[0])
# a    1
# b    4
# c    7
# Name: 0, dtype: int64

# 2.2 选取多行，返回 DataFrame 类型
print(df.loc[[0]])
#    a  b  c
# 0  1  4  7
print(df.loc[[0, 2]])
#    a  b  c
# 0  1  4  7
# 2  3  6  9

# 2.3 以切片形式选取多行
print(df[1:2])
#    a  b  c
# 1  2  5  8

# 2.4 以布尔掩码形式选取多行
print(df[[False, True, False]])
print(df[pd.Series([False, True, False])])
#    a  b  c
# 1  2  5  8
#    a  b  c
# 1  2  5  8

# 3 选取对应行和列的交点
print(df.loc[1, 'b':'c'])
# b    5
# c    8
# Name: 1, dtype: int64
print(df.loc[1:2, 'b':'c'])
#    b  c
# 1  5  8
# 2  6  9
print(df.iloc[1:2, 1:2])
#    b
# 1  5
print(df.iloc[1, 1])
# 5
```

```python
import ccxt

def get_all_api(exchange):
    _api_path_list = []
    all_api_dict = exchange.describe()['api']
    for first_k, first_v in all_api_dict.items():
        if isinstance(first_v, dict):
            for second_k, second_v in first_v.items():
                if second_k in ['get', 'post', 'put', 'delete'] and isinstance(second_v, dict):
                    for third_k, third_v in second_v.items():
                        last_path = ''.join([i[0].upper() + i[1:] for i in third_k.split('/')])
                        last_path = ''.join([i[0].upper() + i[1:] for i in last_path.split('-')])
                        _api_path = first_k + second_k[0].upper() + second_k[1:] + last_path
                        _api_path_list.append(_api_path)
    return _api_path_list


def get_private_functions(_api_word, _api_path_list):
    method = _api_word.split()[0]
    api_name = _api_word.split()[1]
    api_type = api_name.split('/')[1] if 'api' in api_name else ''

    res_list = []
    for api_path in _api_path_list:
        last_api_path = ''.join([i[0].upper() + i[1:] for i in api_name.split('/')[-1:]])
        last_api_path = ''.join([i[0].upper() + i[1:] for i in last_api_path.split('-')])
        if api_path.endswith(last_api_path) and method.capitalize() in api_path:
            if (api_type == 'api') and ('api' not in api_path):
                res_list.append(api_path)
            elif (api_type != 'api') and (api_path.startswith(api_type)):
                res_list.append(api_path)
    print(res_list)


if __name__ == '__main__':
    exchange = ccxt.binance()
    api_path_list = get_all_api(exchange)
    while True:
        api_word = input('API(exp: GET /api/v5/public/price-limit): ')
        get_private_functions(api_word, api_path_list)

```
