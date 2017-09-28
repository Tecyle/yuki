#pragma once

#include "yuki_node.h"
#include "yuki_struct.h"

#define yuki_between(val, l, r) ((val) >= (l) && (val) < (r))

enum YukiDocType
{
	Yuki_article,		///< ��ͨ�������£�����
	Yuki_slide			///< ͼƬչʾ���Ų���־
};

enum YukiBlockRegionType
{
	Yuki_lineRegion,	///< ������ָ�� (startLine, startCol) ��ʼ������ (endLine, endCol) ���ַ�����
	Yuki_blockRegion	///< ������ָ (startLine, startCol) �� (endLine, endCol) Χ�ɵľ��οռ�
};

struct YukiBlockRegion
{
	YukiBlockRegionType type;
	int startLineNum;
	int endLineNum;			///< �����䣬������ endLineNum ��һ��
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