#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_directives.h"
#include "yuki_simple_reference_name.h"

bool YukiDirectives::parse(YukiNode* parentNode, const YukiRegion* region)
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const YukiRegion* oldRegion = reader->selectRegion(region);
	bool succ = false;

	if (!matchNoBackward())
		goto match_finished;

	succ = true;
	wstring directive = m_directiveName + +L"-directive";
	YukiStruct* parser = getParser(directive.c_str());
	if (parser != nullptr)
	{
		parser->parse(parentNode, reader->cutRegionFromCursorToEnd());
	}
	else
	{
		getParser(L"reserved_text")->parse(parentNode, reader->cutRegionFromCursorToEnd());
	}

match_finished:
	if (!succ)
		reader->setCursor(oldCursor);
	reader->selectRegion(oldRegion);
	return succ;
}

bool YukiDirectives::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiDirectives::matchNoBackward()
{
	YukiFileReader* reader = getFileReader();
	YukiSimpleReferenceName* simpleRefName = dynamic_cast<YukiSimpleReferenceName*>(getParser(L"simple_reference_name"));

	if (!simpleRefName->parseReferenceName(&m_directiveName))
		return false;

	if (!reader->matchStr(L"::"))
		return false;

	if (reader->skipSpaces() < 1)
		return false;

	return true;
}

bool YukiDirective::parse(YukiNode* parentNode, const YukiRegion* region)
{
	return inlineMode() ? parseInlineMode(parentNode, region) : parseBlockMode(parentNode, region);
}

bool YukiDirective::match()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiDirective::matchNoBackward()
{
	return inlineMode() ? matchNoBackwardInlineMode() : matchNoBackwardBlockMode();
}

bool YukiDirective::enableInlineMode(bool enable)
{
	if (!m_allowInlineMode)
		return false;

	m_inlineMode = enable;
	return true;
}

/*
	识别要点：
	1. 识别到 simple-reference-name 后面紧跟 ``::`` 以及空格就算识别成功
	2. 第一行如果有内容，则识别为 <arguments>，如果设定不接受 <arguments>，
	   则直接识别为 body
	3. 开始识别 <arguments> 之后，没有遇到空行的前提下，第二行的内容尝试识
	   别为 <option list>，如果失败则继续识别为 <arguments>，并且第二行的
	   缩进开始计算公共缩进
	4. 继续步骤 3，没有遇到空行的前提下，如果遇到的行小于等于 <arguments>
	   的公共缩进，则尝试识别为 <option list>，否则继续识别为 <arguments>
	5. 识别为 <option list> 之后，后续不属于 <option list> 部分的内容全都
	   属于 <body> 内容
	6. 如果设定不接受 <option list>，则空行之前的部分全都作为 <arguments>
	   识别
	7. 如果第一行没有内容，则第二行开始，尝试识别 <option list>，如果失败，
	   并且接受 <arguments>，则识别为 <arguments>，否则识别为 <body>；如果
	   成功，则识别为 <option list>，并认为 <arguments> 没有给出。
*/
bool YukiDirective::matchNoBackwardBlockMode()
{
	YukiFileReader* reader = getFileReader();
	yuki_cursor cursor = reader->getCursor();

	m_bodyRegion = nullptr;
	m_argumentsRegion = nullptr;
	m_optionListRegion = nullptr;

	if (reader->cursorAtLineEnd())
	{
		if (!acceptArguments())
		{
			m_bodyCursor = cursor;
			m_bodyRegion = reader->cutRegionFromCursorToEnd();
			return true;
		}
		// step 7
		if (!reader->moveToNextLine())
			return true;
		cursor = reader->getCursor();
		// TODO
	}
}
