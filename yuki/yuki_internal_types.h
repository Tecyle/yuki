#pragma once

enum yuki_region_type
{
	Yuki_linedRegion,		///< ����������
	Yuki_blockRegion		///< ��״����
};

/*
	yuki_curosr
	�α����

	�洢�˽���λ�ã����λ�ã����кţ��кţ��ַ�����Լ������ַ���š�����
	���ŵ�ֵ����ָ�������еľ���λ�á�

	�к�
		�����˵�ǰ�α�������һ�У��±�� 0 ��ʼ��-1 ��ʾ��Ч���кš�
	�к�
		�����˵�ǰ�α�������һ�У��±�� 0 ��ʼ��-1 ��ʾ��Ч���кš�
	�ַ����
		�����˵�ǰ�α�����λ�����ڴ��еľ���ƫ��λ�á�
	�����ַ����
		�����˵�ǰ�α�����λ������һ�еĵڼ����ַ���Tab���Կ�Խ���У�����ֻ
		��һ���ַ�����

	�������ĸ��ֶ��е�����һ���ֶ�Ϊ -1 ��ʱ������α����һ����Ч���αꡣ
*/
struct yuki_cursor
{
	int ln;		///< �кţ�0 ��ʼ
	int col;	///< �кţ�0 ��ʼ
	int offset;	///< �����ַ���ţ�0 ��ʼ
	int ch;		///< �����ַ���ţ�0 ��ʼ

	yuki_cursor()
		: ln(-1)
		, col(-1)
		, offset(-1)
		, ch (-1)
	{}

	__inline bool isValid() const {
		return ln >= 0 && col >= 0 && offset >= 0 && ch >= 0;
	}

	bool operator ==(const yuki_cursor& r) const;
	bool operator !=(const yuki_cursor& r) const;
};

/*
	YukiRegion
	��ʾ�ַ�����

	һ�����������ַ�����
	* ���������������ڱ༭���е� shift ѡ����
	* �����������ڱ༭���е� alt ѡ����

	һ������������Ԫ�ؾ�����
	* �������ͱ�ǣ����������������������Ļ��ǿ�״�ġ�
	* ����αָ꣬����������ʼλ�ã���������α����ڵ�λ�á�
	* �յ��αָ꣬���������յ�λ�ã�����������α����ڵ�λ�á�
	* ������ָ�������ʾ��Χ�ڲ���ÿһ����ʼӦ�ú��Ե���������Ҫע����ǣ�
	  ������Ҫ���Ե������а����ǿո��ַ���ҲӦ�ñ����ԡ�

	����İ�������������ҿ��ģ�Ҳ����˵�������λ�õ��ǲ������յ�λ�á�

	��һ����������������ʱ����������ľ��������������������Ҳ�����ǿ�״
	�ģ���һ�������ǿ�״�ģ���������һ���ǿ�״�ġ�

	���յ��α�����Ч��ʱ��ָʾ�յ��λ���������ĵ���ĩβ��

	����λ���ڴ󲿷�ʱ����Ӧ���Ǿ��Եģ������ڲ�������ʱ��Ż���ʱ�������
	����
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
	__inline int getIndent() const { return indent; }

	bool operator ==(const yuki_region& r) const;
	bool operator !=(const yuki_region& r) const;

	bool isInRange(const yuki_cursor& cursor) const;
	bool isOutOfRange(const yuki_cursor& cursor) const;

	__inline bool isValid() const;

protected:
	yuki_region_type regionType;	///< ���������
	yuki_cursor startCursor;		///< ���������α�
	yuki_cursor endCursor;			///< ������յ��α�
	int indent;						///< �����������-1 ��ʾ��Ч������

	friend class yuki_region_allocator;
	friend class yuki_region_manager;
};
