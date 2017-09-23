#pragma once

class YukiMdTitleMatcher : public YukiMatcher
{
public:

	virtual bool match(YukiFileReader* reader, yuki_region* region);

	virtual bool lookAhead(const YukiLineString* line, YukiFileReader* reader);
};