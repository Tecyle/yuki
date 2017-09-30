#pragma once

class YukiDocumentNode : public yuki_node
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

class yuki_document : public yuki_structure
{
public:
	yuki_document(yuki_session* globalData);

public:
	YukiDocumentNode* parseYukiDocument();

protected:
	virtual bool parse(yuki_node* parent, const yuki_region* region) override;
};