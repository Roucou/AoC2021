#include <bitset>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout;
using std::string;
using std::to_string;
using std::vector;

struct Coordinate {
	int col;
	int row;
};

struct CoordinateEquals {
	bool operator()(const Coordinate &coord1, const Coordinate &coord2) const {
 	   return (coord1.col == coord2.col) && (coord1.row == coord2.row);
	}
};

struct CoordinateHash {
	size_t operator()(const Coordinate &coord) const {
		long hash_me = coord.col * coord.row;
		return std::hash<long>()(hash_me);
	}
};

class ImageEnhancementAlgorithm {
	std::bitset<512> lookup_table;

	public:
	void parse(const string &str) {
		size_t idx = 0;
		for (auto &c: str) {
			this->lookup_table[idx++] = (c == '#') ? true : false;
		}
	}

	const bool operator[](const size_t idx) const {
		return this->lookup_table[idx];
	}

};

using InfiniGrid = std::unordered_set<Coordinate, CoordinateHash, CoordinateEquals>;

class Image {
	InfiniGrid grid;
	bool canvas_color = false;
	Coordinate image_top_left = {0, 0};
	Coordinate image_bottom_right = {0, 0};

	public:

	void parse(const vector<string> &input) {
		int col;
		int row = 0;

		for (auto &str: input) {
			col = 0;
			for (auto &c: str) {
				if (c == '#') {
					setOn(col, row);
				}
				col++;
			}
			row++;
		}
	}

	void setOn(const int col, const int row) {
		const Coordinate coord = {col, row};
		this->grid.insert(coord);

		// Grow image?
		if (col < this->image_top_left.col) {
			if (this->canvas_color) {
				for (int rr = this->image_top_left.row; rr <= this->image_bottom_right.row; rr++) {
					for (int cc = (this->image_top_left.col - 1); cc >= col; cc--) {
						Coordinate border_coord = {cc, rr};
						this->grid.insert(border_coord);
					}
				}
			}
			const Coordinate corner = {col, this->image_top_left.row};
			this->image_top_left = corner;
		} else if (col > this->image_bottom_right.col) {
			if (this->canvas_color) {
				for (int rr = this->image_top_left.row; rr <= this->image_bottom_right.row; rr++) {
					for (int cc = (this->image_bottom_right.col + 1); cc <= col; cc++) {
						Coordinate border_coord = {cc, rr};
						this->grid.insert(border_coord);
					}
				}
			}
			const Coordinate corner = {col, this->image_bottom_right.row};
			this->image_bottom_right = corner;
		}

		if (row < this->image_top_left.row) {
			if (this->canvas_color) {
				for (int rr = (this->image_top_left.row - 1); rr >= row; rr--) {
					for (int cc = this->image_top_left.col; cc <= this->image_bottom_right.col; cc++) {
						Coordinate border_coord = {cc, rr};
						this->grid.insert(border_coord);
					}
				}
			}
			const Coordinate corner = {this->image_top_left.col, row};
			this->image_top_left = corner;
		} else if (row > this->image_bottom_right.row) {
			if (this->canvas_color) {
				for (int rr = (this->image_bottom_right.row + 1); rr <= row; rr++) {
					for (int cc = this->image_top_left.col; cc <= this->image_bottom_right.col; cc++) {
						Coordinate border_coord = {cc, rr};
						this->grid.insert(border_coord);
					}
				}
			}
			const Coordinate corner = {this->image_bottom_right.col, row};
			this->image_bottom_right = corner;
		}
	}

	bool isOn(const int col, const int row) const {
		if ((col < this->image_top_left.col) || 
			(col > this->image_bottom_right.col) ||
			(row < this->image_top_left.row) ||
			(row > this->image_bottom_right.row)) {
			return this->canvas_color;
		}
		const Coordinate coord = {col, row};
		return this->grid.contains(coord);
	}

	const Coordinate getImageTopLeft() const {
		return this->image_top_left;
	}

	const Coordinate getImageBottomRight() const {
		return this->image_bottom_right;
	}

	bool getCanvasColor() const {
		return this->canvas_color;
	}

	void setCanvasColor(const bool color) {
		this->canvas_color = color;
	}

	unsigned long litPixels() const {
		return this->grid.size();
	}

};

class ImageEnhancer {
	ImageEnhancementAlgorithm algorithm;
	Image image;

	public:
	void parse(const vector<string> &input) {
		size_t idx = 0;
		string str;
		vector<string> image;

		while (idx < input.size()) {
			str = input.at(idx++);

			// First string is image enhancement algorithm
			if (idx == 1) {
				this->algorithm.parse(str);
				continue;
			}

			// Second string should be empty
			if (idx == 2) {
				continue;
			}

			// Remaining strings are starting image
			image.push_back(str);
		}

		// Finally, parse image
		this->image.parse(image);

		print();
	}

	void enhance() {
		Image new_image;
		std::bitset<9> bits;
		Coordinate top_left = this->image.getImageTopLeft();
		Coordinate bottom_right = this->image.getImageBottomRight();
		size_t idx;

		for (int row = (top_left.row - 1); row <= (bottom_right.row + 1); row++) {
			for (int col = (top_left.col - 1); col <= (bottom_right.col + 1); col++) {
				bits[8] = this->image.isOn(col - 1, row - 1); // top-left
				bits[7] = this->image.isOn(col    , row - 1); // top-center
				bits[6] = this->image.isOn(col + 1, row - 1); // top-right
				bits[5] = this->image.isOn(col - 1, row    ); // center-left
				bits[4] = this->image.isOn(col    , row    ); // center
				bits[3] = this->image.isOn(col + 1, row    ); // center-right
				bits[2] = this->image.isOn(col - 1, row + 1); // bottom-left
				bits[1] = this->image.isOn(col    , row + 1); // bottom-center
				bits[0] = this->image.isOn(col + 1, row + 1); // bottom-right
				idx = bits.to_ulong();
				if (this->algorithm[idx]) {
					new_image.setOn(col, row);
				}
			}
		}

		// Update canvas color
		if (this->image.getCanvasColor()) {
			new_image.setCanvasColor(this->algorithm[511]); // 111111111 = 511
		} else {
			new_image.setCanvasColor(this->algorithm[0]);   // 000000000 = 0
		}

		this->image = new_image;

		print();
	}

	void print() const {
		Coordinate top_left = this->image.getImageTopLeft();
		Coordinate bottom_right = this->image.getImageBottomRight();

		for (int row = top_left.row; row <= bottom_right.row; row++) {
			for (int col = top_left.col; col <= bottom_right.col; col++) {
				cout << (this->image.isOn(col, row) ? '#' : '.');
			}
			cout << '\n';
		}
		cout << '\n';
	}

	unsigned long getAnswer() const {
		return this->image.litPixels();
	}

};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day20b <input.txt>\n";
		return -1;
	}

	// Open & read file
	string path = argv[1];
	string str;
	std::ifstream file(path);
	vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	ImageEnhancer ie;
	ie.parse(input);
	for (size_t i = 1; i <= 50; i++)
		ie.enhance();
	unsigned long answer = ie.getAnswer();
	cout << "Answer is " << to_string(answer) << '\n';

	return 0;
}
