#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Lanternfish {
	int _spawn_counter;

	public:
	Lanternfish(int spawn_counter) {
		_spawn_counter = spawn_counter;
	}

	void nextDay(vector<Lanternfish>& fishes) {
		if (_spawn_counter == 0) {
			// Spawn new lanternfish
			Lanternfish fish(8);
			fishes.push_back(fish);
			_spawn_counter = 6;
		} else {
			_spawn_counter--;
		}
	}

	int getSpawnCounter() {
		return _spawn_counter;
	}
};

class Simulation {
	vector<Lanternfish> _fishes;
	int _day = 1;

	void parse(string& input) {
		stringstream ss(input);
		string token;
		while (getline(ss, token, ','))
		{
			int spawn_counter = stoi(token);
			Lanternfish fish(spawn_counter);
			_fishes.push_back(fish);
		}

	}

	public:
	Simulation(string& input) {
		parse(input);
		cout << "Initial state: ";
		print();
		cout << endl;
	}

	void run(int days) {
		while (_day <= days) {
			vector<Lanternfish> new_fishes;
			for (Lanternfish& fish: _fishes) {
				fish.nextDay(new_fishes);
			}
			for (Lanternfish& fish: new_fishes) {
				_fishes.push_back(fish);
			}
			
			// Print outcome
			cout << "After " << to_string(_day) << " days: ";
			print();
			cout << endl;
			_day++;
		}
	}

	void print() {
		for (Lanternfish& fish: _fishes) {
			cout << to_string(fish.getSpawnCounter()) << ',';
		}
	}

	int countFishes() {
		return _fishes.size();
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day6a <input.txt>" << endl;
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
	sim.run(18);
	//sim.run(80);
	//sim.run(256);
	int answer = sim.countFishes();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}

