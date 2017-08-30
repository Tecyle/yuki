// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

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
using namespace std;

#include "string_hashmap.h"
#include "yuki_global.h"

#define yuki_error(x)
#define yuki_tabSize() (8)
#define yuki_simple_inline_markup() (false)

#define yuki_get_absolute_index(index, total) \
	((index) < 0 ? (total) + (index) : (index))