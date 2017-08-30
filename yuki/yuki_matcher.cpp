#include "stdafx.h"
#include "yuki_internal_types.h"
#include "yuki.h"
#include "yuki_matcher.h"

void YukiMatcherCollection::addFollowSet(const wchar_t* parent, const wchar_t* child)
{
	m_followSet[parent].push_back(child);
}

YukiMatcher* YukiMatcherCollection::getMatcher(const wchar_t* parent)
{
	auto it = m_matchers.find(parent);
	return it == m_matchers.end() ? nullptr : it->second;
}

const vector<const wchar_t*>* YukiMatcherCollection::getFollowSet(const wchar_t* parent) const
{
	auto it = m_followSet.find(parent);
	return it == m_followSet.end() ? nullptr : &it->second;
}
