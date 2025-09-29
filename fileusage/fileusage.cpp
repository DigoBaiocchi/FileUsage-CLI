/*
	Name: Rodrigo Baiocchi Ferreira
	Purpose: This is project #3 fileusage. This program will produce a list of files in a particular folder
			 The user can provided several switches which will return different results. In order to know more
			 about the switches enter --help as an argument. This program can also get the data recursing through
			 the files.
	Aug 16, 2025

	Considerations: This program was able to pass all the tests except for #11. I tried different approaches but
					couldn't figure out where is the problem. When I run the arguments "-r sub1" I get the exact
					same results from the fileusage demo for different paths.
*/

#include <filesystem>
#include <regex>

#include "Structs.hpp"
#include "HeadersAndHelperFuncs.hpp"

using namespace std::filesystem;

int main(int argc, char * argv[]) {
	vector<string> vArgs(argv + 1, argv + argc);
	string version = "4.1.1";
	unordered_map <string, fileSizeCount> resultMap;
	size_t width{ 0 };
	path path = ".";
	auto sortFunc = [](auto& lhs, auto& rhs) { return lhs.first < rhs.first; };
	auto sortSizeFunc = [](auto& lhs, auto& rhs) {
		if (lhs.second.size != rhs.second.size)
			return lhs.second.size < rhs.second.size; 
		return lhs.first < rhs.first;
	};
	bool isReverseOrder = false, isSortBySize = false, isSummary = false, isRecursive = false;
	Switches switches;

	regex invalidSwitch("-");
	regex switchesGroup(
		"(^-([c\+\#jwsrRS,x]*)([c\+\#jwsrRS,x]*)$)"
	);
	regex xRegex;

	auto apply_switches = [&switches](string const& s) {
		for (char letter : s) {
			switch (letter) {
			case 'R': switches.R = true; break;
			case ',': { 
				locale here(""); 
				cout.imbue(here); 
			} break;
			case 'S': switches.S = true; break;
			case 's': switches.s = true; break;
			case 'r': switches.r = true; break;
			case 'c': switches.c = true; break;
			case '+': switches.cpp = true; break;
			case '#': switches.csharp = true; break;
			case 'j': switches.j = true; break;
			case 'w': switches.w = true; break;
			case 'x': switches.x = true; break;
			default: break;
			}
		}
	};

	
	if (!vArgs.empty()) {
		string arg1 = vArgs.at(0);
		if (arg1 == "--help") {
			printHelpMenu();
			return EXIT_SUCCESS;
		}

		if (arg1 == "--version") {
			cout << version << endl;
			return EXIT_SUCCESS;
		}

		if (regex_match(arg1, invalidSwitch)) {
			cout << "No switch has been passed.\n";
			return EXIT_FAILURE;
		}

		if (vArgs.size() == 1) {
			if (regex_match(arg1, switchesGroup)) {
				apply_switches(arg1);
			}
			else
				path = canonical(arg1).string();
		}
		if (vArgs.size() == 2) {
			if (regex_match(arg1, switchesGroup)) {
				apply_switches(arg1);
				if (switches.x) {
					xRegex.assign(vArgs.at(1));
				}
				else {
					error_code ec;
					filesystem::path p = vArgs.at(1);
					if (exists(p, ec)) 
						path = absolute(p, ec).string();
					else 
						return EXIT_FAILURE;
				}
			}
			else {
				cout << "Error: Error: too many commandline arguments : " << vArgs.at(1) << "...\n";
				return EXIT_FAILURE;
			}
		}
		else if (vArgs.size() == 3) {
			if (regex_match(arg1, switchesGroup)) {
				apply_switches(arg1);
				xRegex.assign(vArgs.at(1));
				error_code ec;
				filesystem::path p = vArgs.at(2);
				if (exists(p, ec)) 
					path = absolute(p, ec).string();
				else
					return EXIT_FAILURE;
			}
			else {
				cout << "Error: Error: too many commandline arguments : " << vArgs.at(1) << "...\n";
				return EXIT_FAILURE;
			}
		}
	}
			
	error_code er;

	if (switches.r)
		rScan(path, resultMap, width, er);
	else
		scanFiles(path, resultMap, width, er);

	vector<pair<string, fileSizeCount>> vResult(resultMap.begin(), resultMap.end());

	vector<pair<string, fileSizeCount>> vResult2;
	regex regExt(R"(\.(cpp|txt))");
	regex cExt(R"(\.(c|h))");
	regex cppExt(R"(\.(cc|cp|cpp|cxx|c\+\+|hpp|hxx))");
	regex csharpExt(R"(\.(cs|vb|jsl))");
	regex javaExt(R"(\.(class|j|jad|jar|java|jsp|ser))");
	regex wExt(R"(\.(htm|html|html5|js|jse|jsc))");

	if (switches.x)
		copy_if(vResult.begin(), vResult.end(), back_inserter(vResult2), [&xRegex](auto const& result) { return regex_match(result.first, xRegex); });
	else {
		if (switches.c)
			copy_if(vResult.begin(), vResult.end(), back_inserter(vResult2), [&cExt](auto const& result) { return regex_match(result.first, cExt); });
		if (switches.cpp)
			copy_if(vResult.begin(), vResult.end(), back_inserter(vResult2), [&cppExt](auto const& result) { return regex_match(result.first, cppExt); });
		if (switches.csharp)
			copy_if(vResult.begin(), vResult.end(), back_inserter(vResult2), [&csharpExt](auto const& result) { return regex_match(result.first, csharpExt); });
		if (switches.j)
			copy_if(vResult.begin(), vResult.end(), back_inserter(vResult2), [&javaExt](auto const& result) { return regex_match(result.first, javaExt); });
		if (switches.w)
			copy_if(vResult.begin(), vResult.end(), back_inserter(vResult2), [&wExt](auto const& result) { return regex_match(result.first, wExt); });
	}
	
	if (switches.c || switches.cpp || switches.csharp || switches.j || switches.w || switches.x) {
		if (switches.S)
			sort(vResult2.begin(), vResult2.end(), sortSizeFunc);
		else
			sort(vResult2.begin(), vResult2.end(), sortFunc);

		printResults(vResult2, width, switches.R, switches.s);
	}
		
	else {
		if (switches.S)
			sort(vResult.begin(), vResult.end(), sortSizeFunc);
		else
			sort(vResult.begin(), vResult.end(), sortFunc);

		printResults(vResult, width, switches.R, switches.s);
	}
		
}