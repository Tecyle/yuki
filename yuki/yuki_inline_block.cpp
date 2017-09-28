#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_file_reader.h"
#include "yuki_line_string.h"
#include "yuki_struct.h"
#include "yuki_matcher.h"
#include "yuki_plain_text.h"
#include "yuki_inline_block.h"

/*
	�����ṹ����Ҫ�㣺

	һ������������Щ������ǣ�

	1. ��ʽ���
	
		* ``code``
		* *emphasis*
		* **strong**
		* ```key```
		* --deleted--
		* __underline__
		* ==mark==
		* ***bold and italic***
		* x ^^2^^
		* H vv2vv O

	2. �ṹ���

		* ![role](text)
		* ![role][reference]
		* <abbr>(abbreviation)
		* |substitution|
		* |substitution link|_
		* _`inline internal target`
		* `hyper link reference`_  word_  `link <inline>`_  anonymous__
		* [foot note]_
		* `preintepreted`  :role:`text`  `text`:role:
		* [markdown hyper link](somewhere)
		* standalone link target

	ʶ���ԭ��

	1. ������ǵĿ�ʼǰ׺��������ŵ��ַ����벻���ǿհ��ַ���
	2. ������ǵĽ�����׺ǰ������ŵ��ַ����벻���ǿհ��ַ���
	3. ������ǵĿ�ʼǰ׺�ͽ�����׺֮�����������һ���ַ������仰˵����
	   ������������õ����ݲ����ǿյġ�
	4. ������ǵĿ�ʼǰ׺�ͽ�����׺֮ǰ�����ܽ�����һ��δת��ķ�б��(��
	   һ������������Ǿ����������ı��У���������з�б�ܵ�ת�庬�壬����
	   �������ı��Ľ�����׺֮ǰ�ǿ��Գ��ַ�б�ܵ�)��
	5. ���һ����ʼǰ׺֮ǰ������һ����ʼ����(ASCII �ַ����е� ``' " < ( [ {``
	   �����Ƿ� ASCII �ַ����е����Ƶķ���)����ô�����ʼǰ׺�ĺ���Ͳ��ܽ�����
	   ��Ӧ�Ĺرշ��� (ASCII �ַ����е� ' " > ) ] } �����Ƿ� ASCII �ַ����е���
	   �Ƶķ���)��Ŀǰ��yuki ��֧�� ASCII �Ŀ�ʼ���ź͹رշ��š�
	6. ���û�п��� simple-inline-markup ѡ��Ļ�����ô��������ǵĿ�ʼǰ׺����
	   ��һ���ı�����ʼ�������ǽ��������·��ź��棺
			
			* �հ��ַ�
			* ���б����ţ�- : / ' " < ( [ { �����������Ƶķ� ASCII �ַ����ı�����
	
	7. ���û�п��� simple-inline-markup ѡ��Ļ�����ô��������ǵĽ�����׺����
	   ��һ���ı���Ľ�β�����ߺ�����������·��ţ�
	
			* �հ��ַ�
			* ���б����ţ�- . , : ; ! ? \ / ' " ) ] } > �����������Ƶķ� ASCII �ַ����ı�����

	�����·��ű���Ϊ��ʼ���ţ�
	
	* ` - _ = ^ v ! < | [ :

	ʶ���˳��

	1. 
*/
static const wchar_t* g_startCharSet = L"*`-_=^v!<|[";
static const wchar_t* g_headSet = L"-:/'\"<([{��������������";
static const wchar_t* g_fllowSet = L"-.,:;!?\\/'\")]}����������������������";

bool YukiInlineBlock::parse(YukiNode* parentNode, const yuki_region* region)
{
	yuki_file_reader* reader = getFileReader();
	yuki_cursor startCursor = reader->getCursor();
	wchar_t ch;
	bool isLastCharInHeadSet = true;
	bool isLastCharSlash = false;
	const yuki_region* oldRegion = reader->selectRegion(region);

	for(;;)
	{
		ch = reader->getChar();
		// �жϵ�ǰ�Ƿ���Խ���������Ǽ��
		if ((yuki_simple_inline_markup() || isLastCharInHeadSet)
			&& !isLastCharSlash
			&& wcschr(g_startCharSet, ch) != nullptr)
		{
			if (parseInlineMarkup(parentNode, startCursor))
			{
				continue;
			}
		}
		// �жϸ��ַ������Ƿ���Ը����������
		if (!yuki_simple_inline_markup())
		{
			isLastCharInHeadSet = (isspace(ch) || wcschr(g_headSet, ch));
		}
		isLastCharSlash = ch == '\\';
		
		if(!reader->moveToNextChar())
			break;
	}

	// ���һ�� plain_text ����
	getParser(L"plain_text")->parse(parentNode, reader->cutRegionFromCursorToEnd());
	reader->selectRegion(oldRegion);
	return true;
}

bool YukiInlineBlock::parseInlineMarkup(YukiNode* parentNode, yuki_cursor& formerCursor)
{
	yuki_file_reader* reader = getFileReader();
	wchar_t ch = reader->getChar();
	yuki_cursor cursor = reader->getCursor();

	for (auto matcherName : *getMatcherCollection()->getInlineFollowSet(ch))
	{
		if (getParser(matcherName)->parse(parentNode, reader->getRegion()))
		{
			YukiPlainText* plainText = dynamic_cast<YukiPlainText*>(getParser(L"plain_text"));
			plainText->parseInPenult(parentNode, reader->cutRegionBetween(formerCursor, cursor));
			formerCursor = reader->getCursor();
			return true;
		}
	}

	return false;
}
