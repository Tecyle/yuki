#pragma once

/*
	编码规范：
	1. 所有 yuki 的类型名采用全小写加下划线的形式，变量名中间不加下划线。
*/

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

#ifdef _WIN32 || _Wow64
#	include <windows.h>
#endif

// TODO: 在此处引用程序需要的其他头文件
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <limits.h>
#include "yuki_string.h"

#include <vector>
#include <string>
#include <stack>
using namespace std;

#include "string_hashmap.h"
#include "yuki_dynamic_class.h"
#include "yuki_global.h"
#include "yuki_tool_kit.h"
#include "yuki_string_match.h"

#define yuki_error(x)
#define yuki_tabSize() (8)
#define yuki_simple_inline_markup() (false)

#define yuki_get_absolute_index(index, total) \
	((index) < 0 ? (total) + (index) : (index))

__inline bool yuki_isWordChar(wchar_t ch)
{
	return isalnum(ch);
}
