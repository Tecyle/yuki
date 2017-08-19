#pragma once

#include "yuki_node.h"
#include "yuki_struct.h"

enum YukiDocType
{
	Yuki_article,		///< 普通博客文章，论文
	Yuki_slide			///< 图片展示，炫彩日志
};

struct YukiBlockRegion
{
	int startLineNum;
	int endLineNum;
	int startColNum;
	int endColNum;
	int indent;

	YukiBlockRegion() { startLineNum = 0; endLineNum = -1; indent = -1; }
	inline bool invalid() { return startLineNum > endLineNum || indent < 0; }
};

template<typename T>
inline T yuki_min(T a, T b) { return a < b ? a : b; }

namespace yuki
{
	bool convert();
}