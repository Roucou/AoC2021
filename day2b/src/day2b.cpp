#include <iostream>
#include <fstream>
#include <string>

class Submarine
{
	int _x = 0;
	int _depth = 0; // !!! increase depth = go down, decrease depth = go up !!!
	int _aim = 0;

	public:
		int getX()
		{
			return _x;
		}

		int getDepth()
		{
			return _depth;
		}

		int getAim()
		{
			return _aim;
		}

		void moveForward(int amount)
		{
			_x += amount;
			_depth += _aim * amount;
		}

		void moveUp(int amount)
		{
			_aim -= amount;
		}

		void moveDown(int amount)
		{
			_aim += amount;
		}

		void print()
		{
			std::cout << "(x: " << std::to_string(_x) << ", depth: " << std::to_string(_depth) << ", aim: " << std::to_string(_aim) << ")" << std::endl;
		}
};

const static std::string FORWARD = "forward";
const static std::string DOWN = "down";
const static std::string UP = "up";

int main(int argc, char *argv[])
{
	Submarine submarine;

	if (argc != 2)
	{
		std::cout << "Usage: day2b <input.txt>" << std::endl;
		return -1;
	}

	std::string path = argv[1];
	std::ifstream file(path);
	std::string str;
	while (std::getline(file, str))
	{
		int pos = str.find(' ');
		std::string action = str.substr(0, pos);
		std::string amountStr = str.substr(pos + 1);
		int amount = std::stoi(amountStr);
		std::cout << action << ' ' << std::to_string(amount) << std::endl;

		if (FORWARD.compare(action) == 0)
		{
			submarine.moveForward(amount);
			//submarine.print();
		}
		else if (DOWN.compare(action) == 0)
		{
			submarine.moveDown(amount);
			//submarine.print();
		}
		else if (UP.compare(action) == 0)
		{
			submarine.moveUp(amount);
			//submarine.print();
		}
	}
	int mult = submarine.getX() * submarine.getDepth();
	std::cout << std::endl << "Answer: " << std::to_string(mult) << std::endl;

	return 0;
}

