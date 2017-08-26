#pragma once

class YukiFileString;

class YukiRegionString
{
public:
	YukiRegionString(YukiFileString* parent) { m_parent = parent; }

	YukiRegionString* getRegionString(const YukiRegion* region);

protected:
	YukiFileString* m_parent;
	YukiRegion* m_region;
};

class YukiLinedRegionString : public YukiRegionString
{
public:
	YukiLinedRegionString(YukiFileString* parent, const YukiRegion* region);
	~YukiLinedRegionString();

private:
	const YukiLineString* m_lines;
	int m_lineCount;
};

class YukiBlockRegionString : public YukiRegionString
{
public:
	YukiBlockRegionString(YukiFileString* parent, const YukiRegion* region);
	~YukiBlockRegionString();

private:
	const YukiLineString* m_lines;
	int m_lineCount;
};