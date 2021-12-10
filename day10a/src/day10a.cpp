#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class ChunkParser {
	bool is_corrupted = false;
	bool is_incomplete = false;
	int syntax_error_score = 0;

	bool isClosingChar(char c) {
		return (c == '}') || (c == ')') || (c == ']') || (c == '>');
	}

	char getClosingChar(char opening_char) {
		char c;
		switch (opening_char) {
			case '{': c = '}'; break;
			case '(': c = ')'; break;
			case '[': c = ']'; break;
			case '<': c = '>'; break;
		}
		return c;
	}

	void setSyntaxErrorScore(char c) {
		switch (c) {
			case ')': this->syntax_error_score = 3; break;
			case ']': this->syntax_error_score = 57; break;
			case '}': this->syntax_error_score = 1197; break;
			case '>': this->syntax_error_score = 25137; break;
		}
	}

	void parseChunk(string &str, char chunk_type) {
		while (!this->is_corrupted && !this->is_incomplete) {
			char c = str[0];
			cout << c;
			str.erase(0, 1);
			if (isClosingChar(c)) {
				if (c == getClosingChar(chunk_type)) {
					break;
				}
				// CORRUPT
				cout << " CORRUPT" << endl;
				this->is_corrupted = true;
				setSyntaxErrorScore(c);
				break;
			}
			if (str.length() > 0) {
				parseChunk(str, c);
			} else {
				// INCOMPLETE
				cout << " INCOMPLETE" << endl;
				this->is_incomplete = true;
			}
		}
	}

	public:
	ChunkParser(string &str) {
		parseChunk(str, str[0]);
	}

	bool isCorrupted() {
		return this->is_corrupted;
	}

	int getSyntaxErrorScore() {
		return this->syntax_error_score;
	}
};

class NavSys {
	vector<string> input;
	int score = 0;

	void parseLine(string &str) {
	}

	void findCorruptChunks() {
		for (auto &str: this->input) {
			ChunkParser parser(str);
			if (parser.isCorrupted()) {
				this->score += parser.getSyntaxErrorScore();
			}
		}
	}

	public:
	NavSys(vector<string> &input) {
		this->input = input;
		findCorruptChunks();
	}

	int getAnswer() {
		return this->score;
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day10a <input.txt>" << endl;
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
	NavSys ns(input);
	int answer = ns.getAnswer();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}
