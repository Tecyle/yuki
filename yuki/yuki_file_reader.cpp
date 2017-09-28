#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki_file_string.h"
#include "yuki_file_reader.h"
#include "yuki_region_string.h"
#include "yuki_line_string.h"
#include "yuki_region_manager.h"

class yuki_region_stack_manager
{
public:
	yuki_region_stack_manager();
	~yuki_region_stack_manager();

public:
	bool openFile(const wchar_t* fileName);
	void pushRegion(const yuki_region* region);
	void popRegion();

	__inline const yuki_region_string* getCurrentRegionString() const { return m_currentRegionString; }
	__inline yuki_file_string* getFileString() { return &m_fileString; }

private:
	void releaseStack();

private:
	yuki_file_string m_fileString;
	stack<const yuki_region_string*> m_regionStack;
	const yuki_region_string* m_currentRegionString;
};

yuki_region_stack_manager::yuki_region_stack_manager()
{
}

yuki_region_stack_manager::~yuki_region_stack_manager()
{
	releaseStack();
}

bool yuki_region_stack_manager::openFile(const wchar_t* fileName)
{
	releaseStack();
	if (!m_fileString.loadFromFile(fileName))
		return false;

	pushRegion(m_fileString.allocRegionString(nullptr));
	return true;
}

void yuki_region_stack_manager::pushRegion(const yuki_region* region)
{
	m_currentRegionString = m_fileString.allocRegionString(region);
	m_regionStack.push(m_currentRegionString);
}

void yuki_region_stack_manager::popRegion()
{
	delete m_regionStack.top();
	m_regionStack.pop();
	assert(!m_regionStack.empty());
	m_currentRegionString = m_regionStack.top();
}

void yuki_region_stack_manager::releaseStack()
{
	while (!m_regionStack.empty())
	{
		delete m_regionStack.top();
		m_regionStack.pop();
	}
}

//////////////////////////////////////////////////////////////////////////
// yuki_file_reader
yuki_file_reader::yuki_file_reader()
{
	m_regions = new yuki_region_stack_manager();
}

yuki_file_reader::~yuki_file_reader()
{
	delete m_regions;
}

bool yuki_file_reader::openFile(const wchar_t* fileName)
{
	if (!m_regions->openFile(fileName))
		return false;
	m_cursor.setCursor(0, 0, 0, 0);
	return true;
}

void yuki_file_reader::pushRegion(const yuki_region* region)
{
	m_regions->pushRegion(region);
}

void yuki_file_reader::popRegion()
{
	m_regions->popRegion();
}

int yuki_file_reader::skipBlankLines()
{
	auto regionString = m_regions->getCurrentRegionString();

	const yuki_line_string* line = regionString->getLineStringAtCursor(m_cursor);
	int blankLineCount = 0;

	while (line->isBlankLine())
	{
		blankLineCount++;
		if(!moveToNextLine())
			break;
		line = regionString->getLineStringAtCursor(m_cursor);
	}

	return blankLineCount;
}

const yuki_region* yuki_file_reader::cutRegionFromCursorTo(const yuki_cursor& cursor, int indent)
{
	return yukiRegionManager()->allocFromSubRegion(m_regions->getFileString(),
		getRegion(),
		m_cursor,
		cursor,
		indent < 0 ? getRegion()->getIndent() : indent);
}

const yuki_region* yuki_file_reader::cutRegionFromCursorToEnd(int indent /*= -1*/)
{
	return yukiRegionManager()->allocFromSubRegion(m_regions->getFileString(),
		getRegion(),
		m_cursor,
		getRegion()->end(),
		indent < 0 ? getRegion()->getIndent() : indent);
}

const yuki_region* yuki_file_reader::cutRegionToCursorFrom(const yuki_cursor& cursor, int indent /*= -1*/)
{
	return yukiRegionManager()->allocFromSubRegion(m_regions->getFileString(),
		getRegion(),
		cursor,
		m_cursor,
		indent < 0 ? getRegion()->getIndent() : indent);
}

const yuki_region* yuki_file_reader::cutRegionBetween(const yuki_cursor& start, const yuki_cursor& end, int indent /*= -1*/)
{
	return yukiRegionManager()->allocFromSubRegion(m_regions->getFileString(),
		getRegion(),
		start,
		end,
		indent < 0 ? getRegion()->getIndent() : indent);
}

const yuki_region* yuki_file_reader::getRegion()
{
	return m_regions->getCurrentRegionString()->getRegion();
}

int yuki_file_reader::getLineCount() const
{
	return m_regions->getCurrentRegionString()->getLineCount();
}

const yuki_line_string* yuki_file_reader::getLine(int offset /*= 0*/) const
{
	auto regionString = m_regions->getCurrentRegionString();

	int ln = m_cursor.ln - regionString->getRegion()->begin().ln + offset;
	if (ln >= regionString->getLineCount())
		return nullptr;

	return regionString->getLine(ln);
}

const yuki_cursor& yuki_file_reader::getCursor() const
{
	return m_cursor;
}

void yuki_file_reader::setCursor(const yuki_cursor& cursor)
{
	m_cursor = cursor;
}

bool yuki_file_reader::matchStr(const wchar_t* str)
{

}

bool yuki_file_reader::moveToNextLine()
{
	auto line = getLine(1);
	if (line == nullptr)
		return false;

	m_cursor = line->beginCursor();
	return true;
}
