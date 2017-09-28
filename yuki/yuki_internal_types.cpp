#include "stdafx.h"
#include "yuki_internal_types.h"

bool yuki_cursor::operator==(const yuki_cursor& r) const
{
	return ln == r.ln
		&& col == r.col
		&& offset == r.offset
		&& ch == r.ch;
}

bool yuki_cursor::operator!=(const yuki_cursor& r) const
{
	return !operator ==(r);
}

bool yuki_cursor::operator<(const yuki_cursor& r) const
{
	if (offset == r.offset)
		return col < r.col;
	return offset < r.offset;
}

bool yuki_cursor::operator<=(const yuki_cursor& r) const
{
	return operator<(r) || operator==(r);
}

bool yuki_cursor::operator>(const yuki_cursor& r) const
{
	return !operator<=(r);
}

bool yuki_cursor::operator>=(const yuki_cursor& r) const
{
	return !operator<(r);
}

yuki_region::yuki_region(const yuki_region* region)
{
	regionType = region->regionType;
	startCursor = region->startCursor;
	endCursor = region->endCursor;
	indent = region->indent;
}

/*
	判断一个游标是否在行连续区域内，需要判断行列是否在之间。
*/
static __inline bool _isInLinedRegion(const yuki_cursor& startPos, const yuki_cursor& endPos, const yuki_cursor& cursor)
{
	if (cursor.ln < startPos.ln)
		return false;

	if (cursor.ln == startPos.ln && cursor.col < startPos.col)
		return false;

	if (!endPos.isValid())
		return true;

	if (cursor.ln > endPos.ln)
		return false;

	if (cursor.ln == endPos.ln && cursor.col >= endPos.col)
		return false;

	return true;
}

/*
	判断一个游标是否在一个块区域内，需要判断行列是否分别在范围内
*/
static __inline bool _isInBlockRegion(const yuki_cursor& startPos, const yuki_cursor& endPos, const yuki_cursor& cursor)
{
	if (cursor.ln < startPos.ln || cursor.col < startPos.col)
		return false;

	// 如果块状区域的 endPos 无效则代表块状终点无穷远
	if (!endPos.isValid())
		return true;

	// 行相等但是列小于区域是应该返回成功的
	if (cursor.ln > endPos.ln)
		return false;

	if (cursor.col >= endPos.col)
		return false;

	return true;
}

bool yuki_region::isInRange(const yuki_cursor& cursor) const
{
	if (regionType == Yuki_linedRegion)
		return _isInLinedRegion(begin(), end(), cursor);

	return _isInBlockRegion(begin(), end(), cursor);
}

bool yuki_region::isOutOfRange(const yuki_cursor& cursor) const
{
	return !isInRange(cursor);
}

bool yuki_region::isValid() const
{
	return startCursor.isValid();
}

bool yuki_region::operator!=(const yuki_region& r) const
{
	return !operator ==(r);
}

bool yuki_region::operator ==(const yuki_region& r) const
{
	return regionType == r.regionType
		&& startCursor == r.startCursor
		&& endCursor == r.endCursor
		&& indent == r.indent;
}
