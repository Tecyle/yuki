#pragma once

class YukiFileReader;

class YukiMatcher
{
public:
	/*
		ƥ�亯������Ҫ���㣺

		* ƥ��ɹ�֮��Ҫ�ƶ���굽ƥ����ĩβ
		* ƥ��õ�������Ҫ�� region �����з���
		* ƥ��ɹ����� true��ƥ��ʧ�ܷ��� false������Ҫ�ֶ���ԭ���
	*/
	virtual bool match(YukiFileReader* reader, YukiRegion* region) = 0;
};

class YukiMatcherManager
{

};