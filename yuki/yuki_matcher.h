#pragma once

class YukiFileReader;

class YukiMatcher
{
public:
	/*
		ƥ�亯������Ҫ���㣺

		* ƥ��ɹ�֮��Ҫ�ƶ���굽ƥ����ĩβ
		* ƥ��õ�������Ҫ�� region �����з���
		* ƥ��ɹ����� true��ƥ��ʧ�ܷ��� false������Ҫ�ֶ���ԭ���
	*/
	virtual bool match(YukiFileReader* reader, YukiRegion* region) = 0;

	/*
		����ƥ��ʧ����񣬶����ܸı�ԭ������λ��
	*/
	virtual bool lookAhead(const YukiLineString* line, YukiFileReader* reader) = 0;

	const wchar_t* getName() const { return m_name; }

protected:
	const wchar_t* m_name;
};

class YukiMatcherCollection
{
public:
	YukiMatcherCollection(YukiGlobal* globalData);

public:
	// ��ʼ��
	__inline void addFollowSet(const wchar_t* parent, const wchar_t* child);

	// ��ȡ
	__inline YukiMatcher* getMatcher(const wchar_t* parent);
	__inline const vector<const wchar_t*>* getFollowSet(const wchar_t* parent) const;

protected:
	StringHashmap<YukiMatcher*> m_matchers;
	StringHashmap<vector<const wchar_t*>> m_followSet;
};