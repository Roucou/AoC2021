#include <array>
#include <bitset>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const static regex REGEX_PATTERN = regex("([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+) \\| ([a-g]+) ([a-g]+) ([a-g]+) ([a-g]+)");

class DigitMapper {
	array<size_t, 10> _d2p; // digit to position
	array<int, 10> _p2d; // position to digit
	array<bool, 10> _mapped; // position is mapped?

	public:
	DigitMapper() {
		for (size_t i = 0; i < 10; i++) {
			_d2p[i] = 0;
			_p2d[i] = 0;
			_mapped[i] = false;
		}
	}

	void mapDigitToPosition(int digit, size_t position) {
		_d2p[digit] = position;
		_p2d[position] = digit;
		_mapped[position] = true;
	}

	size_t getPositionOfDigit(int digit) {
		return _d2p[digit];
	}

	int getDigitAtPosition(size_t position) {
		return _p2d[position];
	}

	bool isPositionMapped(size_t position) {
		return _mapped[position];
	}
};

class Sample {
	array<bitset<7>, 10> _input;
	array<bitset<7>, 4> _output;
	DigitMapper _mapper;

	bitset<7> bitsetFromString(string &str) {
		bitset<7> bs;
		for (size_t pos = 0; pos < str.length(); pos++) {
			switch (str[pos]) {
				case 'a': bs.set(0, true); break;
				case 'b': bs.set(1, true); break;
				case 'c': bs.set(2, true); break;
				case 'd': bs.set(3, true); break;
				case 'e': bs.set(4, true); break;
				case 'f': bs.set(5, true); break;
				case 'g': bs.set(6, true); break;
			}
		}
		return bs;
	}

	void parse(string &str) {
		smatch matches;
		bool match = regex_match(str, matches, REGEX_PATTERN);
		if (match) {
			for (size_t i = 1; i < matches.size(); i++) {
				string s = matches[i];
				bitset<7> bs = bitsetFromString(s);
				if (i <= 10) {
					_input[i - 1] = bs;
				} else {
					_output[i - 11] = bs;
				}
			}
		} else {
			cout << "Oops, " << str << " doesn't match regex!" << endl;
		}
	}
	
	void mapEasyDigits() {
		for (size_t i = 0; i < _input.size(); i++) {
			bitset<7> bs = _input[i];
			int bsc = bs.count();
			switch (bsc) {
				case 2: _mapper.mapDigitToPosition(1, i); break;
				case 3: _mapper.mapDigitToPosition(7, i); break;
				case 4: _mapper.mapDigitToPosition(4, i); break;
				case 7: _mapper.mapDigitToPosition(8, i); break;
			}
		}
	}

	void mapDifficultDigits() {
		bitset<7> segments1 = _input[_mapper.getPositionOfDigit(1)];
		bitset<7> segments4 = _input[_mapper.getPositionOfDigit(4)];
		bitset<7> segments7 = _input[_mapper.getPositionOfDigit(7)];
		bitset<7> segments8 = _input[_mapper.getPositionOfDigit(8)];

		// 6 = only one with 6 segments that doesn't have all 7-segments
		for (size_t i = 0; i < _input.size(); i++) {
			if (!_mapper.isPositionMapped(i)) {
				bitset<7> bs = _input[i];
				int bsc = bs.count();
				if ((bsc == 6) && ((bs & segments7) != segments7)) {
					_mapper.mapDigitToPosition(6, i);
					break;
				}
			}
		}
		bitset<7> segments6 = _input[_mapper.getPositionOfDigit(6)];

		// 9 = 6 segments, has all 7-segments, and has all (4-segments - 1-segments)
		bitset<7> center_and_top_left = segments4 ^ segments1;
		for (size_t i = 0; i < _input.size(); i++) {
			if (!_mapper.isPositionMapped(i)) {
				bitset<7> bs = _input[i];
				int bsc = bs.count();
				if ((bsc == 6) &&
					 ((bs & segments7) == segments7) &&
					 ((bs & center_and_top_left) == center_and_top_left)) {
					_mapper.mapDigitToPosition(9, i);
					break;
				}
			}
		}
		bitset<7> segments9 = _input[_mapper.getPositionOfDigit(9)];

		// 0 = last remaing one with 6 segments
		for (size_t i = 0; i < _input.size(); i++) {
			if (!_mapper.isPositionMapped(i)) {
				bitset<7> bs = _input[i];
				int bsc = bs.count();
				if (bsc == 6) {
					_mapper.mapDigitToPosition(0, i);
					break;
				}
			}
		}
		bitset<7> segments0 = _input[_mapper.getPositionOfDigit(0)];

		// 5 = 9-segments - top_right
		bitset<7> top_right = segments8 ^ segments6;
		bitset<7> segments5 = segments9 ^ top_right;
		for (size_t i = 0; i < _input.size(); i++) {
			if (!_mapper.isPositionMapped(i)) {
				bitset<7> bs = _input[i];
				int bsc = bs.count();
				if ((bsc == 5) &&
					(segments5 == bs)) {
					_mapper.mapDigitToPosition(5, i);
					break;
				}
			}
		}
		
		// 3 = 9-segments - top_left;
		bitset<7> center = segments8 ^ segments0;
		bitset<7> top_left = center_and_top_left ^ center;
		bitset<7> segments3 = segments9 ^ top_left;
		for (size_t i = 0; i < _input.size(); i++) {
			if (!_mapper.isPositionMapped(i)) {
				bitset<7> bs = _input[i];
				int bsc = bs.count();
				if ((bsc == 5) &&
					(segments3 == bs)) {
					_mapper.mapDigitToPosition(3, i);
					break;
				}
			}
		}
		
		// 2 = only one that is not yet mapped
		for (size_t i = 0; i < _input.size(); i++) {
			if (!_mapper.isPositionMapped(i)) {
				_mapper.mapDigitToPosition(2, i);
				break;
			}
		}
	}

	size_t findOutputInInput(bitset<7> &output) {
		for (size_t i = 0; i < _input.size(); i++) {
			bitset<7> input = _input[i];
			if (output == input) {
				return i;
			}
		}
		return -1; // should never happen
	}

	public:
	Sample(string &str) {
		parse(str);
		mapEasyDigits();
		mapDifficultDigits();
	}

	unsigned long getDigits() {
		unsigned long res = 0;
		bitset<7> output;
		size_t pos;
		unsigned long digit;

		for (size_t i = 0; i < _output.size(); i++) {
			res *= 10;
			output = _output[i];
			pos = findOutputInInput(output);
			digit = _mapper.getDigitAtPosition(pos);
			res += digit;
		}

		return res;
	}

};

class SegmentSearch {
	vector<Sample> _samples;
	unsigned long _answer = 0;

	void parse(vector<string> &input) {
		for (auto &str: input) {
			Sample sample(str);
			_samples.push_back(sample);
			unsigned long value = sample.getDigits();
			cout << "Digits: " << to_string(value) << endl;
			_answer += value;
		}
	}

	public:
	SegmentSearch(vector<string> &input) {
		parse(input);
	}

	unsigned long getAnswer() {
		return _answer;
	}

};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day8b <input.txt>" << endl;
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
	unsigned long answer = ss.getAnswer();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}

