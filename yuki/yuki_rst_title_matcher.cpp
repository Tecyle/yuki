#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_matcher.h"
#include "yuki_line_string.h"
#include "yuki_file_reader.h"
#include "yuki_rst_title_matcher.h"

/*
	rst ���ı���ʶ��ԭ��

	1. ����û������
	2. ��һ���Ƿָ��߻�����������
	3. ��һ������Ƿָ��ߣ���ڶ�������������
	4. �������ݺ�������Ӧ���Ƿָ���
*/
bool YukiRstTitleMatcher::lookAhead(const YukiLineString* line, YukiFileReader* reader)
{
	if (line->isBlankLine() || line->getIndent() != 0)
		return false;

	yuki_cursor oldCursor = reader->getCursor();
	bool succ = false;

	// ��һ������Ƿָ��ߣ�������һ��
	if (line->isSeperator())
	{
		if (!reader->moveToNextLine())
			goto match_failed;
		line = reader->getLine();
	}

	// �ڶ��в����ǿ��У�Ҳ�����Ƿָ��ߣ����ǿ���������
	if (line->isBlankLine() || line->isSeperator())
		goto match_failed;

	// ����һ�У�����һ�б����Ƿָ���
	if (!reader->moveToNextLine())
		goto match_failed;
	line = reader->getLine();

	succ = line->isSeperator();
	
match_failed:
	reader->setCursor(oldCursor);
	return succ;
}
