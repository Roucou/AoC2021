#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::string;
using std::string_view;
using std::to_string;
using std::unique_ptr;
using std::vector;

class Node {
	static void dfs5(shared_ptr<Node> node, int &depth, shared_ptr<Node> &found_node, shared_ptr<Node> &previous_value, shared_ptr<Node> &next_value) {
		if ((found_node != nullptr) && (previous_value != nullptr) && (next_value != nullptr)) {
			return;
		}

		if ((found_node == nullptr) && (node->left != nullptr) && (node->right != nullptr) && (depth == 4)) {
			found_node = node;
			return;
		}

		if ((found_node == nullptr) && (node->left == nullptr) && (node->right == nullptr)) {
			previous_value = node;
		}

		if ((found_node != nullptr) && (next_value == nullptr) && (node->left == nullptr) && (node->right == nullptr)) {
			next_value = node;
			return;
		}

		if (node->left != nullptr) {
			depth++;
			dfs5(node->left, depth, found_node, previous_value, next_value);
			dfs5(node->right, depth, found_node, previous_value, next_value);
			depth--;
		}
	}

	static void dfs10(shared_ptr<Node> node, shared_ptr<Node> &value_node) {
		if (value_node != nullptr) {
			return;
		}
	
		if ((node->left == nullptr) && (node->right == nullptr) && (node->value >= 10)) {
			value_node = node;
			return;
		}

		if (node->left != nullptr) {
			dfs10(node->left, value_node);
			dfs10(node->right, value_node);
		}
	}

	public:
	unsigned int value = 0;
	unsigned int magnitude = 0;
	shared_ptr<Node> left = nullptr;
	shared_ptr<Node> right = nullptr;

	static void reduce(shared_ptr<Node> root) {
		bool fully_reduced;

		do {
			fully_reduced = true;

			// Using depth-first-search, find first pair with depth == 4;
			int depth = 0;
			shared_ptr<Node> found_pair = nullptr;
			shared_ptr<Node> previous_value = nullptr;
			shared_ptr<Node> next_value = nullptr;
			dfs5(root, depth, found_pair, previous_value, next_value);
			if (found_pair != nullptr) {
				// Explode
				if (previous_value != nullptr) {
					previous_value->value += found_pair->left->value;
				}
				if (next_value != nullptr) {
					next_value->value += found_pair->right->value;
				}
				found_pair->value = 0;
				found_pair->left = nullptr;
				found_pair->right = nullptr;
				fully_reduced = false;
			}
		
			if (fully_reduced) {
				// Nothing to explode, anything to split?
				shared_ptr<Node> value_node = nullptr;
				dfs10(root, value_node);
				if (value_node != nullptr) {
					unsigned int value = value_node->value;
					shared_ptr<Node> left_value_node = make_shared<Node>();
					left_value_node->value = value / 2;
					value_node->left = left_value_node;
					shared_ptr<Node> right_value_node = make_shared<Node>();
					right_value_node->value = ((value % 2) == 0) ? (value / 2) : ((value / 2) + 1);
					value_node->right = right_value_node;
					value_node->value = 0;
					fully_reduced = false;
				}
			}
		} while (!fully_reduced);
	}

	static void calculateMagnitude(shared_ptr<Node> node) {
		if (node == nullptr) {
			return;
		}

		shared_ptr<Node> left_node = node->left;
		shared_ptr<Node> right_node = node->right;

		if ((left_node == nullptr) && (right_node == nullptr)) {
			node->magnitude = node->value;
			return;
		}

		calculateMagnitude(left_node);
		calculateMagnitude(right_node);

		node->magnitude = (left_node->magnitude * 3) + (right_node->magnitude * 2);
	}

	static void deepcopy(const shared_ptr<Node> &src, shared_ptr<Node> &dst) {
		if (src != nullptr) {
			dst = make_shared<Node>();
			dst->value = src->value;
			dst->magnitude = src->magnitude;
			deepcopy(src->left, dst->left);
			deepcopy(src->right, dst->right);
		} else {
			dst = nullptr;
		}
	}

	static void print(shared_ptr<Node> node) {
		if (node != nullptr) {
			if ((node->left == nullptr) && (node->right == nullptr)) {
				cout << to_string(node->value);
			} else {
				cout << '[';
				print(node->left);
				cout << ',';
				print(node->right);
				cout << ']';
			}
		}
	}
};

class SnailfishNumberParser {

	public:
	static void parse(string_view &view, shared_ptr<Node> node) {
		if (!view.empty()) {
			const char c = view.front();
			if (c == '[') {
				view.remove_prefix(1); // '['
				shared_ptr<Node> left_node = make_shared<Node>();
				parse(view, left_node);
				node->left = left_node;
				view.remove_prefix(1); // ','
				shared_ptr<Node> right_node = make_shared<Node>();
				parse(view, right_node);
				node->right = right_node;
				view.remove_prefix(1); // ']'
			} else if ((c >= '0') && (c <= '9')) {
				// 0 - 9
				view.remove_prefix(1); // '0' - '9'
				node->value = c - '0';
			}
		}
	}

};

class SnailfishNumber {
	shared_ptr<Node> root = nullptr;

	public:
	SnailfishNumber() {
		this->root = make_shared<Node>();
	}

	SnailfishNumber(const string &str) {
		SnailfishNumberParser parser;
		std::string_view strv(str);
		this->root = make_shared<Node>();
		parser.parse(strv, this->root);
	}

	static void deepcopy(unique_ptr<SnailfishNumber> &src, unique_ptr<SnailfishNumber> &dst) {
		dst = make_unique<SnailfishNumber>();
		Node::deepcopy(src->root, dst->root);
	}

	static unique_ptr<SnailfishNumber> add(unique_ptr<SnailfishNumber> &left, unique_ptr<SnailfishNumber> &right) {
		unique_ptr<SnailfishNumber> sum = make_unique<SnailfishNumber>();

		sum->root->left = left->root;
		sum->root->right = right->root;

		Node::reduce(sum->root);

		return sum;
	}

	unsigned int calculateMagnitude() const {
		Node::calculateMagnitude(this->root);
		return this->root->magnitude;
	}

	void print() const {
		Node::print(this->root);
		cout << '\n';
	}
};

class Calculator {
	vector<unique_ptr<SnailfishNumber>> fishes;
	int answer;

	void parse(const string &str) {
		unique_ptr<SnailfishNumber> fish = make_unique<SnailfishNumber>(str);
		this->fishes.push_back(std::move(fish));
	}

	public:
	Calculator(const vector<string> &strings) {
		for (auto &str: strings) {
			parse(str);
		}
	}

	void calculate() {
		unsigned int max_magnitude = 0;
		unsigned int magnitude;
		unique_ptr<SnailfishNumber> fish1 = nullptr;
		unique_ptr<SnailfishNumber> fish2 = nullptr;
		unique_ptr<SnailfishNumber> fish_sum = nullptr;

		for (size_t idx1 = 0; idx1 < this->fishes.size(); idx1++) {
			for (size_t idx2 = 0; idx2 < this->fishes.size(); idx2++) {
				if (idx1 == idx2) {
					continue;
				}
				SnailfishNumber::deepcopy(this->fishes.at(idx1), fish1);
				SnailfishNumber::deepcopy(this->fishes.at(idx2), fish2);
				fish_sum = SnailfishNumber::add(fish1, fish2);
				magnitude = fish_sum->calculateMagnitude();
				max_magnitude = std::max(magnitude, max_magnitude);
			}
		}

		// Calculate magnitude
		this->answer = max_magnitude;
	}

	int getAnswer() const {
		return this->answer;
	}

};

int main(int argc, char *argv[]) {
	// Grab command line arguments
	if (argc != 2) {
		cout << "Usage: day18b <input.txt>" << '\n';
		return -1;
	}

	// Open & read file
	string path = argv[1];
	string str;
	std::ifstream file(path);
	vector<string> input;
	while (getline(file, str)) {
		input.push_back(str);
	}

	// And go!
	Calculator c(input);
	c.calculate();
	unsigned long answer = c.getAnswer();
	cout << "Answer is " << to_string(answer) << '\n';

	return 0;
}
