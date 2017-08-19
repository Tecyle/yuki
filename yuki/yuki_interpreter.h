#pragma once

class YukiInterpreter
{
public:

protected:
	YukiDocument* parseYukiToTree();

private:
	YukiFileLoader m_fileLoader;
};