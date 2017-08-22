#include "stdafx.h"
#include "yuki.h"
#include "yuki_body.h"
#include "yuki_document.h"

YukiDocument::YukiDocument(YukiFileLoader* fileLoader)
	: YukiStruct(fileLoader, nullptr)
{
}

// Document : Body
bool YukiDocument::parse(YukiStruct*)
{
	m_parent = nullptr;
	YukiBlockRegion region;
	return appendChild(new YukiBody(m_fileLoader, &region));
}
