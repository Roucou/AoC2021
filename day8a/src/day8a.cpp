#include <array>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const static regex REGEX_PATTERN = regex("([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) \\| ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+)");

class SegmentSearch {
	void parse(vector<string> &input) {
		int answer = 0;
		for (auto &str: input) {
			smatch matches;
			bool match = regex_match(str, matches, REGEX_PATTERN);
			if (match) {
				for (size_t i = 1; i < matches.size(); i++) {
					string token = matches[i];
					//cout << to_string(i) << "). " << token << endl;
					if (i > 10) {
						int length = token.length();
						switch(length) {
							case 2:
							case 3:
							case 4:
							case 7: answer++;
						}
					}
				}
			} else {
				cout << "Oops, " << str << " doesn't match regex!" << endl;
			}
		}
		cout << "Answer: " << to_string(answer) << endl;
	}

	public:
	SegmentSearch(vector<string> &input) {
		parse(input);
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day8a <input.txt>" << endl;
		return -1;
	}

	// Open file
	string path = argv[1];
	ifstream file(path);

	// Read file
	string str;
	vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	SegmentSearch ss(input);
	//int answer = -1;
	//cout << "Answer: " << to_string(answer) << endl;

	return 0;
}

