#pragma once

class YukiMdTitleMatcher : public YukiMatcher
{
public:

	virtual bool match(YukiFileReader* reader, yuki_region* region);

	virtual bool lookAhead(const yuki_line_string* line, YukiFileReader* reader);
};