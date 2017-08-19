# 代码组织结构

文档对象，可以分为这么几类：

1. 列表结构。
2. 块结构。
3. 图结构。
4. 表结构。
5. 公式结构。
6. 内联结构。
7. 装饰结构。

自定义字符串类型：

```C++
class YukiString
{
private:
	size_t m_strLength;
	char* m_content;
};
```

一个文档解析结构，需要包括以下解析接口：

1. 继承自 ``yukiStructInterpretor`` 的解析接口。

```C++
class YukiNode
{
public:
	YukiString getNodeType() const;
	YukiString getNodeName() const;
};
```

> hello|df
> ---|---
> df | dfdf

