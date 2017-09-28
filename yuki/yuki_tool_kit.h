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

template<typename T>
__inline T yuki_min(T a, T b) { return a < b ? a : b; }

template<typename T>
__inline T yuki_max(T a, T b) { return a < b ? b : a; }