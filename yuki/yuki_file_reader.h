#pragma once

class YukiFileReader
{
public:
	const YukiRegion* selectRegion(const YukiRegion* region);
	int skipBlankLines();
	const YukiRegion* cutRegionFromCursorTo(int ln, int col);
	const YukiRegion* getRegion();
	int getLineCount() const;
	const YukiLineString* getLine(int offset = 0) const;

protected:
	YukiFileString m_fileString;
};