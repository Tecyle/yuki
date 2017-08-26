#pragma once

class YukiLineString;

class YukiFileString
{
public:
	friend class YukiLineString;
	YukiFileString();
	~YukiFileString();

	bool loadFromFile(const wchar_t* fileName);

	// Properties
public:
	// -1 �������һ�У��Դ�����
	__inline const YukiLineString* getLine(int index);
	__inline const int getLineCount() const { return m_lineCount; }
	const YukiRegionString* getRegionString(const YukiRegion* region);

protected:
	bool buildFromBuffer();

private:
	wchar_t* m_buffer;	///< ԭʼ�ı���Ϣ
	int m_bufferLength;	///< ԭʼ�ı�����

	YukiLineString* m_lines;	///< �洢���е�����Ϣ
	int m_lineCount;	///< һ���ж�����
};