#include <array>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

class Simulation {
	int _day = 0;
	array<unsigned long, 9> _spawn_buckets;

	void parse(string& input) {
		stringstream ss(input);
		string token;
		while (getline(ss, token, ','))
		{
			int spawn_bucket = stoi(token);
			_spawn_buckets[spawn_bucket]++;
		}
	}

	public:
	Simulation(string& input) {
		parse(input);
		print();
	}

	void run(int days) {
		while (_day < days) {
			shift_buckets();
			_day++;
			print();
		}
	}

	void shift_buckets() {
		// Bucket 0
		unsigned long spawn_new = _spawn_buckets[0];

		// Buckets 1 to 8
		for (size_t spawn_bucket = 1; spawn_bucket < 9; spawn_bucket++) {
			_spawn_buckets[spawn_bucket - 1] = _spawn_buckets[spawn_bucket];
		}

		// Handle spawners
		_spawn_buckets[6] += spawn_new;
		_spawn_buckets[8] = spawn_new;
	}

	unsigned long countFishes() {
		unsigned long count = 0;
		for (size_t spawn_bucket = 0; spawn_bucket < 9; spawn_bucket++) {
			count += _spawn_buckets[spawn_bucket];
		}

		return count;
	}

	void print() {
		cout << "Day " << to_string(_day) << ':' << endl;
		cout << "---------" << endl;
		for (size_t spawn_bucket = 0; spawn_bucket < 9; spawn_bucket++) {
			cout << "Bucket " << to_string(spawn_bucket);
			cout << ": " << to_string(_spawn_buckets[spawn_bucket]) << endl;
		}
		cout << endl;
	}
};

int main(int argc, char *argv[]) {
	// First command line argument is the file to open
	if (argc != 2) {
		cout << "Usage: day6b <input.txt>" << endl;
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
	//sim.run(18);
	//sim.run(80);
	sim.run(256);
	unsigned long answer = sim.countFishes();
	cout << "Answer: " << to_string(answer) << endl;

	return 0;
}

