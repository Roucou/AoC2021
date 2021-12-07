#include <iostream>
#include <fstream>
#include <string>

class Window
{
	int _measure1 = -1;
	int _measure2 = -1;
	int _measure3 = -1;

	public:
		int sum()
		{
			return isComplete() ? _measure1 + _measure2 + _measure3 : -1;
		}

		void addMeasure(int measure)
		{
			_measure1 = _measure2;
			_measure2 = _measure3;
			_measure3 = measure;
		}

		bool isComplete()
		{
			return (_measure1 != -1) && (_measure2 != -1) && (_measure3 != -1);
		}

		void print()
		{
			std::cout << "(" << std::to_string(_measure1) << ", " << std::to_string(_measure2) << ", " << std::to_string(_measure3) << ")" << std::endl;
		}
};

int main(int, char**)
{
	int measure;
	Window window;
	bool first_measure = true;
	int sum;
	int increases = 0;
	int previous_sum;

	std::ifstream file("/home/koen/Projects/AoC2021/day1b/day1_input.txt");
	std::string str;
	while (std::getline(file, str))
	{
		measure = std::stoi(str);
		window.addMeasure(measure);
		// window.print();
		if (!window.isComplete()) continue;

		sum = window.sum();
		if (first_measure)
		{
			std::cout << std::to_string(sum) << " (N/A - no previous measurement)" << std::endl;
			first_measure = false;
		}
		else if (sum > previous_sum)
		{
			std::cout << std::to_string(sum) << " (increased)" << std::endl;
			increases = increases + 1;
		}
		else if (sum < previous_sum)
		{
			std::cout << std::to_string(sum) << " (decreased)" << std::endl;
		}
		else
		{
			std::cout << std::to_string(sum) << " (unchanged)" << std::endl;
		}
		previous_sum = sum;
	}
	std::cout << std::endl << "Number of increases: " << std::to_string(increases) << std::endl;
}

