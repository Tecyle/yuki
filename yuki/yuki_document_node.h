#pragma once

/*
	yuki_document_node
	�ĵ��ڵ�

	�ĵ��ڵ������нڵ�ĸ��ڵ㣬�ĵ��ڵ㲻��ӵ�и��ڵ㣬��ӵ��
	ȫ���ĵ����ԡ�
*/
class yuki_document_node : public yuki_node
{
public:
	yuki_document_node();
	virtual ~yuki_document_node();

public:
	__inline void setAuthor(const wstring& author) { m_author = author; }
	__inline const wstring& getAuthor() const { return m_author; }

	__inline void setAuthors(const wstring& authors) { m_authors = authors; }
	__inline const wstring& getAuthors() const { return m_authors; }

	__inline void setOrganization(const wstring& org) { m_organization = org; }
	__inline const wstring& getOrganization() const { return m_organization; }

	__inline void setContact(const wstring& contact) { m_contact = contact; }
	__inline const wstring& getContact() const { return m_contact; }

	__inline void setAddress(const wstring& address) { m_address = address; }
	__inline const wstring& getAddress() const { return m_address; }

	__inline void setVersion(const wstring& version) { m_version = version; }
	__inline const wstring& getVersion() const { return m_version; }

	__inline void setStatus(const wstring& status) { m_status = status; }
	__inline const wstring& getStatus() const { return m_status; }

	__inline void setDate(const wstring& date) { m_date = date; }
	__inline const wstring& getDate() const { return m_date; }
	
	__inline void setCopyright(const wstring& copyright) { m_copyright = copyright; }
	__inline const wstring& getCopyright() const { return m_copyright; }

	__inline void setAbstract(const wstring& abst) { m_abstract = abst; }
	__inline const wstring& getAbstract() const { return m_abstract; }

	__inline void setTitle(const wstring& title) { m_title = title; }
	__inline const wstring& getTitle() const { return m_title; }

private:
	// �ĵ�����
	wstring m_author;				///< ����
	wstring m_authors;				///< ��������
	wstring m_organization;			///< ��������
	wstring m_contact;				///< ��ϵ��ʽ
	wstring m_address;				///< ͨ�ŵ�ַ
	wstring m_version;				///< �ĵ��汾��
	wstring m_status;				///< �ĵ�״̬
	wstring m_date;					///< �ĵ���������
	wstring m_copyright;			///< �ĵ���Ȩ��Ϣ
	wstring m_abstract;				///< �ĵ�ժҪ

	wstring m_title;				///< �ĵ�����
};