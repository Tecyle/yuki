#pragma once

class yuki_file_string;

class yuki_region_string
{
public:
	yuki_region_string(yuki_file_string* parent, const yuki_region* region);
	~yuki_region_string();

public:
	/*
		��ȡ cursor λ�ö�Ӧ�� line�������αꡣ
		�����ȡʧ�ܣ����� nullptr��
	*/
	const yuki_line_string* getLineStringAtCursor(const yuki_cursor& cursor) const;

	/*
		����� ln �����������
	*/
	const yuki_line_string* getLine(int ln) const { return m_lines[ln]; }
	int getLineCount() const { return m_lineCount; }

	__inline const yuki_region* getRegion() const { return m_region; }

protected:
	/*
		�����������캯�������� file string �е��ã�Ϊ���Ǳ��ⲻ��Ҫ�Ĺ���
		line string ���顣���ԣ������ͨ�������������캯������Ļ���������
		ȥ�ͷ� line string ���顣
	*/
	yuki_region_string(yuki_file_string* parent, const yuki_region* region,
		const yuki_line_string** lines, int lineCount)
		: m_parent(parent)
		, m_region(region)
		, m_lines(lines)
		, m_lineCount(lineCount)
		, m_needRelease(false)
	{}
	yuki_region_string(const yuki_region_string* r);

	void buildLineStringArray();

protected:
	yuki_file_string* m_parent;
	const yuki_region* m_region;
	const yuki_line_string** m_lines;
	int m_lineCount;
	bool m_needRelease;

	friend class yuki_file_string;
};
