#pragma once

/*
	Doctest 块的识别要点：

	1. 以 ``>>>`` 和空格打头，就认为是 doctest 块
	2. 第二行起，如果遇到 ``...`` 和空格打头的，认为是代码部分，
	   否则，全部认为是输出部分
	3. 遇过遇到了空行，则结束 doctest 部分
*/
class yuki_doctest_block : public yuki_structure_parser
{
public:
	yuki_doctest_block(yuki_session* globalData) : yuki_structure_parser(globalData) {}
	virtual ~yuki_doctest_block() {}

public:
	virtual bool parse(yuki_node* parentNode, const yuki_region* region) override;
	virtual bool match() override;

protected:
	virtual bool matchNoBackward();
};