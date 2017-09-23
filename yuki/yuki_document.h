#pragma once

class YukiDocumentNode : public YukiNode
{
public:
	YukiDocumentNode(const yuki_region* region);

private:
	// Œƒµµ Ù–‘
	wstring m_author;
	wstring m_authors;
	wstring m_organization;
	wstring m_contact;
	wstring m_address;
	wstring m_version;
	wstring m_status;
	wstring m_date;
	wstring m_copyright;
	wstring m_abstract;

	wstring m_title;
};

class YukiDocument : public YukiStruct
{
public:
	YukiDocument(YukiGlobal* globalData);

public:
	YukiDocumentNode* parseYukiDocument();

protected:
	virtual bool parse(YukiNode* parent, const yuki_region* region) override;
};