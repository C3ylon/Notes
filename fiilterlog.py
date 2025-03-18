import re
from collections import defaultdict

# ===================== 配置区域 =====================
# 标签过滤条件（支持*通配符）
FILTER_CONFIG = {
    'A': "INFO*",    # 匹配以INFO开头的A标签
    'B': "WARN",     # 精确匹配B标签为WARN
    'C': "*",        # 匹配任意C标签
    'D': "SERVER*"   # 匹配以SERVER开头的D标签
}

INPUT_FILE = "input.log"    # 输入日志文件路径
OUTPUT_FILE = "output.log"  # 输出文件路径
# ===================================================

def compile_pattern(pattern):
    """将通配符模式转换为正则表达式"""
    if not pattern:
        return None
    pattern = re.escape(pattern)
    return re.compile(f'^{pattern.replace(r"\*", ".*")}$')

def generate_content_pattern(content):
    """生成内容特征模式（将数字替换为#）"""
    return re.sub(r'\d+', '#', content)

def main():
    # 编译标签过滤规则
    filters = {
        key: compile_pattern(value)
        for key, value in FILTER_CONFIG.items()
    }

    # 预编译日志行正则
    line_regex = re.compile(r'^\[(.*?)\]\[(.*?)\]\[(.*?)\]\[(.*?)\]\s(.*)$')

    # 使用字典存储分组信息
    content_groups = defaultdict(lambda: {
        'count': 0,
        'pattern': '',
        'examples': []
    })

    try:
        # 读取并处理日志文件
        with open(INPUT_FILE, 'r') as f:
            for line in f:
                line = line.strip()
                if not line:
                    continue

                # 解析日志行
                match = line_regex.match(line)
                if not match:
                    continue  # 跳过格式错误行

                # 提取标签和内容
                a, b, c, d = match.groups()[:4]
                content = match.group(5)

                # 执行标签过滤
                if not all(
                    (not pattern) or pattern.match(tag)
                    for tag, pattern in zip([a, b, c, d], filters.values())
                ):
                    continue

                # 生成内容特征模式
                content_pattern = generate_content_pattern(content)

                # 更新分组信息
                group = content_groups[content_pattern]
                if group['count'] == 0:
                    group['pattern'] = content_pattern
                group['count'] += 1
                if group['count'] <= 3:  # 保存前3个示例
                    group['examples'].append(content)

        # 写入分组结果到文件
        with open(OUTPUT_FILE, 'w') as f:
            # 按出现次数降序排序
            sorted_groups = sorted(
                content_groups.values(),
                key=lambda x: (-x['count'], x['pattern'])
            )

            for idx, group in enumerate(sorted_groups, 1):
                f.write(f"=== 第 {idx} 组（共 {group['count']} 条） ===\n")
                f.write(f"特征模式：{group['pattern']}\n")
                f.write("示例条目：\n")
                for example in group['examples']:
                    f.write(f"  - {example}\n")
                if group['count'] > 3:
                    f.write(f"  ...（还有 {group['count']-3} 条类似记录）\n")
                f.write("\n")

        print(f"日志分析完成！结果已保存至：{OUTPUT_FILE}")

    except FileNotFoundError:
        print(f"错误：输入文件 {INPUT_FILE} 不存在")
    except Exception as e:
        print(f"处理出错：{str(e)}")

if __name__ == '__main__':
    main()