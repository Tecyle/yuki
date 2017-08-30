#pragma once

class YukiSettings;
class YukiStructCollection;
class YukiMatcherCollection;
class YukiFileReader;

class YukiGlobal
{
public:
	YukiGlobal();

public:
	__inline YukiStructCollection* getStructCollection(const wchar_t* name) { return &m_structCollection; }
	__inline YukiFileReader* getFileReader() { return &m_fileReader; }
	__inline YukiMatcherCollection* getMatcherCollection() { return &m_matcherCollection; }

private:
	YukiSettings m_settings;
	YukiStructCollection m_structCollection;
	YukiFileReader m_fileReader;
	YukiMatcherCollection m_matcherCollection;
};