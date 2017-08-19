#include "stdafx.h"
#include "yuki.h"
#include "yuki_rst_quote_block.h"
#include "yuki_body.h"

/*
	Body �ڵ���������������½ڵ㣺

	1. �����б�
	2. �����б�
	3. �ֶ��б�
	4. ѡ���б�
	5. �����б�
	6. ���б�
	7. Ԥ�����ֿ�
	8. Markdown Ԥ�����ֿ�
	9. ���ÿ�
	10. Markdown ���ÿ�
	11. doctest ��
	12. ��ʾ��ǿ�
	13. �����
	14. �򵥱�
	15. Markdown ��
	16. �ָ���
	17. ����

	�����ڵ������Ϊ��

	1. ���������б�����Ҫ�󣬲��Һ�����ڿո� ��
	2. �� ``*``, ``+`` �� ``-`` ��ͷ���Һ�����ڿո� ��
	3. ���� ``:filed:`` ��ʽ��ͷ�����ڿո� ��
	4. ���������ͷ���������������ո� ��
	5. ��һ�в��ǿ��Ҵ������� ��
	6. �� ``|`` ��ͷ�Ҵ��ڿո� ��
	7. �� ``::`` ��ͷ�Ҵ��ڿո� ��
	8. �� ``` ��ͷ ��
	9. ����������� ��
	10. �� ``>`` ��ͷ�����ڿո�������� ��
	11. �� ``>>>`` ��ͷ�����ڿո� ��
	12. �� ``..`` ��ͷ�����ڿո� ��
	13. ���������ͷ�� ��
	14. ����򵥱�ͷ�� ��
	15. ������ڴ��� ``|``������Ѱ�ұ����У�����ҵ������� ``|`` ���л��߿��У���ƥ��ʧ�� ��
	16. ����4����ͬ����ͷ ��
	17. �����Ϊ����

	��������������ʶ��˳�����£�

	1. ���������ͷ��ʶ��Ϊ���ÿ�
	2. ����� ``.. `` ��ͷ����ʶ��Ϊ��ʽ��ǿ�
	3. ����� ``` ��ͷ����ʶ��Ϊ Markdown Ԥ�����ֿ�
	4. ����� ``>>> ``��ͷ����ʶ��Ϊ doctest ��
	5. ����� ``>`` ��ͷ����ʶ��Ϊ Markdown ���ÿ�
	6. ����� ``::`` ��ͷ��������һ�к���һ��ֻ�пո���ʶ��ΪԤ�����ֿ�
	7. �����б��Ǵ�ͷ��ʶ��Ϊ�����б�
	8. �����б��Ǵ�ͷ��ʶ��Ϊ�����б�
	9. field ������ͷ��ʶ��Ϊ�����б�
	10. �����ĸ�����ͷ����������������֣���ͬ���ĸ���������ʶ��Ϊ�ָ���
	11. ƥ�������ͷ
	12. ƥ��򵥱�ͷ
	13. ƥ�������ͷ
	14. ��һ����������ʶ��Ϊ�����б�
	15. ������ڷ����״��� ``|``�������¼��У��ж��Ƿ��� Markdown �����
	16. ����� ``| `` ��ͷ����ʶ��Ϊ���б�
	17. Ȼ��ֻʣ������
*/
void YukiBody::parse(YukiStruct* parent)
{
	m_parent = parent;

	for (;;)
	{
		if (outOfRegion())
			break;

		const YukiString* line = m_fileLoader->getLine();

		if(line == nullptr)
			break;

		if (line->isBlankLine())
		{
			m_fileLoader->moveToNextLine();
			continue;
		}

		int indentLevel = line->getIndentLevel();
		// �������С�ڵ�ǰ body ������������Ϊ body �����Ѿ�����
		if (indentLevel < m_indentLevel)
			break;

		if (indentLevel > m_indentLevel)
		{
			// �������������ڵ�ǰ body ��������������Ϊ������ rst �������ÿ�
			appendChildByRegion(new YukiRstQuoteBlock(m_fileLoader, m_indentLevel), m_limitRegion);
		}
		else if (line.matchExplicitMark())
		{
			doExplicitMark();
		}
		else if (line.matchMarkdownLiteralBlockMark())
		{
			appendChild(new YukiMdLiteralBlock());
		}
		else if (line.matchDoctestMark())
		{
			appendChild(new YukiRstDoctestBlock());
		}
		else if (line.matchMarkdownQuoteBlockMark())
		{
			appendChild(new YukiMdQuoteBlock());
		}
		else if(line.matchRstLiteralBlockMark())
		{
			appendChild(new YukiRstLiteralBlock());
		}
		else if (line.matchBulletListMark())
		{
			appendChild(new YukiBulletListMark());
		}
		else if (line.matchNumericListMark())
		{
			appendChild(new YukiNumericList());
		}
		else if (line.matchFieldListMark())
		{
			appendChild(new YukiFieldList());
		}
		else if (line.matchSeperatorMark())
		{
			appendChild(new YukiSeperator());
		}
		else if (line.matchGridTableMark())
		{
			appendChild(new YukiGridTable());
		}
		else if (line.matchSimpleTableMark())
		{
			appendChild(new YukiSimpleTable());
		}
		else if (line.matchOptionListMark())
		{
			appendChild(new YukiOptionList());
		}
		else if (line.matchDefinitionListMark())
		{
			appendChild(new YukiDefinitionList());
		}
		else if (line.matchMarkdownTableMark())
		{
			appendChild(new YukiMarkdownTable());
		}
		else if (line.matchLineListMark())
		{
			appendChild(new YukiLineList());
		}
		else
		{
			appendChild(new YukiParagraph());
		}
	}

}
