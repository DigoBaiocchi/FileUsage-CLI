#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include <algorithm>
#include <unordered_map>

#include "Structs.hpp"

using namespace std;
using namespace std::filesystem;

inline void scanFiles(path const& f, unordered_map <string, fileSizeCount>& resultMap, size_t& width, error_code& er) {
	for (directory_iterator d(f, er), e; d != e; ++d) {
		string ext = d->path().extension().string();

		if (!is_directory(d->status())) {
			auto loc = resultMap.find(ext);
			if (loc == end(resultMap)) {
				resultMap.insert(pair<string, fileSizeCount>(ext, { 1, d->file_size() }));
				width = max(width, to_string(d->file_size()).size());
			}

			else {
				loc->second.count++;
				loc->second.size += d->file_size();
				width = max(width, to_string(loc->second.size).size());
			}
		}
	}
	if (er)
		cout << "Invalid path: " << f << endl;
}

inline void rScan(path const& f, unordered_map <string, fileSizeCount>& resultMap, size_t& width, error_code& er) {
	for (auto d : recursive_directory_iterator(f, directory_options::skip_permission_denied, er)) {
		string ext = d.path().extension().string();

		if (er) return;

		if (!is_directory(d.status())) {
			auto loc = resultMap.find(ext);
			if (loc == end(resultMap)) {
				resultMap.insert(pair<string, fileSizeCount>(ext, { 1, d.file_size() }));
				width = max(width, to_string(d.file_size()).size());
			}

			else {
				loc->second.count++;
				loc->second.size += d.file_size();
				width = max(width, to_string(loc->second.size).size());
			}
		}
	}
}

void printResults(vector<pair<string, fileSizeCount>> const& resultMap, size_t const& sizeWidth, bool printReverse, bool isSummary);

void printHelpMenu();