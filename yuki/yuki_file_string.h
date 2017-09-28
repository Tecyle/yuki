#pragma once

class yuki_line_string;

/*
	YukiFileString
	
	��ʾһ�� yuki �ļ��������ַ���Ϣ������һϵ�б�Я���ʺ�����
	���е� region �� line ���Ǵ������������������ġ�
*/
class yuki_file_string
{
public:
	friend class yuki_line_string;
	yuki_file_string();
	~yuki_file_string();

	/*
		��һ���ļ��м��� yuki ����
		TODO Ŀǰֻ֧�� UTF-8���Ժ���չ֧�ֶ��ֱ���
	*/
	bool loadFromFile(const wchar_t* fileName);

	// Properties
public:
	/*
		����һ�У�-1 �������һ��
	*/
	__inline const yuki_line_string* getLine(int index);

	/*
		�����е�����
	*/
	__inline const int getLineCount() const { return m_lineCount; }

	/*
		����һ�� Region�����ض�Ӧ�� RegionString��
		������� nullptr���򷵻�һ�����������ļ��� region string��
	*/
	const yuki_region_string* allocRegionString(const yuki_region* region);

	/*
		��������� offset ���α� target �ľ����α꣬���������
		target �С�
	*/
	void addCursorOffsetToCursor(yuki_cursor& target, const yuki_cursor& offset);

	__inline const wchar_t* getBuffer() const { return m_buffer; }

protected:
	bool buildFromBuffer();
	void buildWholeFileRegionString();

private:
	wchar_t* m_buffer;					///< ԭʼ�ı���Ϣ
	int m_bufferLength;					///< ԭʼ�ı�����

	const yuki_line_string** m_lines;	///< �洢���е�����Ϣ
	int m_lineCount;					///< һ���ж�����

	yuki_region_string* m_wholeFileRegionString;
};