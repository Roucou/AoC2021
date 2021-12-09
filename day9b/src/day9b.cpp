#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct Point {
	int col;
	int row;
	int height;
};

class Heightmap {
	array<array<int, 100>, 100> heightmap;
	array<array<bool, 100>, 100> mapped;
	int map_width;
	int map_height;
	vector<Point> low_points;
	vector<int> basins;

	void parse(vector<string> &input) {
		int row = 0;
		for (auto &str: input) {
			this->map_width = str.length();
			for (size_t i = 0; i < str.length(); i++) {
				char c = str[i];
				int height = c - '0';
				this->heightmap[row][i] = height;
			}
			row++;
		}
		this->map_height = row;
		cout << "Map dimensions: " << to_string(this->map_width) << "x" << to_string(this->map_height) << endl;
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
					Point p;
					p.col = col;
					p.row = row;
					p.height = this->heightmap[row][col];
					low_points.push_back(p);
				}
			}
		}
	}

	vector<Point> findNeighbours(Point &p) {
		vector<Point> neighbours;
		int col = p.col;
		int row = p.row;
		int height = p.height;

		// Top
		if ((row > 0) &&
			!this->mapped[row - 1][col] &&
			(this->heightmap[row - 1][col] >= height) &&
			(this->heightmap[row - 1][col] < 9)) {
			Point pp;
			pp.col = col;
			pp.row = row - 1;
			pp.height = this->heightmap[row - 1][col];
			this->mapped[row - 1][col] = true;
			neighbours.push_back(pp);
		}

		// Left
		if ((col > 0) &&
			!this->mapped[row][col - 1] &&
			(this->heightmap[row][col - 1] >= height) &&
			(this->heightmap[row][col - 1] < 9)) {
			Point pp;
			pp.col = col - 1;
			pp.row = row;
			pp.height = this->heightmap[row][col - 1];
			this->mapped[row][col - 1] = true;
			neighbours.push_back(pp);
		}

		// Right
		if (((col + 1) < this->map_width) &&
			!this->mapped[row][col + 1] &&
			(this->heightmap[row][col + 1] >= height) &&
			(this->heightmap[row][col + 1] < 9)) {
			Point pp;
			pp.col = col + 1;
			pp.row = row;
			pp.height = this->heightmap[row][col + 1];
			this->mapped[row][col + 1] = true;
			neighbours.push_back(pp);
		}

		// Bottom
		if (((row + 1) < this->map_height) &&
			!this->mapped[row + 1][col] &&
			(this->heightmap[row + 1][col] >= height) &&
			(this->heightmap[row + 1][col] < 9)) {
			Point pp;
			pp.col = col;
			pp.row = row + 1;
			pp.height = this->heightmap[row + 1][col];
			this->mapped[row + 1][col] = true;
			neighbours.push_back(pp);
		}

		return neighbours;
	}

	vector<Point> findNeighbours(vector<Point> &neighbours) {
		vector<Point> new_neighbours;
		for (auto &p:  neighbours) {
			vector<Point> found_neighbours = findNeighbours(p);
			for (auto &pp: found_neighbours) {
				new_neighbours.push_back(pp);
			}
		}
		return new_neighbours;
	}

	void findBasins() {
		for (size_t row = 0; row < this->map_height; row++) {
			for (size_t col = 0; col < this->map_width; col++) {
				this->mapped[row][col] = false;
			}
		}
		for (auto &p: this->low_points) {
			cout << "Find neighbours for point " << to_string(p.col) << ',' << to_string(p.row) << endl;
			vector<Point> neighbours;
			neighbours.push_back(p);
			vector<Point> new_neighbours = findNeighbours(p);
			while (!new_neighbours.empty()) {
				for (auto &p: new_neighbours) {
					neighbours.push_back(p);
				}
				new_neighbours = findNeighbours(new_neighbours);
			}
			this->basins.push_back(neighbours.size());
			cout << "Basis with size " << to_string(neighbours.size()) << endl;
		}
	}

	public:
	Heightmap(vector<string> &input) {
		parse(input);
		findLowPoints();
		findBasins();
	}

	int getAnswer() {
		int answer;
		sort(this->basins.begin(), this->basins.end(), greater<int>());
		for (auto size: basins) {
			cout << to_string(size) << ' ';
		}
		cout << endl;
		answer = basins[0] * basins[1] * basins[2];
		return answer;
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day9b <input.txt>" << endl;
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
