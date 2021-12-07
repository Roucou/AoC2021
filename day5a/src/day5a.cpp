#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const static int GRID_MAX_WIDTH = 1000;
const static int GRID_MAX_HEIGHT = 1000;
const static regex REGEX_PATTERN = regex("([0-9]+),([0-9]+) -> ([0-9]+),([0-9]+)");

struct Point {
	int x;
	int y;
};

class Line {
	int _x1 = 0;
	int _y1 = 0;
	int _x2 = 0;
	int _y2 = 0;

	void parseFromString(string str) {
		smatch matches;
		bool match = regex_match(str, matches, REGEX_PATTERN);
		if (match) {
			_x1 = stoi(matches[1]);
			_y1 = stoi(matches[2]);
			_x2 = stoi(matches[3]);
			_y2 = stoi(matches[4]);
		} else {
			cout << "Oops, " << str << " doesn't match regex!" << endl;
		}
	}

	public:
	Line(int x1, int y1, int x2, int y2) {
		_x1 = x1;
		_y1 = y1;
		_x2 = x2;
		_y2 = y2;
	}

	Line(string str) {
		parseFromString(str);
	}

	void print() {
		cout << '(' << to_string(_x1) << ',' << to_string(_y1) << ')';
		cout << " - ";
		cout << '(' << to_string(_x2) << ',' << to_string(_y2) << ')';
		cout << endl;
	}

	bool isHorizontal() {
		return _y1 == _y2;
	}

	bool isVertical() {
		return _x1 == _x2;
	}

	int getX1() { return _x1; }
	int getY1() { return _y1; }
	int getX2() { return _x2; }
	int getY2() { return _y2; }
};

class Grid {
	array<array<int, GRID_MAX_HEIGHT>, GRID_MAX_WIDTH> _measures;
	int _width = GRID_MAX_WIDTH;
	int _height = GRID_MAX_HEIGHT;

	void plotHorizontal(Line &line) {
		cout << "Plot Horizontal" << endl;	
		int y = line.getY1();
		int start = min(line.getX1(), line.getX2());
		int steps = abs(line.getX1() - line.getX2()) + 1;
		int end = start + steps;
		for (size_t x = start; x < end; x++) {
			_measures[y][x] += 1;
		}
	}

	void plotVertical(Line &line) {
		cout << "Plot Vertical" << endl;	
		int x = line.getX1();
		int start = min(line.getY1(), line.getY2());
		int steps = abs(line.getY1() - line.getY2()) + 1;
		int end = start + steps;
		for (size_t y = start; y < end; y++) {
			_measures[y][x] += 1;
		}
	}

	public:
	Grid(const int width, const int height, vector<string> &input) {
		_width = width;
		_height = height;
		zeroAll();
		parse(input);
	}

	void zeroAll() {
		for (size_t y = 0; y < _height; y++) {
			for (size_t x = 0; x < _width; x++) {
				_measures[y][x] = 0;
			}
		}
	}

	void parse(vector<string> &input) {
		for (auto &str: input) {
			Line line(str);
			line.print();
			if (line.isHorizontal() || line.isVertical()) {
				plot(line);
			} else {
				cout << "Ignored!" << endl;	
			}
		}
	}

	void plot(Line &line) {
		if (line.isHorizontal()) {
			plotHorizontal(line);
		} else if (line.isVertical()) {
			plotVertical(line);
		}
	}

	void print() {
		for (size_t y = 0; y < _height; y++) {
			for (size_t x = 0; x < _width; x++) {
				int measure = _measures[y][x];
				if (measure == 0) {
					cout << '.';
				} else if (measure < 10) {
					cout << to_string(measure);
				} else {
					cout << 'X';
				}
			}
			cout << endl;
		}
	}

	int getMeasure(Point p) {
		return _measures[p.y][p.x];
	}

	int getMeasure(int x, int y) {
		return _measures[y][x];
	}

	int countOverlaps() {
		int count = 0;

		for (size_t y = 0; y < _height; y++) {
			for (size_t x = 0; x < _width; x++) {
				if (_measures[y][x] > 1) {
					count++;
				}
			}
		}

		return count;
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day5a <input.txt>" << endl;
		return -1;
	}

	// Open file
	string path = argv[1];
	ifstream file(path);

	// Read file, line per line
	std::string str;
	vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	Grid vents(1000, 1000, input);
	vents.print();
	int overlaps = vents.countOverlaps();
	cout << "Answer: " << to_string(overlaps) << endl;

	return 0;
}

