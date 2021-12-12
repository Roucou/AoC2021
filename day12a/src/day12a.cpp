#include <algorithm>
#include <array>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>
#include <regex>
#include <stack>
#include <string>
#include <unordered_set>
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

	bool isStartCave() {
		return this->cave_type == StartCave;
	}

	bool isEndCave() {
		return this->cave_type == EndCave;
	}

	bool isSmallCave() {
		return this->cave_type == SmallCave;
	}

	bool isBigCave() {
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
	Path() {}

	Path(const string &cave_id) {
		this->path.push_back(cave_id);
	}

	vector<string> getPath() const {
		vector<string> copied_path;
		for (auto &id: this->path) {
			copied_path.push_back(id);
		}
		return copied_path;
	}

	void add(const string &cave_id) {
		this->path.push_back(cave_id);
	}

	const string getLast() const {
		return this->path.back();
	}

	bool isClosed() const {
		string cave_id = this->path.back();
		return cave_id == END_ID;
	}

	bool contains(const string &cave_id) const {
		for (auto &id: this->path) {
			if (id == cave_id) {
				return true;
			}
		}
		return false;
	}
};

using CavePtr = shared_ptr<Cave>;
using Directory = map<string, CavePtr>;
using PathPtr = shared_ptr<Path>;

class CaveExplorer {
	Directory caves;
	CavePtr start_cave;
	CavePtr end_cave;

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
		this->start_cave = caves["start"];
		this->end_cave = caves["end"];
	}

	bool visitedCave(const string &id, const vector<string> &visited) {
		for (const string &str: visited) {
			if (str == id) {
				return true;
			}
		}
		return false;
	}

	vector<unsigned int> findOpenPaths(const map<unsigned int, PathPtr> &paths) const {
		vector<unsigned int> open_paths;
		for (auto path_it: paths) {
			PathPtr path = path_it.second;
			if (!path->isClosed()) {
				unsigned int id = path_it.first;
				open_paths.push_back(id);
			}
		}
		return open_paths;
	}

	bool allPathsClosed(const map<unsigned int, PathPtr> &paths) const {
		vector<unsigned int> open_paths = findOpenPaths(paths);
		return open_paths.empty();
	}

	void findPaths() {
		map<unsigned int, PathPtr> paths;
		unsigned int unique_path_id = 0;

		// Start in start_cave
		CavePtr current_cave = this->start_cave;
		string current_id = current_cave->getId();
		PathPtr path = make_shared<Path>(current_id);
		paths.insert({++unique_path_id, path});

		// Go spelunking!
		while (!allPathsClosed(paths)) {
			cout << "--- NEW ITERATION ---" << endl;
			// For every open path, grab the last cave, find its connections,
			// and create new paths for every connection.
			// Finally, remove the original path.
			const vector<unsigned int> open_paths = findOpenPaths(paths);
			for (auto &path_id: open_paths) {
				cout << "Found open path with id " << to_string(path_id) << endl;
				path = paths[path_id];
				current_id = path->getLast();
				current_cave = this->caves[current_id];
				vector<string> connected = current_cave->getConnectedCaves();
				for (auto &cave_id: connected) {
					CavePtr cave = this->caves[cave_id];

					// Don't visit start
					if (cave->isStartCave()) {
						continue;
					}

					// Don't visit small caves more than once
					if (cave->isSmallCave() && path->contains(cave_id)) {
						continue;
					}
	
					// Create new path based on current path
					//PathPtr new_path = make_shared<Path>(path);
					PathPtr new_path = make_shared<Path>();
					vector<string> old_path = path->getPath();
					for (auto &old_cave_id: old_path) {
						new_path->add(old_cave_id);
					}
					new_path->add(cave_id);
					paths.insert({++unique_path_id, new_path});
					cout << "Created new path with id " << to_string(unique_path_id) << endl;
				}
			}

			// Remove open paths from map
			for (auto &path_id: open_paths) {
				cout << "Deleting path with id " << to_string(path_id) << endl;
				paths.erase(path_id);
			}
			
		}

		// Count paths
		unsigned int valid_paths = paths.size();
		cout << "Answer is " << to_string(valid_paths) << endl;
	}

	public:
	CaveExplorer(const vector<string> &input) {
		parse(input);
		print();
		findPaths();
	}

	void print() {
		cout << "Found " << to_string(this->caves.size()) << " caves." << endl;
		cout << "Start cave's id: " << this->start_cave->getId() << endl;
		cout << "End cave's id: " << this->end_cave->getId() << endl;
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
		cout << "Usage: day12a <input.txt>" << endl;
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
