#pragma once

/*
	directives 结构组成：

	simple-reference-name:: <arguments>
		<option list>

		<body>

	识别要点：
	1. 识别到 simple-reference-name 后面紧跟 ``::`` 以及空格就算识别成功
	2. 第一行如果有内容，则识别为 <arguments>，如果设定不接受 <arguments>，
	   则直接识别为 body
	3. 开始识别 <arguments> 之后，没有遇到空行的前提下，第二行的内容尝试识
	   别为 <option list>，如果失败则继续识别为 <arguments>，并且第二行的
	   缩进开始计算公共缩进
	4. 继续步骤 3，没有遇到空行的前提下，如果遇到的行小于等于 <arguments>
	   的公共缩进，则尝试识别为 <option list>，否则继续识别为 <arguments>
	5. 识别为 <option list> 之后，后续不属于 <option list> 部分的内容全都
	   属于 <body> 内容
	6. 如果设定不接受 <option list>，则空行之前的部分全都作为 <arguments>
	   识别
	7. 如果第一行没有内容，则第二行开始，尝试识别 <option list>，如果失败，
	   并且接受 <arguments>，则识别为 <arguments>，否则识别为 <body>；如果
	   成功，则识别为 <option list>，并认为 <arguments> 没有给出。

	如果遇到了未定义的指令，则整体内容均作为预排文本块输出。
*/

// 指令分配器
class YukiDirectives : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();

protected:
	wstring m_directiveName;
};

// 指令基类，所有指令必须命名为 指令名称 + ``-directive``，比如 ``image-directive``
class YukiDirective : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const YukiRegion* region) override;
	virtual bool match() override;

	virtual bool matchNoBackward();

protected:
	bool m_acceptArguments;
	bool m_acceptOptionLists;
	bool m_acceptBody;
};