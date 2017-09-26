#pragma once

/*
	将一个 pos 对齐到 tabSize 的列号，一个应用的场景就是，如果
	pos 位置出现的是 tab 字符，那么计算的结果就是下一个字符出现
	的列号。

	注意，这里的 pos 和 返回值 指的都是 0 开始的列号。

	例如，tabSize 如果是 8，则：
	0-6 返回的都应该是 7，而
	7-14 返回的都应该是 15，以此类推。
*/
__inline int yukiAlignPosToTabSize(int pos, int tabSize = 8);