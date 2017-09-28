#pragma once

class YukiMdTitleMatcher : public YukiMatcher
{
public:

	virtual bool match(yuki_file_reader* reader, yuki_region* region);

	virtual bool lookAhead(const yuki_line_string* line, yuki_file_reader* reader);
};