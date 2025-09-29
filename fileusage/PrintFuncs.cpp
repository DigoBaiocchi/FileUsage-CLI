#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>

#include "Structs.hpp"
using namespace std;

void printHelpMenu() {
	int spacing1 = 8;
	int spacing2 = 16;
	int spacing3 = 7;
	cout << "fileusage {v1.0.0} (c) 2025, Rodrigo Baiocchi Ferreira\n\n"
		<< setw(spacing1) << " " << "Usage: fileusage [--help] [--version] [-,c+j#wsrRS(x regularexpression)] [folder]\n"
		<< setw(spacing1) << " " << "switches:\n"
		<< setw(spacing2) << " " << "," << setw(spacing3) << " " << "commas\n"
		<< setw(spacing2) << " " << "c" << setw(spacing3) << " " << "filter c files\n"
		<< setw(spacing2) << " " << "+" << setw(spacing3) << " " << "filter c++ files\n"
		<< setw(spacing2) << " " << "j" << setw(spacing3) << " " << "filter java files\n"
		<< setw(spacing2) << " " << "#" << setw(spacing3) << " " << "filter C# files\n"
		<< setw(spacing2) << " " << "x" << setw(spacing3) << " " << "filter with a regular expression\n"
		<< setw(spacing2) << " " << "w" << setw(spacing3) << " " << "filter web files\n"
		<< setw(spacing2) << " " << "s" << setw(spacing3) << " " << "summary\n"
		<< setw(spacing2) << " " << "r" << setw(spacing3) << " " << "recursively visit subfolders\n"
		<< setw(spacing2) << " " << "R" << setw(spacing3) << " " << "reverse the order of the listing\n"
		<< setw(spacing2) << " " << "S" << setw(spacing3) << " " << "sort by file sizes\n"
		<< setw(spacing1) << " " << "folder\n"
		<< setw(spacing2) << " " << "starting folder or current folder if not provided\n";
}

void printResults(vector<pair<string, fileSizeCount>> const& resultMap, size_t const& sizeWidth, bool printReverse, bool isSummary) {
	auto printItems = [&sizeWidth](auto x) { cout << setw(sizeWidth) << right << x.second.size << " " << x.second.count << " " << x.first << endl; };
	auto totalSize = accumulate(resultMap.begin(), resultMap.end(), 0, [](auto lhs, auto rhs) { return lhs + rhs.second.size; });
	auto totalFiles = accumulate(resultMap.begin(), resultMap.end(), 0, [](auto lhs, auto rhs) { return lhs + rhs.second.count; });
	if (isSummary)
		cout << totalSize << " " << totalFiles << " " << resultMap.size() << endl;
	else if (printReverse)
		for_each(resultMap.rbegin(), resultMap.rend(), printItems);
	else
		for_each(resultMap.begin(), resultMap.end(), printItems);
}