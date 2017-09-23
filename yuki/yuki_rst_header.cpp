#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_file_reader.h"
#include "yuki_region_ex.h"
#include "yuki_rst_header.h"

bool YukiRstHeader::parse(YukiNode* parentNode, const yuki_region* region)
{
	if (!match())
		return false;

	YukiFileReader* reader = getFileReader();
	YukiRstHeaderNode* headerNode = new YukiRstHeaderNode;
	wchar_t upperChar = 0;
	wchar_t lowerChar = 0;
	int upperSize = 0;
	int lowerSize = 0;
	int titleSize = 0;
	// �ϱ����жϣ��ϱ��߽���װ�Σ�����ϱ��ߺ��±��߷��Ų�һ�£����������棬���±���Ϊ׼
	const YukiLineString* line = reader->getLine();
	if (line->isSeperator())
	{
		// Լ���������߱���û������
		upperChar = line->getCharAtIndex(0);
		upperSize = line->getContentLength();
		reader->moveToNextLine();
		line = reader->getLine();
	}

	// �������ݽ���
	titleSize = line->getIndent() + line->getContentLength();
	const yuki_region* headerTextRegion = new YukiLinedRegion(line->beginCursor(), line->endCursor());
	getParser(L"inline_block")->parse(headerNode, headerTextRegion);

	// �±��߷���
	reader->moveToNextLine();
	line = reader->getLine();
	assert(line->isSeperator());
	lowerChar = line->getCharAtIndex(0);
	lowerSize = line->getContentLength();
	headerNode->setHeaderChar(lowerChar);
	headerNode->setDoubleLined(upperChar != 0);

	if (upperChar != 0 && upperChar != lowerChar)
	{
		if (upperChar != lowerChar)
		{
			// @WARNING ���±������õ��ַ���һ��
		}
		if (upperSize != lowerSize)
		{
			// @WARNING ���±��ߵĳ��Ȳ�һ��
		}
	}

	if (lowerSize < titleSize)
	{
		// @WARNING �±��ߵĳ������ڱ���ĳ���
	}

	reader->moveToNextLine();
	parentNode->appendChild(headerNode);
	return true;
}

/*
	rst ���ı���ʶ��ԭ��

	1. ����û������
	2. ��һ���Ƿָ��߻�����������
	3. ��һ������Ƿָ��ߣ���ڶ�������������
	4. �������ݺ�������Ӧ���Ƿָ���
*/
bool YukiRstHeader::match()
{
	YukiFileReader* reader = getFileReader();
	const YukiLineString* line = reader->getLine();

	if (line == nullptr || line->isBlankLine() || line->getIndent() != 0)
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
