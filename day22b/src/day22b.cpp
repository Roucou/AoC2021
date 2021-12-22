#include <cmath>
#include <iostream>
#include <fstream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <unordered_set>
#include <unordered_map>
#include <vector>

using std::cout;
using std::string;
using std::to_string;

const static std::regex REGEX_PATTERN = std::regex("(on|off) x=([-]?[0-9]+)..([-]?[0-9]+),y=([-]?[0-9]+)..([-]?[0-9]+),z=([-]?[0-9]+)..([-]?[0-9]+)");

struct RebootStep {
	bool switch_on;
	int from_x;
	int to_x;
	int from_y;
	int to_y;
	int from_z;
	int to_z;
	int from_x_idx;
	int to_x_idx;
	int from_y_idx;
	int to_y_idx;
	int from_z_idx;
	int to_z_idx;
};

using RebootList = std::vector<RebootStep>;

using Coordinate3d = std::tuple<int, int, int>;

struct Coordinate3dHash {
	size_t operator()(const Coordinate3d &key) const {
		int x = std::get<0>(key);
		int y = std::get<1>(key);
		int z = std::get<2>(key);
		long vector_length = std::sqrt(x*x + y*y + z*z);
		return std::hash<long>()(vector_length);
	}
};

struct Coordinate3dEquals {
	bool operator()(const Coordinate3d &key1, const Coordinate3d &key2) const {
 	   return (std::get<0>(key1) == std::get<0>(key2))
	 	   && (std::get<1>(key1) == std::get<1>(key2))
	 	   && (std::get<2>(key1) == std::get<2>(key2));
	}
};

using Reactor = std::unordered_set<Coordinate3d, Coordinate3dHash, Coordinate3dEquals>;

class Simulation {
	Reactor reactor;
	RebootList reboot_steps;
	std::set<int> coords_x;
	std::set<int> coords_y;
	std::set<int> coords_z;
	std::map<int, int> coords_x2index;
	std::map<int, int> coords_y2index;
	std::map<int, int> coords_z2index;
	std::map<int, int> index2coords_x;
	std::map<int, int> index2coords_y;
	std::map<int, int> index2coords_z;
	unsigned long answer;

	void createLookupTables() {
		int idx;

		idx = 0;
		for (auto &coord: this->coords_x) {
			this->coords_x2index.insert({coord, idx});
			this->index2coords_x.insert({idx, coord});
			idx++;
		}

		idx = 0;
		for (auto &coord: this->coords_y) {
			this->coords_y2index.insert({coord, idx});
			this->index2coords_y.insert({idx, coord});
			idx++;
		}

		idx = 0;
		for (auto &coord: this->coords_z) {
			this->coords_z2index.insert({coord, idx});
			this->index2coords_z.insert({idx, coord});
			idx++;
		}
	}

	void fillIndices() {
		for (auto &step: this->reboot_steps) {
			step.from_x_idx = this->coords_x2index[step.from_x];
			step.to_x_idx = this->coords_x2index[step.to_x];
			step.from_y_idx = this->coords_y2index[step.from_y];
			step.to_y_idx = this->coords_y2index[step.to_y];
			step.from_z_idx = this->coords_z2index[step.from_z];
			step.to_z_idx = this->coords_z2index[step.to_z];
		}
	}

	public:
	void parse(const std::vector<string> &input) {
		bool match;
		std::smatch matches;

		for (auto &str: input) {
			match = std::regex_match(str, matches, REGEX_PATTERN);
			if (match) {
				bool switch_on = (matches[1] == "on") ? true : false;
				int from_x = stoi(matches[2]);
				this->coords_x.insert(from_x);
				int to_x = stoi(matches[3]);
				this->coords_x.insert(to_x);
				int from_y = stoi(matches[4]);
				this->coords_y.insert(from_y);
				int to_y = stoi(matches[5]);
				this->coords_y.insert(to_y);
				int from_z = stoi(matches[6]);
				this->coords_z.insert(from_z);
				int to_z = stoi(matches[7]);
				this->coords_z.insert(to_z);
				RebootStep step = {switch_on, from_x, to_x, from_y, to_y, from_z, to_z};
				this->reboot_steps.push_back(step);
			} else {
				cout << "HUH?!\n";
			}
		}

		// Create lookup tables
		createLookupTables();

		// Fill in indices
		fillIndices();
	}

	void run() {

		for (auto &step: this->reboot_steps) {
			for (int x = step.from_x_idx; x <= step.to_x_idx; x++) {
				for (int y = step.from_y_idx; y <= step.to_y_idx; y++) {
					for (int z = step.from_z_idx; z <= step.to_z_idx; z++) {
						Coordinate3d coord = std::make_tuple(x, y, z);
						if (step.switch_on) {
							this->reactor.insert(coord);
						} else {
							this->reactor.erase(coord);
						}
					}
				}
			}
		}

		unsigned long volume = 0;
		for (auto &coord: this->reactor) {
			int from_x = std::get<0>(coord) - 1;
			int from_y = std::get<1>(coord) - 1;
			int from_z = std::get<2>(coord) - 1;
			int to_x = from_x + 1;
			int to_y = from_y + 1;
			int to_z = from_z + 1;
/*
			cout << '(' << to_string(from_x) << ',' << to_string(from_y) << ',' << to_string(from_z) << ") - "
					<< '(' << to_string(to_x) << ',' << to_string(to_y) << ',' << to_string(to_z) << ")   ->   ";
*/
			int coord_x_from = this->index2coords_x[from_x];
			int coord_x_to = this->index2coords_x[to_x];
			int coord_y_from = this->index2coords_y[from_y];
			int coord_y_to = this->index2coords_y[to_y];
			int coord_z_from = this->index2coords_z[from_z];
			int coord_z_to = this->index2coords_z[to_z];
/*
			cout << '(' << to_string(coord_x_from) << ',' << to_string(coord_y_from) << ',' << to_string(coord_z_from) << ") - "
					<< '(' << to_string(coord_x_to) << ',' << to_string(coord_y_to) << ',' << to_string(coord_z_to) << ")   ->   ";
*/
			int w = coord_x_to - coord_x_from;
			int h = coord_y_to - coord_y_from;
			int d = coord_z_to - coord_z_from;
/*
			cout << "w: " << to_string(w) << ", h: " << to_string(h) << ", d: " << to_string(d)
					<< ", volume = " << to_string(w*h*d) << '\n';
*/
			volume += (w * h * d);
		}
		this->answer = volume;
	}

	const unsigned long getAnswer() const {
		return this->answer;
	}

};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day22b <input.txt>\n";
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
	sim.run();
	unsigned long answer = sim.getAnswer();
	cout << "Answer is " << to_string(answer) << '\n';

	return 0;
}
