#pragma once

/*
	yuki_document_node
	文档节点

	文档节点是所有节点的根节点，文档节点不能拥有父节点，且拥有
	全局文档属性。
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
	// 文档属性
	wstring m_author;				///< 作者
	wstring m_authors;				///< 附加作者
	wstring m_organization;			///< 所属机构
	wstring m_contact;				///< 联系方式
	wstring m_address;				///< 通信地址
	wstring m_version;				///< 文档版本号
	wstring m_status;				///< 文档状态
	wstring m_date;					///< 文档发布日期
	wstring m_copyright;			///< 文档版权信息
	wstring m_abstract;				///< 文档摘要

	wstring m_title;				///< 文档标题
};