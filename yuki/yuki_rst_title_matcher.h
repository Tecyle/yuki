#pragma once

class YukiRstTitleMatcher : public YukiMatcher
{
public:

	virtual bool match(YukiFileReader* reader, YukiRegion* region);

	virtual bool lookAhead(const YukiLineString* line, YukiFileReader* reader);
};