#include "stdafx.h"
#include "yuki.h"
#include "yuki_document.h"

// Document : Body
void YukiDocument::parse(YukiStruct*)
{
	m_parent = nullptr;
	appendChild(new YukiBody(&m_fileLoader, m_indentLevel));
}
