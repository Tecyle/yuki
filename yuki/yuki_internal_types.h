#pragma once

/*
	yuki_curosr
	游标对象

	存储了解析位置（光标位置）的行号，列号，字符序号以及行内字符序号。上述
	符号的值都是指在区域中的绝对位置。

	行号
		表明了当前游标正在哪一行，下标从 0 开始，-1 表示无效的行号。
	列号
		表明了当前游标正在哪一列，下标从 0 开始，-1 表示无效的列号。
	字符序号
		表明了当前游标所在位置在内存中的绝对偏移位置。
	行内字符序号
		表明了当前游标所在位置是这一行的第几个字符（Tab可以跨越多列，但是只
		算一个字符）。

	当上面四个字段中的任意一个字段为 -1 的时候，这个游标就是一个无效的游标。
*/
struct yuki_cursor
{
	int ln;		///< 行号，0 开始
	int col;	///< 列号，0 开始
	int offset;	///< 总体字符序号，0 开始
	int ch;		///< 行内字符序号，0 开始

	yuki_cursor()
		: ln(-1)
		, col(-1)
		, offset(-1)
		, ch (-1)
	{}

	yuki_cursor(int _ln, int _col, int _offset, int _ch)
		: ln(_ln)
		, col(_col)
		, offset(_offset)
		, ch(_ch)
	{}

	void setCursor(int _ln, int _col, int _offset, int _ch) {
		ln = _ln; col = _col; offset = _offset; ch = _ch;
	}

	__inline bool isValid() const {
		return ln >= 0 && col >= 0 && offset >= 0 && ch >= 0;
	}

	bool operator ==(const yuki_cursor& r) const;
	bool operator !=(const yuki_cursor& r) const;
	bool operator <(const yuki_cursor& r) const;
	bool operator <=(const yuki_cursor& r) const;
	bool operator >(const yuki_cursor& r) const;
	bool operator >=(const yuki_cursor& r) const;
};

/*
	YukiRegion
	表示字符区域

	一共存在两种字符区域：
	* 行连续区域，类似于编辑器中的 shift 选区。
	* 块区域，类似于编辑器中的 alt 选区。

	一个区域由以下元素决定：
	* 区域类型标记，标记了这个区域是行连续的还是块状的。
	* 起点游标，指这个区域的起始位置，包括这个游标所在的位置。
	* 终点游标，指这个区域的终点位置，不包括这个游标所在的位置。
	* 缩进，指明区域表示范围内部，每一行起始应该忽略的列数，需要注意的是，
	  可能需要忽略的列数中包含非空格字符，也应该被忽略。

	区域的包含区间是左闭右开的，也就是说包含起点位置但是不包含终点位置。

	当一个区域是行连续的时候，其子区域的绝对区域可以是行连续的也可以是块状
	的；当一个区域是块状的，其子区域一定是块状的。

	当终点游标是无效的时候，指示终点的位置在整个文档的末尾。

	区域位置在大部分时候都是应该是绝对的，仅在内部解析的时候才会临时构建相对
	区域。
*/
class yuki_region
{
protected:
	yuki_region(yuki_region_type type, yuki_cursor startPos, yuki_cursor endPos, int regionIndent)
		: regionType(type)
		, startCursor(startPos)
		, endCursor(endPos)
		, indent(regionIndent)
	{}

	yuki_region(const yuki_region* region);

public:
	__inline yuki_region_type getRegionType() const { return regionType; }
	__inline const yuki_cursor& begin() const { return startCursor; }
	__inline const yuki_cursor& end() const { return endCursor; }
	__inline const yuki_cursor& getLeftEdgeCursor() const { return leftEdgeCursor; }
	__inline const yuki_cursor& getRightEdige() const { return rightEdgeCursor; }
	__inline int getIndent() const { return indent; }

	bool operator ==(const yuki_region& r) const;
	bool operator !=(const yuki_region& r) const;

	bool isInRange(const yuki_cursor& cursor) const;
	bool isOutOfRange(const yuki_cursor& cursor) const;

	__inline bool isValid() const;

protected:
	yuki_region_type regionType;	///< 区域的类型
	yuki_cursor startCursor;		///< 区域的起点游标
	yuki_cursor endCursor;			///< 区域的终点游标
	yuki_cursor leftEdgeCursor;		///< 区域的左边界，仅在 regionType 为 blockRegion 的时候有效
	yuki_cursor rightEdgeCursor;	///< 区域的右边界，仅在 regionType 为 blockRegion 的时候有效
	int indent;						///< 区域的缩进，-1 表示无效的缩进

	friend class yuki_region_allocator;
	friend class yuki_region_manager;
};
