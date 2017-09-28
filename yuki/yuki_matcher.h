#pragma once

class yuki_file_reader;

class YukiMatcher
{
public:
	/*
		匹配函数，需要满足：

		* 匹配成功之后，要移动光标到匹配块的末尾
		* 匹配得到的区间要在 region 参数中返回
		* 匹配成功返回 true，匹配失败返回 false，不需要手动还原光标
	*/
	virtual bool match(yuki_file_reader* reader, yuki_region* region) = 0;

	/*
		无论匹配失败与否，都不能改变原本光标的位置
	*/
	virtual bool lookAhead(const yuki_line_string* line, yuki_file_reader* reader) = 0;

	const wchar_t* getName() const { return m_name; }

protected:
	const wchar_t* m_name;
};

class YukiMatcherCollection
{
public:
	YukiMatcherCollection(YukiGlobal* globalData);

public:
	// 初始化
	__inline void addFollowSet(const wchar_t* parent, const wchar_t* child);

	// 获取
	__inline YukiMatcher* getMatcher(const wchar_t* parent);
	__inline const vector<const wchar_t*>* getFollowSet(const wchar_t* parent) const;
	__inline const vector<const wchar_t*>* getInlineFollowSet(wchar_t ch) const;

protected:
	StringHashmap<YukiMatcher*> m_matchers;
	StringHashmap<vector<const wchar_t*>> m_followSet;
	unordered_map<wchar_t, vector<const wchar_t*>> m_inlineFollowSet;
};