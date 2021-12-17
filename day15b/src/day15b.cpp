#include <array>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

static const unsigned int MAX_WIDTH = 500;
static const unsigned int MAX_HEIGHT = 500;

struct Position {
	size_t col;
	size_t row;
};

class Simulation {
	array<array<unsigned int, MAX_WIDTH>, MAX_HEIGHT> weights;
	array<array<unsigned int, MAX_WIDTH>, MAX_HEIGHT> distance;
	array<array<bool, MAX_WIDTH>, MAX_HEIGHT> visited;
	size_t width;
	size_t height;

	void duplicate() {
		size_t dst_row;
		size_t dst_col;
		unsigned int weight;

		for (size_t cpy_row = 0; cpy_row < 5; cpy_row++) {
			for (size_t cpy_col = 0; cpy_col < 5; cpy_col++) {
				if ((cpy_col > 0) || (cpy_row > 0)) {
					for (size_t row = 0; row < this->height; row++) {
						for (size_t col = 0; col < this->width; col++) {
							weight = this->weights[row][col] + cpy_col + cpy_row;
							if (weight > 9)
								weight -= 9; // "risk levels above 9 wrap back around to 1"
							dst_row = cpy_row * this->height + row;
							dst_col = cpy_col * this->width + col;
							this->weights[dst_row][dst_col] = weight;
							this->distance[dst_row][dst_col] = (unsigned int)-1;
							this->visited[dst_row][dst_col] = false;
						}
					}
				}
			}
		}

		this->height *= 5;
		this->width *= 5;
	}

	void parse(const vector<string> &input) {
		size_t row = 0;
		for (auto &str: input) {
			size_t col = 0;
			for (auto &ch: str) {
				this->weights[row][col] = ch - '0';
				this->distance[row][col] = (unsigned int)-1;
				this->visited[row][col] = false;
				col++;
			}
			row++;
			this->width = col;
		}
		this->height = row;
		duplicate();
	}

	Position minDistance() {
		// Initialize min value
		unsigned int min = (unsigned int)-1;
		Position min_pos;
 
		for (size_t row = 0; row < this->height; row++) {
			for (size_t col = 0; col < this->width; col++) {
				if (this->visited[row][col] == false && this->distance[row][col] <= min) {
					min = this->distance[row][col];
					min_pos = { col, row };
				}
			}
		}
 
		return min_pos;
	}

	void updateDistance(Position u, Position v) {
		// Update dist[v] only if is not visited, and total weight of path
		// from src to adj through u is smaller than current value of dist[adj]
		if (!this->visited[v.row][v.col] && (this->distance[u.row][u.col] != (unsigned int)-1) && ((this->distance[u.row][u.col] + this->weights[v.row][v.col]) < this->distance[v.row][v.col])) {
			this->distance[v.row][v.col] = this->distance[u.row][u.col] + this->weights[v.row][v.col];
		}
	}

	public:
	Simulation(const vector<string> &input) {
		parse(input);
	}

	void findPath() {
		Position src = {0, 0};
		this->distance[src.row][src.col] = 0;

		// Find shortest path for all vertices
		for (size_t row = 0; row < this->height; row++) {
			for (size_t col = 0; col < this->width; col++) {
				// Pick the minimum distance vertex from the set of vertices not
		        // yet processed. u is always equal to src in the first iteration.
				Position u = minDistance();

				// Mark the picked vertex as processed
				this->visited[u.row][u.col] = true;

				// Update dist value of the adjacent vertices of the picked vertex.
				Position adj;

				// Update up
				if (u.row > 0)  {
					adj = { u.col, u.row - 1 };
					updateDistance(u, adj);
				}

				// Update right
				if ((u.col + 1) < this->width)  {
					adj = { u.col + 1, u.row };
					updateDistance(u, adj);
				}

				// Update bottom
				if ((u.row + 1) < this->height)  {
					adj = { u.col, u.row + 1 };
					updateDistance(u, adj);
				}

				// Update left
				if (u.col > 0) {
					adj = { u.col - 1, u.row };
					updateDistance(u, adj);
				}
			}
		}
	}

	unsigned long getAnswer() const {
		return this->distance[this->height - 1][this->width - 1];
	}

	void printDistances() const {
		for (size_t row = 0; row < this->height; row++) {
			for (size_t col = 0; col < this->width; col++) {
				unsigned int dist = this->distance[row][col];
				if (dist < 10) {
					cout << ' ';
				}
				cout << to_string(dist) << ' ';
			}
			cout << endl;
		}
	}

	void printWeights() const {
		for (size_t row = 0; row < this->height; row++) {
			for (size_t col = 0; col < this->width; col++) {
				unsigned int dist = this->weights[row][col];
				cout << to_string(dist);
			}
			cout << endl;
		}
	}
};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day15b <input.txt>" << endl;
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
	Simulation sim(input);
	sim.findPath();
	//sim.printDistances();
	sim.printWeights();
	unsigned long answer = sim.getAnswer();
	cout << "Answer is " << to_string(answer) << endl;

	return 0;
}
