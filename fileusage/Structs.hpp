#pragma once
#include <numeric>

struct fileSizeCount {
	size_t count;
	uintmax_t size;
};

struct Switches {
	bool c = false;
	bool cpp = false;
	bool csharp = false;
	bool j = false;
	bool w = false;
	bool s = false;
	bool x = false;
	bool r = false;
	bool R = false;
	bool S = false;
};