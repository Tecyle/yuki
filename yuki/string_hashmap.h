#pragma once

#include <unordered_map>
using namespace stdext;

struct StringHashmapCmp
{
	bool operator()(const wchar_t* l, const wchar_t* r) const
	{
		return wcscmp(l, r) == 0;
	}
};

struct StringHashmapHash
{
	size_t operator()(const wchar_t* str) const
	{
		return std::_Hash_seq((unsigned char*)str, wcslen(str) * sizeof(wchar_t));
	}
};

template <typename T>
class StringHashmap : public unordered_map<const wchar_t*, T, StringHashmapHash, StringHashmapCmp>
{
};
