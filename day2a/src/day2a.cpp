#include <iostream>
#include <fstream>
#include <string>

class Submarine
{
	int _x = 0;
	int _y = 0;
	int _z = 0; // !!! positive z = depth, negative z = height !!!

	public:
		int getX()
		{
			return _x;
		}

		int getY()
		{
			return _y;
		}

		int getZ()
		{
			return _z;
		}

		void moveForward(int amount)
		{
			_x += amount;
		}

		void moveBackward(int amount)
		{
			_x -= amount;
		}

		void increaseDepth(int amount)
		{
			_z += amount;
		}

		void decreaseDepth(int amount)
		{
			_z -= amount;
		}

		void print()
		{
			std::cout << "(" << std::to_string(_x) << ", " << std::to_string(_y) << ", " << std::to_string(_z) << ")" << std::endl;
		}
};

const static std::string FORWARD = "forward";
const static std::string DOWN = "down";
const static std::string UP = "up";

int main(int, char**)
{
	Submarine submarine;

	std::ifstream file("/home/koen/Projects/AoC2021/day2a/day2_input.txt");
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
			submarine.print();
		}
		else if (DOWN.compare(action) == 0)
		{
			submarine.increaseDepth(amount);
			submarine.print();
		}
		else if (UP.compare(action) == 0)
		{
			submarine.decreaseDepth(amount);
			submarine.print();
		}
	}
	int mult = submarine.getX() * submarine.getZ();
	std::cout << std::endl << "Answer: " << std::to_string(mult) << std::endl;
}

