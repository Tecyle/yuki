#pragma once

class YukiSettings;
class YukiStructCollection;
class YukiMatcherCollection;
class yuki_file_reader;

class YukiGlobal
{
public:
	YukiGlobal();

public:
	__inline YukiStructCollection* getStructCollection(const wchar_t* name) { return &m_structCollection; }
	__inline yuki_file_reader* getFileReader() { return &m_fileReader; }
	__inline YukiMatcherCollection* getMatcherCollection() { return &m_matcherCollection; }

private:
	YukiSettings m_settings;
	YukiStructCollection m_structCollection;
	yuki_file_reader m_fileReader;
	YukiMatcherCollection m_matcherCollection;
};