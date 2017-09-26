#include "stdafx.h"
#include "yuki_tool_kit.h"

int yukiAlignPosToTabSize(int pos, int tabSize)
{
	int res = (pos + 1 + tabSize) / tabSize;
	res *= tabSize;
	return res - 1;
}