#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_matcher.h"
#include "yuki_line_string.h"
#include "yuki_file_reader.h"
#include "yuki_md_title_matcher.h"

/*
	Markdown �����ȷ�ϱ�־

	1. �ж��в���������
	2. �����ɸ� ``#`` ��ͷ������ ``#`` ����������һ���ո�
	3. ``#`` ����������һ���ǿհ��ַ�
*/
bool YukiMdTitleMatcher::lookAhead(const yuki_line_string* line, yuki_file_reader* reader)
{
	bool succ = false;
	if (line->isBlankLine() || line->getIndent() != 0)
		return false;

	if (!reader->matchChar('#'))
		return false;

	yuki_cursor oldCursor = reader->getCursor();
	while (reader->matchChar('#'));

	if (reader->skipSpaces() == 0)
		goto match_failed;

	if (reader->cursorAtLineEnd())
		goto match_failed;

	succ = true;

match_failed:
	reader->setCursor(oldCursor);
	return succ;
}
