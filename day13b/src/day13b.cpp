#include <array>
#include <bitset>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

using namespace std;

const static int MAX_WIDTH = 1311;
const static int MAX_HEIGHT = 894;
const static regex REGEX_PATTERN1 = regex("([0-9]+),([0-9]+)");
const static regex REGEX_PATTERN2 = regex("fold along (x|y)=([0-9]+)");

struct Coordinate {
	int x;
	int y;
};

struct Instruction {
	string axis;
	int position;
};

class Paper {
	array<array<bool, MAX_WIDTH>, MAX_HEIGHT> grid;
	int width = -1;
	int height = -1;

	public:
	Paper(int width, int height, const vector<Coordinate> &coordinates) {
		this->width = width;
		this->height = height;
		clearPaper();
		plotDots(coordinates);
	}

	void clearPaper() {
		for (size_t y = 0; y < this->height; y++) {
			for (size_t x = 0; x < this->width; x++) {
				this->grid[y][x] = false;
			}
		}
	}

	void plotDots(const vector<Coordinate> &coordinates) {
		for (auto &coord: coordinates) {
			this->grid[coord.y][coord.x] = true;
		}
	}

	void foldHorizontal(int position) {
		int rows_to_fold = this->height - 1 - position;
		for (size_t row = 0; row < rows_to_fold; row++) {
			for (size_t x = 0; x < this->width; x++) {
				int dst_y = position - rows_to_fold + row;
				int src_y = this->height - 1 - row;
				this->grid[dst_y][x] = this->grid[dst_y][x] || this->grid[src_y][x];
			}
		}
		this->height = position;
	}

	void foldVertical(int position) {
		int cols_to_fold = this->width - 1 - position;
		for (size_t col = 0; col < cols_to_fold; col++) {
			for (size_t y = 0; y < this->height; y++) {
				int dst_x = position - cols_to_fold + col;
				int src_x = this->width - 1 - col;
				this->grid[y][dst_x] = this->grid[y][dst_x] || this->grid[y][src_x];
			}
		}
		this->width = position;
	}

	void print() const {
		for (size_t y = 0; y < this->height; y++) {
			for (size_t x = 0; x < this->width; x++) {
				cout << (this->grid[y][x] ? '#' : '.');
			}
			cout << endl;
		}
	}
};

class Origami {
	unique_ptr<Paper> paper;
	vector<Coordinate> coordinates;
	vector<Instruction> instructions;
	int width = -1;
	int height = -1;

	void parse(const vector<string> &input) {
		int max_width = -1;
		int max_height = -1;
		smatch matches;
		bool match;
		for (const string &str: input) {
			if (str.empty()) {
				continue;
			}
			match = regex_match(str, matches, REGEX_PATTERN1);
			if (match) {
				// Found coordinate
				Coordinate coord;
				coord.x = stoi(matches[1]);
				coord.y = stoi(matches[2]);
				this->coordinates.push_back(coord);
				cout << "Found coordinate (" << to_string(coord.x);
				cout << ',' << to_string(coord.y) << ')' << endl;
				max_width = max(max_width, coord.x);
				max_height = max(max_height, coord.y);
			} else {
				match = regex_match(str, matches, REGEX_PATTERN2);
				if (match) {
					// Found instruction
					Instruction instruction;
					instruction.axis = matches[1];
					instruction.position = stoi(matches[2]);
					this->instructions.push_back(instruction);
					cout << "Found fold instruction along " << instruction.axis << "-axis, ";
					cout  << "at position " << to_string(instruction.position) << endl;
				} else {
					cout << "Oops, " << str << " doesn't match regex!" << endl;
				}
			}
		}
		this->width = max_width + 1;
		this->height = max_height + 1;
	}

	void process(const Instruction &instruction) {
		cout << "Processing fold instruction along " << instruction.axis << "-axis, ";
		cout  << "at position " << to_string(instruction.position) << endl;
		if (instruction.axis == "y") {
			this->paper->foldHorizontal(instruction.position);
		} else {
			this->paper->foldVertical(instruction.position);
		}
	}

	public:
	Origami(const vector<string> &input) {
		parse(input);
		this->paper = make_unique<Paper>(this->width, this->height, this->coordinates);
		cout << "Grid size is " << to_string(this->width) << 'x' << to_string(this->height) << endl;
		this->paper->print();
	}

	void fold() {
		for (auto &instruction: this->instructions) {
			process(instruction);
			this->paper->print();
		}
	}

	void print() const {
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day13b <input.txt>" << endl;
		return -1;
	}

	// Open & read file
	string path = argv[1];
	string str;
	ifstream file(path);
	vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	unique_ptr<Origami> origami = make_unique<Origami>(input);
	origami->fold();

	return 0;
}
