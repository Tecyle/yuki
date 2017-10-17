#include "stdafx.h"
#include "yuki.h"
#include "yuki_internal_types.h"
#include "yuki_rst_quote_block.h"
#include "yuki_session.h"
#include "yuki_body.h"
#include "yuki_file_reader.h"
#include "yuki_body_node.h"
#include "yuki_structure_parser_collection.h"
#include "yuki_line_string.h"

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

	1. ���������ͷ��ʶ��Ϊ���ÿ� ��
	2. ����� ``.. `` ��ͷ����ʶ��Ϊ��ʽ��ǿ� ��
	3. ����� ``` ��ͷ����ʶ��Ϊ Markdown Ԥ�����ֿ� ��
	4. ����� ``>>> ``��ͷ����ʶ��Ϊ doctest �� ��
	5. ����� ``>`` ��ͷ����ʶ��Ϊ Markdown ���ÿ� ��
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
yuki_body::yuki_body(yuki_session* globalData)
	: yuki_structure_parser(globalData)
{
	m_name = yuki_body_name;
	m_type = yuki_blockStructure;
}


bool yuki_body::parse(yuki_node* parent, const yuki_region* region)
{
	int indentLevel = region->getIndent();
	yuki_file_reader* fileReader = getFileReader();
	fileReader->pushRegion(region);

	yuki_body_node* bodyNode = new yuki_body_node();
	auto followSet = m_session->getStructureParserCollection()->getFollowParsersByName(getName());

	while (!fileReader->outOfRegion())
	{
		const yuki_line_string* line = fileReader->getLine();

		if (line == nullptr)
			break;

		if (line->isBlankLine())
		{
			fileReader->moveToNextLine();
			continue;
		}

		if (line->getIndent() < region->getIndent())
			break;

		bool matched = false;
		for (auto parser : *followSet)
		{
			assert(parser);
			if (!parser->match())
				continue;

			matched = parser->parse(parent, region);
			break;
		}

		assert(matched);
	}

	fileReader->popRegion();
	parent->appendChild(bodyNode);

	return true;
}
