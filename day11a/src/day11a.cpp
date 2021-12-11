#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <memory>
#include <stack>
#include <string>
#include <unordered_set>
#include <vector>

using namespace std;

static long id_gen = 0;

class Octopus {
	long id = ++id_gen;
	size_t col;
	size_t row;
	unsigned int energy;

	public:
	Octopus() {
		this->col = -1;
		this->row = -1;
		this->energy = 0;
	}

	Octopus(size_t col, size_t row, unsigned int energy) {
		this->col = col;
		this->row = row;
		this->energy = energy;
	}

	size_t getColumn() const {
		return this->col;
	}

	size_t getRow() const {
		return this->row;
	}

	void increaseEnergy() {
		this->energy++;
	}

	unsigned int getEnergy() {
		return this->energy;
	}

	void resetEnergy() {
		this->energy = 0;
	}

	bool isOverloaded() const {
		return this->energy > 9;
	}

/*
	static bool compare(const Octopus &o1, const Octopus &o2) {
		return ((o1.col == o2.col) && (o1.row == o2.row));
	}

	bool operator ==(const Octopus &other) const {
		return ((this->col == other.col) && (this->row == other.row));
	}
*/

	long getId() const {
		return this->id;
	}

	void print() const {
		cout << "Octopus " << to_string(this->id);
		cout << " at " << to_string(this->col) << ',' << to_string(this->row);
		cout << " has energy level " << to_string(this->energy);
		cout << (this->isOverloaded() ? " (OVERLOADED!)" : "") << endl;
	}
};

/*
namespace std {
	template<> struct hash<Octopus> {
		size_t operator()(const Octopus &octopus) const {
			return hash<long>()(octopus.getId());
		}
	};
}
// See: https://thispointer.com/how-to-use-unordered_set-with-user-defined-classes-tutorial-example/
*/

using OctopusPtr = shared_ptr<Octopus>;
using Aquarium = unordered_set<OctopusPtr>;

class Simulation {
	array<array<OctopusPtr, 10>, 10> octopuses;
	unsigned long flashes = 0;

	void parse(const vector<string> &input) {
		size_t row = 0;
		for (const string &str: input) {
			for (size_t col = 0; col < 10; col++) {
				unsigned int energy = str[col] - '0';
				OctopusPtr octopus = make_shared<Octopus>(col, row, energy);
				this->octopuses[row][col] = octopus;
			}
			row++;
		}
	}

	void increaseEnergyLevels() {
		for (size_t row = 0; row < 10; row++) {
			for (size_t col = 0; col < 10; col++) {
				OctopusPtr octopus = this->octopuses[row][col];
				octopus->increaseEnergy();
			}
		}
	}

	void flashOctopus(OctopusPtr octopus) {
		// Increase energy level of neighbours
		size_t col = octopus->getColumn();
		size_t row = octopus->getRow();
		OctopusPtr neighbour;
		
		// Top-left
		if ((row > 0) && (col > 0)) {
			neighbour = this->octopuses[row - 1][col - 1];
			neighbour->increaseEnergy();
		}

		// Top
		if (row > 0) {
			neighbour = this->octopuses[row - 1][col];
			neighbour->increaseEnergy();
		}

		// Top-right
		if ((row > 0) && ((col + 1) < 10)) {
			neighbour = this->octopuses[row - 1][col + 1];
			neighbour->increaseEnergy();
		}

		// Left
		if (col > 0) {
			neighbour = this->octopuses[row][col - 1];
			neighbour->increaseEnergy();
		}

		// Right
		if ((col + 1) < 10) {
			neighbour = this->octopuses[row][col + 1];
			neighbour->increaseEnergy();
		}

		// Bottom-left
		if (((row + 1) < 10) && (col > 0)) {
			neighbour = this->octopuses[row + 1][col - 1];
			neighbour->increaseEnergy();
		}

		// Bottom
		if ((row + 1) < 10) {
			neighbour = this->octopuses[row + 1][col];
			neighbour->increaseEnergy();
		}

		// Bottom-right
		if (((row + 1) < 10) && ((col + 1) < 10)) {
			neighbour = this->octopuses[row + 1][col + 1];
			neighbour->increaseEnergy();
		}
	}

	unsigned long flashOctopuses() {
		Aquarium flashed;
		Aquarium overloaded;
		do {
			// Find overloaded octopuses
			overloaded.clear();
			for (size_t row = 0; row < 10; row++) {
				for (size_t col = 0; col < 10; col++) {
					OctopusPtr octopus = this->octopuses[row][col];
					if (octopus->isOverloaded() && !flashed.contains(octopus)) {
						overloaded.insert(octopus);
					}
				}
			}

			// Flash and add to flashed
			for (auto octopus: overloaded) {
				flashOctopus(octopus);
				flashed.insert(octopus);
			}
		} while (!overloaded.empty());

		// Reset energy levels of flashed octopuses
		for (auto octopus: flashed) {
			octopus->resetEnergy();
		}

		return flashed.size();
	}

	public:
	Simulation(const vector<string> &input) {
		parse(input);
	}

	void simulate(const unsigned int steps) {
		print();
		for (unsigned int step = 1; step <= steps; step++) {
			increaseEnergyLevels();
			this->flashes += flashOctopuses();
			cout << "Total flashes after step " << to_string(step);
			cout << " is " << to_string(this->flashes) << endl;
			print();
		}
	}

	unsigned long getAnswer() const {
		return this->flashes;
	}

	void print() const {
		for (size_t row = 0; row < 10; row++) {
			for (size_t col = 0; col < 10; col++) {
				OctopusPtr octopus = this->octopuses[row][col];
				unsigned int energy = octopus->getEnergy();
				cout << to_string(energy) << ' ';
			}
			cout << endl;
		}
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 3) {
		cout << "Usage: day11a <input.txt> <steps>" << endl;
		return -1;
	}
	string path = argv[1];
	unsigned int steps = atoi(argv[2]);

	// Open file
	string str;
	ifstream file(path);
	vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	unique_ptr<Simulation> sim = make_unique<Simulation>(input);
	sim->simulate(steps);
	unsigned long answer = sim->getAnswer();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}
