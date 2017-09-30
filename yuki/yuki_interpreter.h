#pragma once

class YukiInterpreter
{
public:

protected:
	yuki_document* parseYukiToTree();

private:
	YukiFileLoader m_fileLoader;
};