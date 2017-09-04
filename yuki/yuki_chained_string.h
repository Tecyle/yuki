#pragma once

struct yuki_string_chain_node
{
	wchar_t* pStr;
	size_t length;
};

class YukiChainedString
{
private:
	yuki_string_chain_node* m_head;
};