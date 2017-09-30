#pragma once

class YukiMdHeaderNode : public yuki_node
{
public:
	__inline void setHeaderLevel(int headerLevel) { m_headerLevel = headerLevel; }

private:
	int m_headerLevel;
};

class YukiMdHeader : public yuki_structure
{
public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;
};