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
	const YukiLineString* m_lines;
	int m_lineCount;
};

class YukiLinedRegionString : public YukiRegionString
{
public:
	YukiLinedRegionString(yuki_file_string* parent, const yuki_region* region);
	~YukiLinedRegionString();

private:
};

class YukiBlockRegionString : public YukiRegionString
{
public:
	YukiBlockRegionString(yuki_file_string* parent, const yuki_region* region);
	~YukiBlockRegionString();

private:
	const YukiLineString* m_lines;
	int m_lineCount;
};