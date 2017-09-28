#pragma once

#include "yuki_node.h"
#include "yuki_struct.h"

#define yuki_between(val, l, r) ((val) >= (l) && (val) < (r))

enum YukiDocType
{
	Yuki_article,		///< 普通博客文章，论文
	Yuki_slide			///< 图片展示，炫彩日志
};

enum YukiBlockRegionType
{
	Yuki_lineRegion,	///< 行区域，指从 (startLine, startCol) 开始连续到 (endLine, endCol) 的字符区间
	Yuki_blockRegion	///< 块区域，指 (startLine, startCol) 到 (endLine, endCol) 围成的矩形空间
};

struct YukiBlockRegion
{
	YukiBlockRegionType type;
	int startLineNum;
	int endLineNum;			///< 开区间，不包括 endLineNum 这一行
	int startColNum;
	int endColNum;
	int startIndex;
	int endIndex;
	int indent;

	YukiBlockRegion() { startLineNum = 0; endLineNum = -1; indent = -1; }
	inline bool invalid() { return startLineNum > endLineNum || indent < 0; }
	inline bool isLineOutOfRegion(int lineNum) const { return endLineNum >= 0 && lineNum >= endLineNum; }
	inline bool inRegion(int lineNum, int colNum) const {
		if (type == Yuki_lineRegion)
		{
			if(lineNum < startLineNum || (endLineNum >= 0 && lineNum >= endLineNum))
				return false;

			if (lineNum == startLineNum)
				return colNum >= startColNum;
			else if (endLineNum < 0)
				return true;
			else if (lineNum == endLineNum - 1)
				return endColNum < 0 || colNum < endColNum;
			else
				return true;
		}
		else if (type == Yuki_blockRegion)
		{
			assert(endLineNum >= 0 && endColNum >= 0);
			return yuki_between(lineNum, startLineNum, endLineNum)
				&& yuki_between(colNum, startColNum, endColNum);
		}

		return false;
	}
};

namespace yuki
{
	bool convert();
}