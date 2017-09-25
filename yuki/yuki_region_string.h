#pragma once

class yuki_file_string;

class YukiRegionString
{
public:
	YukiRegionString(yuki_file_string* parent) { m_parent = parent; }

	YukiRegionString* getRegionString(const yuki_region* region);

protected:
	yuki_file_string* m_parent;
	yuki_region* m_region;
	const yuki_line_string* m_lines;
	int m_lineCount;
};
