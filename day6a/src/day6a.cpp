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
			vector<Lanternfish> new_fishes; // See "Note 1"
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
	// Second command line argument is the number of days to simulate
	if (argc != 3) {
		cout << "Usage: day6a <input.txt> <days>" << endl;
		return -1;
	}
	string path = argv[1];
	int days = stoi(argv[2]);

	// Open file
	string str;
	ifstream file(path);
	getline(file, str);

	// And go!
	Simulation sim(str);
	sim.run(days);
	int answer = sim.countFishes();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}

/*
Note 1
------
Don't add elements to a vector while iterating over it!

A vector manages its own storage. If its internal storage is exhausted,
the vector will do an internal restructure. During this restructure the
memory addresses of existing elements can (read: will) change. This will
mess up the current iterator.

So either keep new elements in a temporary vector and add the contents
of that vector to the original vector after you have finished iterating.

Or, if you know the maximum required capacity beforehand, reserve this
capacity using this method: v.reserve(n), where n is the number of elements
to reserve. But then you probably are using the wrong type of container. ;-)
*/
