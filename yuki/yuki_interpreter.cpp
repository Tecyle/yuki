#include "stdafx.h"
#include "yuki_document.h"
#include "yuki_interpreter.h"

yuki_document* YukiInterpreter::parseYukiToTree()
{
	yuki_document* doc = new yuki_document(&m_fileLoader, 0);
	doc->parse(nullptr);
	return doc;
}
