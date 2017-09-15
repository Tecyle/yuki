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
