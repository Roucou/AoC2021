#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Heightmap {
	array<array<int, 100>, 100> heightmap;
	int map_width;
	int map_height;
	vector<int> low_points;
	int risk_level = 0;

	void parse(vector<string> &input) {
		int row = 0;
		for (auto &str: input) {
			map_width = str.length();
			for (size_t i = 0; i < str.length(); i++) {
				char c = str[i];
				int height = c - '0';
				this->heightmap[row][i] = height;
			}
			row++;
		}
		map_height = row;
		cout << "Map dimensions: " << to_string(map_width) << "x" << to_string(map_height) << endl;
	}

	bool isLowPoint(int col, int row) {
		int top = (row > 0) ? this->heightmap[row - 1][col] : 10;
		int left = (col > 0) ? this->heightmap[row][col - 1] : 10;
		int center = this->heightmap[row][col];
		int right = ((col + 1) < this->map_width) ? this->heightmap[row][col + 1] : 10;
		int bottom = ((row + 1) < this->map_height) ? this->heightmap[row + 1][col] : 10;
		
		bool isLowest = ((center < top) && (center < left) && (center < right) && (center < bottom));
		
		return isLowest;
	}

	void findLowPoints() {
		for (size_t row = 0; row < this->map_height; row++) {
			for (size_t col = 0; col < this->map_width; col++) {
				if (isLowPoint(col, row)) {
					low_points.push_back(this->heightmap[row][col]);
				}
			}
		}
	}

	public:
	Heightmap(vector<string> &input) {
		parse(input);
		findLowPoints();
	}

	int getAnswer() {
		int sum = 0;
		for (auto height: this->low_points) {
			sum += height + 1;
		}
		return sum;
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day9a <input.txt>" << endl;
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
	Heightmap hm(input);
	int answer = hm.getAnswer();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}

