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
	ʶ��Ҫ�㣺
	1. ʶ�� simple-reference-name ������� ``::`` �Լ��ո����ʶ��ɹ�
	2. ��һ����������ݣ���ʶ��Ϊ <arguments>������趨������ <arguments>��
	   ��ֱ��ʶ��Ϊ body
	3. ��ʼʶ�� <arguments> ֮��û���������е�ǰ���£��ڶ��е����ݳ���ʶ
	   ��Ϊ <option list>�����ʧ�������ʶ��Ϊ <arguments>�����ҵڶ��е�
	   ������ʼ���㹫������
	4. �������� 3��û���������е�ǰ���£������������С�ڵ��� <arguments>
	   �Ĺ�������������ʶ��Ϊ <option list>���������ʶ��Ϊ <arguments>
	5. ʶ��Ϊ <option list> ֮�󣬺��������� <option list> ���ֵ�����ȫ��
	   ���� <body> ����
	6. ����趨������ <option list>�������֮ǰ�Ĳ���ȫ����Ϊ <arguments>
	   ʶ��
	7. �����һ��û�����ݣ���ڶ��п�ʼ������ʶ�� <option list>�����ʧ�ܣ�
	   ���ҽ��� <arguments>����ʶ��Ϊ <arguments>������ʶ��Ϊ <body>�����
	   �ɹ�����ʶ��Ϊ <option list>������Ϊ <arguments> û�и�����
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
		// ��һ�������ݣ����Ҳ����� arguments����ֱ��ʶ��Ϊ body
		if (!acceptArguments())
		{
			return matchBody();
		}
		// ��һ�������ݣ���Ӧ��ʶ��Ϊ arguments
		matchArguments();
		matchOptionList();
		matchBody();
	}
	else
	{
		// ��һ��û�����ݣ�����ʶ�� option list
		if (!reader->moveToNextLine())
			return true;

		m_isFirstLine = false;
		if (!matchOptionList())
		{
			// option list ʶ��ʧ�ܵĻ�����ʶ��Ϊ arguments ���� body
			if (acceptArguments())
				matchArguments();
			else
				matchBody();
		}
		else
		{
			// option list ʶ��ɹ�֮��ʶ�� body
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
		// �������У���ʾ arguments ���ֽ���
		if(line->isBlankLine())
			break;
		// ������� option list �ɹ��������
		if (needOptionList && line->getIndent() <= commonIndent)
		{
			if (parser->match())
				break;
		}
	}

	// ��¼ arguments ������
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
