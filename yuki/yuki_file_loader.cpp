#include "stdafx.h"
#include "yuki.h"
#include "yuki_file_loader.h"

void YukiFileLoader::loadYukiFile(const char* fileName)
{
	FILE* fp = nullptr;
	fopen_s(&fp, fileName, "rt");

	if (fp == nullptr)
	{
		yuki_error(1);
		return;
	}

	free(m_fileBuffer);
	fseek(fp, 0, SEEK_END);
	size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* tpBuffer = (char*)malloc(fileSize + 1);
	size_t readSize = fread_s(tpBuffer, fileSize, 1, fileSize, fp);
	tpBuffer[readSize] = 0;

	m_bufferSize = MultiByteToWideChar(CP_UTF8, NULL, tpBuffer, readSize, NULL, 0);
	m_fileBuffer = (wchar_t*)malloc(m_bufferSize);
	MultiByteToWideChar(CP_UTF8, NULL, tpBuffer, readSize, m_fileBuffer, m_bufferSize);
	free(tpBuffer);

	fclose(fp);

	initStates();

	splitLines();
}

const YukiString* YukiFileLoader::getLine(int offset /*= 0*/)
{
	int index = m_cursor.rowNum + offset;
	if (index < 0 || index >= m_lineCount)
		return nullptr;
	return &m_lines[index];
}

bool YukiFileLoader::moveToNextLine()
{
	if (m_cursor.rowNum < 0 || m_cursor.rowNum >= m_lineCount)
		return false;

	m_cursor.index += m_lines[m_cursor.rowNum].length();
	++m_cursor.rowNum;
	m_cursor.colNum = 0;
	m_cursor.indexInline = 0;

	return true;
}

void YukiFileLoader::splitLines()
{
	free(m_lines);
	m_lineCount = 1;

	wchar_t* p = m_fileBuffer;
	while (*p)
	{
		if (*p == '\n')
			++m_lineCount;
	}

	m_lines = (YukiString*)malloc(m_lineCount);
	p = m_fileBuffer;
	wchar_t* pStart = p;
	size_t lineIndex = 0;
	while (lineIndex < m_lineCount)
	{
		while (*p != '\n')
		{
			++p;
		}

		m_lines[lineIndex].init(pStart, p - pStart);

		++p;
		pStart = p;
		++lineIndex;
	}
}

void YukiFileLoader::initStates()
{
	m_cursor.rowNum = m_cursor.colNum = m_cursor.index = m_cursor.indexInline = 0;
	m_savedCursor = m_cursor;
}

bool YukiFileLoader::match(const wchar_t* str)
{
	const YukiString* line = getLine();
	int matchSize = line->match(str, m_cursor.indexInline);

	if (matchSize == 0)
		return false;

	m_cursor.colNum += matchSize;
	m_cursor.indexInline += matchSize;
	m_cursor.index += matchSize;
	return true;
}

int YukiFileLoader::matchAndSkipSpace()
{
	const YukiString* line = getLine();
	CursorOffset res = line->matchAndSkipSpace(m_cursor.indexInline, m_cursor.colNum);

	m_cursor.colNum += res.colOffset;
	m_cursor.indexInline += res.indexOffset;
	m_cursor.index += res.indexOffset;

	return res.colOffset;
}

int YukiFileLoader::matchIndent()
{

	if (m_cursor.indexInline != 0)
		return -1;

	if (getLine()->isBlankLine())
		return 0;

	return matchAndSkipSpace();
}

int YukiFileLoader::skipBlankLinesInRegion(const YukiBlockRegion* region)
{
	int lineCount = 0;
	while (!region->isLineOutOfRegion(getLineNum()) && getLine()->isBlankLine())
	{
		if (!moveToNextLine())
			break;

		++lineCount;
	}

	return lineCount;
}
