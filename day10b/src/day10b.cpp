#include <algorithm>
#include <iostream>
#include <fstream>
#include <stack>
#include <string>
#include <vector>

using namespace std;

class ChunkParser {
	bool is_corrupted = false;
	bool is_incomplete = false;
	unsigned long autocomplete_score = 0;
	stack<char> tokens;

	bool isClosingChar(char c) {
		return (c == '}') || (c == ')') || (c == ']') || (c == '>');
	}

	bool isExpectedClosingChar(char c, char o) {
		return ((o == '{') && (c == '}')) || ((o == '(') && (c == ')')) || ((o == '[') && (c == ']')) || ((o == '<') && (c == '>'));
	}

	void calculateAutocompleteScore() {
		unsigned long score = 0;
		while (!this->tokens.empty()) {
			score *= 5;
			char c = this->tokens.top();	
			this->tokens.pop();	
			switch (c) {
				case '(': score += 1; break;
				case '[': score += 2; break;
				case '{': score += 3; break;
				case '<': score += 4; break;
				default:
					cout << "Found unexpected character *" << c << '*' << endl;
			}
		}
		this->autocomplete_score = score;
	}

	void parseChunk(string &str, char chunk_type) {
		while (!(this->is_corrupted || this->is_incomplete )) {
			if (str.length() == 0) {
				if (!this->tokens.empty()) {
					// INCOMPLETE
					this->is_incomplete = true;
					calculateAutocompleteScore();
				}
				break;
			}

			char c = str[0];
			str.erase(0, 1);
			if (isClosingChar(c)) {
				if (!isExpectedClosingChar(c, chunk_type)) {
					// CORRUPT
					this->is_corrupted = true;
					break;
				} else {
					this->tokens.pop();
					break;
				}
			} else {
				this->tokens.push(c);
				if (str.length() > 0) {
					parseChunk(str, c);
				} else {
					// INCOMPLETE
					this->is_incomplete = true;
					calculateAutocompleteScore();
					break;
				}
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

	bool isIncomplete() {
		return this->is_incomplete;
	}

	unsigned long getAutocompleteScore() {
		return this->autocomplete_score;
	}

};

class NavSys {
	vector<string> input;
	vector<unsigned long> scores;

	void parseLine(string &str) {
	}

	void findIncompleteChunks() {
		for (auto &str: this->input) {
			ChunkParser parser(str);
			if (parser.isIncomplete()) {
				unsigned long score = parser.getAutocompleteScore();
				scores.push_back(score);
			}
		}
	}

	public:
	NavSys(vector<string> &input) {
		this->input = input;
		findIncompleteChunks();
	}

	unsigned long getAnswer() {
		sort(this->scores.begin(), this->scores.end());
		int size = this->scores.size();
		unsigned long answer = this->scores[size / 2];
		return answer;
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day10b <input.txt>" << endl;
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
	unsigned long answer = ns.getAnswer();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}
