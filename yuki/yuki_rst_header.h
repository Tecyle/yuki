#pragma once

class YukiRstHeaderNode : public yuki_node
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

class YukiRstHeader : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;
};