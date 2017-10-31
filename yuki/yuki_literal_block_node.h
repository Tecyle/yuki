#pragma once

class yuki_literal_block_node : public yuki_node
{
public:
	void setLanguage(wstring language);
	const wstring& getLanguage();
};