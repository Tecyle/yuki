#include "stdafx.h"
#include "yuki_document.h"
#include "yuki_interpreter.h"

YukiDocument* YukiInterpreter::parseYukiToTree()
{
	YukiDocument* doc = new YukiDocument(&m_fileLoader, 0);
	doc->parse(nullptr);
	return doc;
}
