#include "stdafx.h"
#include "yuki.h"
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
bool YukiInlineBlock::parse(YukiStruct* parent)
{

}
