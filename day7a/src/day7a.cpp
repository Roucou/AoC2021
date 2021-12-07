#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const static int MAX_CRAB_COUNT = 1000;

class Simulation {
	array<int, MAX_CRAB_COUNT> _horizontal_positions;
	int _count;
	int _minimum_horizontal_position;
	int _maximum_horizontal_position;

	void parse(string& input) {
		stringstream ss(input);
		string token;
		int count = 0;
		int mini = -1;
		int maxi = -1;
		while (getline(ss, token, ','))
		{
			int horizontal_position = stoi(token);
			_horizontal_positions[count++] = horizontal_position;
			mini = (mini == -1) ? horizontal_position : min(mini, horizontal_position);
			maxi = (maxi == -1) ? horizontal_position : max(maxi, horizontal_position);
		}
		_count = count;
		_minimum_horizontal_position = mini;
		_maximum_horizontal_position = maxi;
	}

	int calculateFuel(int reference_position) {
		int fuel = 0;
		int delta;
		int horizontal_position;

		for (size_t i = 0; i < _count; i++) {
			horizontal_position = _horizontal_positions[i];
			delta = abs(reference_position - horizontal_position);
			fuel += delta;
		}

		return fuel;
	}

	public:
	Simulation(string& input) {
		parse(input);
		print();
	}

	int findMinimumFuel() {
		int minimum_fuel = -1;
		int fuel = 0;

		for (int pos = _minimum_horizontal_position; pos <= _maximum_horizontal_position; pos++) {
			int fuel = calculateFuel(pos);
			minimum_fuel = (minimum_fuel == -1) ? fuel : min(fuel, minimum_fuel);
		}

		return minimum_fuel;
	}

	void print() {
		cout << "Count: " << _count << "   ";
		cout << "Min: " << _minimum_horizontal_position << "   ";
		cout << "Max: " << _maximum_horizontal_position << endl;
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day7a <input.txt>" << endl;
		return -1;
	}

	// Open file
	string path = argv[1];
	ifstream file(path);

	// Read file
	string str;
	getline(file, str);

	// And go!
	Simulation sim(str);
	int answer = sim.findMinimumFuel();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}

