#pragma once

class yuki_file_string;

class yuki_region_string
{
public:
	yuki_region_string(yuki_file_string* parent, const yuki_region* region);
	~yuki_region_string();

public:
	yuki_region_string* getRegionString(const yuki_region* region);

protected:
	void buildLineStringArray();

protected:
	yuki_file_string* m_parent;
	const yuki_region* m_region;
	const yuki_line_string** m_lines;
	int m_lineCount;
};
