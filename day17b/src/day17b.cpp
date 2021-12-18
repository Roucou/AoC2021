#include <array>
#include <iostream>
#include <regex>
#include <string>
#include <utility>
#include <unordered_set>

using namespace std;

const static regex REGEX_PATTERN = regex("target area: x=([+-]?[0-9]+)..([+-]?[0-9]+), y=([+-]?[0-9]+)..([+-]?[0-9]+)");

struct Rect {
	int x1;
	int y1;
	int x2;
	int y2;
};

class Probe {
	int x = 0;
	int y = 0;
	int velocity_x = 0;
	int velocity_y = 0;
	int initial_velocity_x = 0;
	int initial_velocity_y = 0;

	public:
	Probe(int velocity_x, int velocity_y) {
		this->initial_velocity_x = velocity_x;
		this->initial_velocity_y = velocity_y;
		this->velocity_x = velocity_x;
		this->velocity_y = velocity_y;
	}

	void step() {
		this->x += this->velocity_x;
		this->y += this->velocity_y;
		if (this->velocity_x > 0) {
			this->velocity_x -= 1;
		} else if (this->velocity_x < 0) {
			this->velocity_x += 1;
		}
		this->velocity_y -= 1;
	}

	int getX() const {
		return this->x;
	}

	int getY() const {
		return this->y;
	}

	int getInitialVelocityX() const {
		return this->initial_velocity_x;
	}

	int getInitialVelocityY() const {
		return this->initial_velocity_y;
	}

	int getVelocityX() const {
		return this->velocity_x;
	}

	int getVelocityY() const {
		return this->velocity_y;
	}

	bool withinTarget(const Rect &target) {
		return !((this->x < target.x1)
					|| (this->x > target.x2)
					|| (this->y > target.y2)
					|| (this->y < target.y1));
	}

	bool overshotTarget(const Rect &target) {
		return ((this->x > target.x2) || (this->y < target.y1));
	}
};

// https://thispointer.com/using-unordered_set-with-custom-hasher-and-comparision-function/

struct VelocityComparator {
    bool operator()(const Probe &p1, const Probe &p2) const {
		return ((p1.getInitialVelocityX() == p2.getInitialVelocityX())
				&& (p1.getInitialVelocityY() == p2.getInitialVelocityY()));
    }
};

struct VelocityHash {
	size_t operator()(const Probe &p) const {
		return hash<int>()(p.getInitialVelocityX());
	}
};

class ProbeLauncher {
	Rect target;

	void parse(const string &input) {
		smatch matches;
		bool match = regex_match(input, matches, REGEX_PATTERN);
		if (match) {
			int x1 = stoi(matches[1]);
			int x2 = stoi(matches[2]);
			int y1 = stoi(matches[3]);
			int y2 = stoi(matches[4]);
			this->target = {x1, y1, x2, y2};
		} else {
			cout << "Huh?!" << endl;
		}
	}

	public:
	ProbeLauncher(const string &input) {
		parse(input);
	}

	void fireAtTarget() {
		unordered_set<Probe, VelocityHash, VelocityComparator> hits;

		for (int vx = 1; vx <= this->target.x2; vx++) {
			for (int vy = this->target.y1; vy <= (this->target.y1 * -1); vy++) {
				Probe p(vx, vy);
				while (!p.overshotTarget(this->target)) {
					p.step();
					if (p.withinTarget(this->target)) {
						// Hit!
						hits.insert(p);
						break;
					}
				}
			}
		}
		cout << "Hits: " << to_string(hits.size()) << endl;
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day17b <input>" << endl;
		return -1;
	}

	// Open & read file
	string input = argv[1];

	// And go!
	ProbeLauncher launcher(input);
	launcher.fireAtTarget();

	return 0;
}
