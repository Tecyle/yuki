#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_line_string.h"
#include "yuki_file_reader.h"
#include "yuki_section.h"

/*
	�½ڵĽ�������

	1. ��ƥ��һ�����⣬���ƥ��ʧ�ܣ�����Ϊ����û�б�����½�
	2. ������������һ�����⣬�м�Ĳ��־����½���

	�����ǰ������п��л������ĵ���ͷ
*/
bool YukiSection::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* fileReader = getFileReader();
	const YukiRegion* oldRegion = fileReader->selectRegion(region);
	YukiSectionNode* sectionNode = new YukiSectionNode;

	fileReader->skipBlankLines();
	if (!getParser(L"md_header")->parse(sectionNode, region)
		&& !getParser(L"rst_header")->parse(sectionNode, region))
	{
		// Section with no header
	}

	getParser(L"body")->parse(sectionNode, searchSectionBodyRegion());

	parentNode->appendChild(sectionNode);
	fileReader->selectRegion(oldRegion);
	return true;
}

const YukiRegion* YukiSection::searchSectionBodyRegion()
{
	YukiFileReader* fileReader = getFileReader();
	int ln;
	bool lastLineIsBlankLine = false;

	for (ln=0; ln < fileReader->getLineCount(); ++ln)
	{
		const YukiLineString* line = fileReader->getLine(ln);

		if (line->isBlankLine())
		{
			lastLineIsBlankLine = true;
			continue;
		}

		if (lastLineIsBlankLine)
		{
			if (getMatcher(L"md_header")->lookAhead(line, fileReader)
				|| getMatcher(L"rst_header")->lookAhead(line, fileReader))
			{
				return fileReader->cutRegionFromCursorTo(ln - 1, -1);
			}
			lastLineIsBlankLine = false;
		}
	}

	return fileReader->getRegion();
}
