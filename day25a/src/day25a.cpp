#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout;
using std::string;
using std::to_string;

const static size_t MAX_WIDTH = 140;
const static size_t MAX_HEIGHT = 140;

struct Coordinate {
	size_t x;
	size_t y;
};

struct CoordinateHash {
	size_t operator()(const Coordinate &key) const {
		return std::hash<size_t>()(key.x * key.y);
	}
};

struct CoordinateEquals {
	bool operator()(const Coordinate &key1, const Coordinate &key2) const {
 	   return ((key1.x == key2.x) && (key1.y == key2.y));
	}
};

class Simulation {
	unsigned long answer = 0;
	std::array<std::array<char, MAX_WIDTH>, MAX_HEIGHT> map;
	size_t width = 0;
	size_t height = 0;

	unsigned long moveCucumbersEast() {
		std::unordered_set<Coordinate, CoordinateHash, CoordinateEquals> movers;

		for (size_t pos_y = 0; pos_y < this->height; pos_y++) {
			for (size_t pos_x = 0; pos_x < this->width; pos_x++) {
				char ch = this->map[pos_y][pos_x];
				if (ch == '>') {
					// Can move to the east?
					if (   (((pos_x + 1) < this->width) && (this->map[pos_y][pos_x + 1] == '.'))
						|| (((pos_x + 1) == this->width) && (this->map[pos_y][0] == '.')) ) {
						// Yes, can move
						Coordinate coord = {pos_x, pos_y};
						movers.insert(coord);
					}
				}
			}
		}

		for (auto &coord: movers) {
			this->map[coord.y][coord.x] = '.';
			this->map[coord.y][(coord.x + 1) % this->width] = '>';
		}

		return movers.size();
	}

	unsigned long moveCucumbersSouth() {
		std::unordered_set<Coordinate, CoordinateHash, CoordinateEquals> movers;

		for (size_t pos_x = 0; pos_x < this->width; pos_x++) {
			for (size_t pos_y = 0; pos_y < this->height; pos_y++) {
				char ch = this->map[pos_y][pos_x];
				if (ch == 'v') {
					// Can move to the south?
					if (   (((pos_y + 1) < this->height) && (this->map[pos_y + 1][pos_x] == '.'))
						|| (((pos_y + 1) == this->height) && (this->map[0][pos_x] == '.')) ) {
						// Yes, can move
						Coordinate coord = {pos_x, pos_y};
						movers.insert(coord);
					}
				}
			}
		}

		for (auto &coord: movers) {
			this->map[coord.y][coord.x] = '.';
			this->map[(coord.y + 1) % this->height][coord.x] = 'v';
		}

		return movers.size();
	}

	public:
	void parse(const std::vector<string> &input) {
		size_t pos_x;
		size_t pos_y = 0;
		for (auto &str: input) {
			pos_x = 0;
			for (auto &ch: str) {
				this->map[pos_y][pos_x] = ch;
				pos_x++;
			}
			pos_y++;
		}
		this->width = pos_x;
		this->height = pos_y;
		//cout << "Map dimensions: " << to_string(this->width) << 'x' << to_string(this->height) << '\n';
	}

	void init() {
	}

	void run() {
		unsigned long step = 0;
		unsigned long mover_count;

		cout << "Initial state:\n";
		print();

		do {

			mover_count = 0;
			step++;

			mover_count += moveCucumbersEast();
			mover_count += moveCucumbersSouth();

			cout << "(v) After " << to_string(step) << ((step == 1) ? " step:\n" : " steps:\n");
			print();

		} while (mover_count > 0);

		this->answer = step;
	}

	void print() {
		for (size_t pos_y = 0; pos_y < this->height; pos_y++) {
			for (size_t pos_x = 0; pos_x < this->width; pos_x++) {
				cout << this->map[pos_y][pos_x];
			}
			cout << '\n';
		}
		cout << '\n';
	}

	const unsigned long getAnswer() const {
		return this->answer;
	}

};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day25a <input.txt>\n";
		return -1;
	}

	// Open & read file
	string path = argv[1];
	string str;
	std::ifstream file(path);
	std::vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	Simulation sim;
	sim.parse(input);
	sim.init();
	sim.run();
	unsigned long answer = sim.getAnswer();
	cout << "Answer is " << to_string(answer) << '\n';

	return 0;
}
