#include <iostream>
#include <fstream>
#include <string>

int calculateGamma(int bits, int *ones, int *zeros)
{
	int gamma = 0;

	// Count most common bit
	for (size_t pos = 0; pos < bits; pos++)
	{
		gamma *= 2;
		if (ones[pos] > zeros[pos]) gamma++;
	}
	
	return gamma;
}

int calculateEpsilon(int bits, int *ones, int *zeros)
{
	int epsilon = 0;

	// Count most common bit
	for (size_t pos = 0; pos < bits; pos++)
	{
		epsilon *= 2;
		if (ones[pos] < zeros[pos]) epsilon++;
	}
	
	return epsilon;
}

int main(int argc, char *argv[])
{
	std::string str;
	int *ones;
	int *zeros;
	int bits = -1;

	// First command line argument is the file to open
	if (argc != 2)
	{
		std::cout << "Usage: day3a <input.txt>" << std::endl;
		return -1;
	}

	// Open file
	std::string path = argv[1];
	std::ifstream file(path);

	// Read file, line per line
	while (std::getline(file, str))
	{
		// Set number of bits
		if (bits == -1)
		{
			bits = str.length();
			ones = new int[bits];
			zeros = new int[bits];
			std::cout << "#Bits = " << std::to_string(bits) << std::endl;
		}

		// Count occurences per position
		for (size_t pos=0; pos < bits; pos++)
		{
			char c = str.at(pos);
			c == '1' ? ones[pos] += 1 : zeros[pos] += 1;
		}
	}

	// Calculate power consumption
	int gamma = calculateGamma(bits, ones, zeros);
	int epsilon = calculateEpsilon(bits, ones, zeros);

	// Show power consumption
	int power = gamma * epsilon;
	std::cout << std::endl << "Answer: " << std::to_string(power) << std::endl;

	// Clean up
	delete[] ones;
	delete[] zeros;

	return 0;
}

