#pragma once

#include "yuki_node.h"
#include "yuki_struct.h"

enum YukiDocType
{
	Yuki_article,		///< ��ͨ�������£�����
	Yuki_slide			///< ͼƬչʾ���Ų���־
};

struct YukiBlockRegion
{
	int startLineNum;
	int endLineNum;			///< �����䣬������ endLineNum ��һ��
	int startColNum;
	int endColNum;
	int indent;

	YukiBlockRegion() { startLineNum = 0; endLineNum = -1; indent = -1; }
	inline bool invalid() { return startLineNum > endLineNum || indent < 0; }
	inline bool isLineOutOfRegion(int lineNum) const { return endLineNum >= 0 && lineNum >= endLineNum; }
};

template<typename T>
inline T yuki_min(T a, T b) { return a < b ? a : b; }

namespace yuki
{
	bool convert();
}