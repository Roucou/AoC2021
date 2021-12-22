#include <algorithm>
#include <cmath>
#include <iostream>
#include <fstream>
#include <regex>
#include <string>
#include <tuple>
#include <unordered_set>
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

	public:
	void parse(const std::vector<string> &input) {
		bool match;
		std::smatch matches;

		for (auto &str: input) {
			match = std::regex_match(str, matches, REGEX_PATTERN);
			if (match) {
				bool switch_on = (matches[1] == "on") ? true : false;
				int from_x = stoi(matches[2]);
				int to_x = stoi(matches[3]);
				if (from_x > to_x) std::swap(from_x, to_x);
				int from_y = stoi(matches[4]);
				int to_y = stoi(matches[5]);
				if (from_y > to_y) std::swap(from_y, to_y);
				int from_z = stoi(matches[6]);
				int to_z = stoi(matches[7]);
				if (from_z > to_z) std::swap(from_z, to_z);
				RebootStep step = {switch_on, from_x, to_x, from_y, to_y, from_z, to_z};
				this->reboot_steps.push_back(step);
			} else {
				cout << "HUH?!\n";
			}
		}

		for (auto &step: this->reboot_steps) {
			cout << (step.switch_on ? "on" : "off") << ' '
				<< "x=" << to_string(step.from_x) << ".." << to_string(step.to_x) << ','
				<< "y=" << to_string(step.from_y) << ".." << to_string(step.to_y) << ','
				<< "z=" << to_string(step.from_z) << ".." << to_string(step.to_z) << '\n';
		}
	}

	void run() {
		for (auto &step: this->reboot_steps) {
			if ((step.from_x < -50) || (step.from_x > 50)
				|| (step.to_x < -50) || (step.to_x > 50)
				|| (step.from_y < -50) || (step.from_y > 50)
				|| (step.to_y < -50) || (step.to_y > 50)
				|| (step.from_z < -50) || (step.from_z > 50)
				|| (step.to_z < -50) || (step.to_z > 50)) {
					continue;
			}
			for (int x = step.from_x; x <= step.to_x; x++) {
				for (int y = step.from_y; y <= step.to_y; y++) {
					for (int z = step.from_z; z <= step.to_z; z++) {
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
	}

	const unsigned long getAnswer() const {
		return this->reactor.size();
	}

};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day22a <input.txt>\n";
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
