#pragma once

/*
	��һ�� pos ���뵽 tabSize ���кţ�һ��Ӧ�õĳ������ǣ����
	pos λ�ó��ֵ��� tab �ַ�����ô����Ľ��������һ���ַ�����
	���кš�

	ע�⣬����� pos �� ����ֵ ָ�Ķ��� 0 ��ʼ���кš�

	���磬tabSize ����� 8����
	0-6 ���صĶ�Ӧ���� 7����
	7-14 ���صĶ�Ӧ���� 15���Դ����ơ�
*/
__inline int yukiAlignPosToTabSize(int pos, int tabSize = 8);