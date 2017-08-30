// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����

#ifdef _WIN32 || _Wow64
#	include <windows.h>
#endif

// TODO: �ڴ˴����ó�����Ҫ������ͷ�ļ�
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