#pragma once

/*
	yuki 结构的名称
*/
#define yuki_document_name								"document"
#define yuki_section_name								"section"
#define yuki_body_name									"body"
#define yuki_rst_header_name							"rst_header"
#define yuki_md_header_name								"md_header"
#define yuki_inline_block_name							"inline_block"
#define yuki_inline_markup_name							"inline_markup"
#define yuki_inline_bold_italic_name					"inline_bold_italic"
#define yuki_inline_code_name							"inline_code"
#define yuki_inline_deleted_name						"inline_deleted"
#define yuki_inline_emphasis_name						"inline_emphasis"
#define yuki_inline_format_name							"inline_format"
#define yuki_inline_key_name							"inline_key"
#define yuki_inline_marked_name							"inline_marked"
#define yuki_inline_strong_name							"inline_strong"
#define yuki_inline_sub_name							"inline_sub"
#define yuki_inline_sup_name							"inline_sup"
#define yuki_inline_underline_name						"inline_underline"
#define yuki_link_uri_name								"link_uri"
#define yuki_plain_text_name							"plain_text"
#define yuki_reserved_text_name							"reserved_text"
#define yuki_simple_reference_name_name					"simple_reference_name"
#define yuki_doctest_block_name							"doctest"
#define yuki_literal_block_name							"literal_block"
#define yuki_rst_literal_block_name						"rst_literal_block"
#define yuki_md_literal_block_name						"md_literal_block"
#define yuki_quote_block_name							"quote_block"
#define yuki_rst_quote_block_name						"rst_quote_block"
#define yuki_md_quote_block_name						"md_quote_block"
#define yuki_paragraph_block_name						"paragraph"
#define yuki_comment_block_name							"comment"
#define yuki_directives_block_name						"directives"
#define yuki_explicit_markup_block_name					"explicit_markup"
#define yuki_footnote_citation_definition_block_name	"footnote_citation_definition"
#define yuki_hyperlink_target_block_name				"hyperlink_target"
#define yuki_substitution_definition_block_name			"substitution_definition"

#if !defined(_T)
#	define _T(s) L#s
#endif