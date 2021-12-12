#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <regex>
#include <string>
#include <vector>

using namespace std;

const static string START_ID = "start";
const static string END_ID = "end";
const static regex REGEX_PATTERN = regex("([a-zA-Z]+)-([a-zA-Z]+)");

class Cave {
	string id;
	map<string, shared_ptr<Cave>> connections;
	enum {SmallCave, BigCave, StartCave, EndCave} cave_type;

	static bool isAllLowerCase(const string &str) {
		string lower = "";
		for (const char &c: str) {
			lower += tolower(c);
		}
		return str == lower;
	}

	public:
	Cave(const string &id) {
		this->id = id;
		if (id == START_ID) {
			cave_type = StartCave;
		} else if (id == END_ID) {
			cave_type = EndCave;
		} else if (isAllLowerCase(id)) {
			cave_type = SmallCave;
		} else {
			cave_type =  BigCave;
		}
	}

	const string getId() const {
		return this->id;
	}

	void connectTo(shared_ptr<Cave> to) {
		this->connections.insert({to->getId(), to});
	}

	bool connectedTo(string id) const {
		return this->connections.contains(id);
	}

	vector<string> getConnectedCaves() const {
		vector<string> connected;
		for (auto cave_iter: connections) {
			string id = cave_iter.first;
			connected.push_back(id);
		}
		return connected;
	}

	bool isStartCave() const {
		return this->cave_type == StartCave;
	}

	bool isEndCave() const {
		return this->cave_type == EndCave;
	}

	bool isSmallCave() const {
		return this->cave_type == SmallCave;
	}

	bool isBigCave() const {
		return this->cave_type == BigCave;
	}

	void print() const {
		cout << this->id << " ->";
		for (auto cave_it: this->connections) {
			string id = cave_it.first;
			cout << ' ' << id;
		}
		cout << endl;
	}
};

class Path {
	vector<string> path;

	public:
	//Path() {}

	void add(const string &cave_id) {
		this->path.push_back(cave_id);
	}

	bool contains(const string &cave_id) const {
		for (auto &id: this->path) {
			if (id == cave_id) {
				return true;
			}
		}
		return false;
	}

	void print() const {
		for(auto &cave_id: this->path) {
			cout << cave_id << ',';
		}
		cout << endl;
	}
};

using CavePtr = shared_ptr<Cave>;

class CaveExplorer {
	map<string, CavePtr> caves;
	vector<Path> paths;

	void parse(const vector<string> &input) {
		smatch matches;
		string id1;
		string id2;
		CavePtr cave1;
		CavePtr cave2;
		for (const string &str: input) {
			bool match = regex_match(str, matches, REGEX_PATTERN);
			if (match) {
				// First cave
				id1 = matches[1];
				if (this->caves.contains(id1)) {
					// Existing cave
					cave1 = this->caves[id1];
				} else {
					// New cave
					cave1 = make_shared<Cave>(id1);
					this->caves.insert({id1, cave1});
				}

				// Second cave
				id2 = matches[2];
				if (this->caves.contains(id2)) {
					// Existing cave
					cave2 = this->caves[id2];
				} else {
					// New cave
					cave2 = make_shared<Cave>(id2);
					this->caves.insert({id2, cave2});
				}

				// Map cave1 to cave2
				if (!cave1->connectedTo(cave2->getId())) {
					cave1->connectTo(cave2);
				}

				// Map cave2 to cave1
				if (!cave2->connectedTo(cave1->getId())) {
					cave2->connectTo(cave1);
				}
			} else {
				cout << "Oops, " << str << " doesn't match regex!" << endl;
			}
		}
	}

	void walker(const string &cave_id, Path path, const string &visited_twice) {
		path.add(cave_id);

		CavePtr cave = this->caves[cave_id];
		if (cave->isEndCave()) {
			this->paths.push_back(path);
			return;
		}

		vector<string> connected = cave->getConnectedCaves();

		for (auto &conn_id: connected) {
			CavePtr conn_cave = this->caves[conn_id];
			bool is_visited = path.contains(conn_id);

			if (!is_visited || conn_cave->isBigCave()) {
				walker(conn_id, path, visited_twice);
			} else if ((visited_twice == "") &&
						 !(conn_cave->isStartCave() || conn_cave->isEndCave())) {
				walker(conn_id, path, conn_id);
			}
		}
	}

	void findPaths() {
		// Go spelunking!
		Path path;
		walker(START_ID, path, "");
	}

	public:
	CaveExplorer(const vector<string> &input) {
		// Parse input
		parse(input);

		// Find paths
		findPaths();

		// Print paths
		for (auto &path: paths) {
			path.print();
		}

		// Count paths
		unsigned int valid_paths = paths.size();
		cout << "Answer is " << to_string(valid_paths) << endl;
	}

	void print() const {
		cout << "Found " << to_string(this->caves.size()) << " caves." << endl;
		for (auto cave_it: caves) {
			CavePtr cave = cave_it.second;
			cave->print();
		}
		cout << endl;
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day12b <input.txt>" << endl;
		return -1;
	}

	// Open & read file
	string path = argv[1];
	string str;
	ifstream file(path);
	vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	unique_ptr<CaveExplorer> explorer = make_unique<CaveExplorer>(input);

	return 0;
}
