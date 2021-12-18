#include <array>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

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

	public:
	Probe(int velocity_x, int velocity_y) {
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

	void fireAtWill() {
		int max_coolness_y = 0;
		int coolness_x = 0;
		int coolness_y = 0;

		for (int vx = 1; vx < this->target.x2; vx++) {
			int vy = (this->target.y1 * -1) - 1;
//			for (size_t vy = 0; vy < (this->target.y1 * -1); vy++) {
				int max_y = 0;
				Probe p(vx, vy);
				while (!p.overshotTarget(this->target)) {
					p.step();
					max_y = max(max_y, p.getY());
					if (p.withinTarget(this->target)) {
						if (max_y > max_coolness_y) {
							max_coolness_y = max_y;
							coolness_x = vx;
							coolness_y = vy;
						}
					}
//				}
			}
		}

		cout << "Coolest shot was with velocity (";
		cout <<  to_string(coolness_x) << ',' << to_string(coolness_y) << ')' << endl;
		cout << "Probe highest point was " << to_string(max_coolness_y) << endl;
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day17a <input>" << endl;
		return -1;
	}

	// Open & read file
	string input = argv[1];

	// And go!
	ProbeLauncher launcher(input);
	launcher.fireAtWill();

	return 0;
}
