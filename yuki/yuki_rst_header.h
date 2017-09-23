#pragma once

class YukiRstHeaderNode : public YukiNode
{
public:
	void setHeaderChar(wchar_t ch);
	void setDoubleLined(bool isDoubleLined);
	void setHeaderLevel(int headerLevel);

private:
	wchar_t m_headerChar;
	bool m_isDoubleLined;
	int m_headerLevel;
};

class YukiRstHeader : public YukiStruct
{
public:
	virtual bool parse(YukiNode* parentNode, const yuki_region* region) override;
	virtual bool match() override;
};