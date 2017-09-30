#pragma once

#include <unordered_map>
using namespace stdext;

struct yuki_string_hashmap_cmp
{
	bool operator()(const wchar_t* l, const wchar_t* r) const
	{
		return wcscmp(l, r) == 0;
	}
};

struct yuki_string_hashmap_hash
{
	size_t operator()(const wchar_t* str) const
	{
		return std::_Hash_seq((unsigned char*)str, wcslen(str) * sizeof(wchar_t));
	}
};

template <typename T>
class yuki_string_hashmap : public unordered_map<const wchar_t*, T, yuki_string_hashmap_hash, yuki_string_hashmap_cmp>
{
};
