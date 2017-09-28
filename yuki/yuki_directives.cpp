#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_directives.h"
#include "yuki_simple_reference_name.h"

bool YukiDirectives::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);
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
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();

	bool succ = matchNoBackward();

	reader->setCursor(oldCursor);
	return succ;
}

bool YukiDirectives::matchNoBackward()
{
	yuki_file_reader* reader = getFileReader();
	YukiSimpleReferenceName* simpleRefName = dynamic_cast<YukiSimpleReferenceName*>(getParser(L"simple_reference_name"));

	if (!simpleRefName->parseReferenceName(&m_directiveName))
		return false;

	if (!reader->matchStr(L"::"))
		return false;

	if (reader->skipSpaces() < 1)
		return false;

	return true;
}

bool YukiDirective::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor oldCursor = reader->getCursor();
	const yuki_region* oldRegion = reader->selectRegion(region);

	bool succ =  inlineMode() ? parseInlineMode(parentNode, region) : parseBlockMode(parentNode, region);

	if (!succ)
		reader->setCursor(oldCursor);
	reader->selectRegion(oldRegion);
	return succ;
}

bool YukiDirective::match()
{
	yuki_file_reader* reader = getFileReader();
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
	yuki_file_reader* reader = getFileReader();
	yuki_cursor cursor = reader->getCursor();

	m_bodyRegion = nullptr;
	m_argumentsRegion = nullptr;
	m_optionListRegion = nullptr;
	m_isFirstLine = true;
	bool succ = false;

	if (!reader->cursorAtLineEnd())
	{
		// 第一行有内容，并且不接受 arguments，则直接识别为 body
		if (!acceptArguments())
		{
			return matchBody();
		}
		// 第一行有内容，则应该识别为 arguments
		matchArguments();
		matchOptionList();
		matchBody();
	}
	else
	{
		// 第一行没有内容，尝试识别 option list
		if (!reader->moveToNextLine())
			return true;

		m_isFirstLine = false;
		if (!matchOptionList())
		{
			// option list 识别失败的话，则识别为 arguments 或者 body
			if (acceptArguments())
				matchArguments();
			else
				matchBody();
		}
		else
		{
			// option list 识别成功之后，识别 body
			matchBody();
		}
	}

	return true;
}

bool YukiDirective::parseBlockMode(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	
	if (!matchNoBackwardBlockMode())
		return false;

	YukiNode* node = getMainDirectiveNode();
	if (m_argumentsRegion != nullptr)
	{
		reader->setCursor(m_argumentsCursor);
		parseArguments(node, m_argumentsRegion);
	}

	if (m_optionListRegion != nullptr)
	{
		reader->setCursor(m_optionListCursor);
		parseOptionLists(node, m_optionListRegion);
	}

	if (m_bodyRegion != nullptr)
	{
		reader->setCursor(m_bodyCursor);
		parseBody(node, m_bodyRegion);
	}

	return true;
}

bool YukiDirective::matchArguments()
{
	yuki_file_reader* reader = getFileReader();
	if (!acceptArguments())
		return false;

	bool needOptionList = acceptOptionLists();
	int commonIndent = m_isFirstLine ? INT_MAX : reader->getLine()->getIndent();
	YukiStruct* parser = getParser(L"option_list");
	m_argumentsCursor = reader->getCursor();

	while (reader->moveToNextLine())
	{
		const yuki_line_string* line = reader->getLine();
		// 遇到空行，表示 arguments 部分结束
		if(line->isBlankLine())
			break;
		// 如果尝试 option list 成功，则结束
		if (needOptionList && line->getIndent() <= commonIndent)
		{
			if (parser->match())
				break;
		}
	}

	// 记录 arguments 的区域
	m_argumentsRegion = reader->cutRegionToCursorFrom(m_argumentsCursor);
	return true;
}

bool YukiDirective::matchOptionList()
{
	yuki_file_reader* reader = getFileReader();
	if (!acceptOptionLists())
		return false;

	YukiOptionList* parser = dynamic_cast<YukiOptionList*>(getParser(L"option_list"));
	if (!parser->match())
		return false;

	m_optionListCursor = reader->getCursor();
	m_optionListRegion = parser->testValidRegion();

	return true;
}

bool YukiDirective::matchBody()
{
	yuki_file_reader* reader = getFileReader();
	if (!acceptBody())
		return false;

	m_bodyCursor = reader->getCursor();
	m_bodyRegion = reader->cutRegionFromCursorToEnd();

	return true;
}
