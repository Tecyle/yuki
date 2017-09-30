#pragma once

class yuki_session_variables;

class yuki_file_reader;

class yuki_session
{
public:
	yuki_session();

public:
	__inline YukiStructCollection* getStructCollection(const wchar_t* name);
	__inline yuki_file_reader* getFileReader();
	__inline YukiMatcherCollection* getMatcherCollection();
	__inline yuki_document_node* getRootNode();

private:
	YukiSettings m_settings;
	YukiStructCollection m_structCollection;
	yuki_file_reader m_fileReader;
	YukiMatcherCollection m_matcherCollection;
};