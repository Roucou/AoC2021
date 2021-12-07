#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

bool hasCharAtPos(string str, char c, int pos)
{
	return str.at(pos) == c;
}

int convertToInt(string s)
{
	int res = 0;
	for (size_t pos = 0; pos < s.length(); pos++)
	{
		res *= 2;
		if (s.at(pos) == '1')
		{
			res++;
		}
	}

	return res;
}

char getMostCommon(int pos, vector<string> &bit_strings)
{
	int zero_count = 0;
	int one_count = 0;

	for (const auto &str : bit_strings)
	{
		char c = str.at(pos);
		c == '0' ? zero_count++ : one_count++;	
	}
	
	return (zero_count > one_count) ? '0' : '1';
}

char getLeastCommon(int pos, vector<string> &bit_strings)
{
	int zero_count = 0;
	int one_count = 0;

	for (const auto &str : bit_strings)
	{
		char c = str.at(pos);
		c == '0' ? zero_count++ : one_count++;	
	}
	
	return (one_count < zero_count) ? '1' : '0';
}

int filter_ogr(int bits, vector<string> bit_strings)
{
	for (size_t pos = 0; pos < bits; pos++)
	{
		cout << "Iteration " << to_string(pos) << ": " << bit_strings.size() << endl;
		char c = getMostCommon(pos, bit_strings);
		cout << "Remove strings with " << c << " at position pos." << endl;
		for (auto it = bit_strings.begin(); it != bit_strings.end();)
		{
			if (!hasCharAtPos(*it, c, pos))
			{
				it = bit_strings.erase(it);
			}
			else
			{
				++it;
			}
		}
		if (bit_strings.size() == 1)
		{
			cout << "Break!" << endl;
			break;
		}
	}

	int ogr = convertToInt(bit_strings.front());

	cout << bit_strings.size() << " -> Should be 1!!!" << endl;
	cout << bit_strings.front() <<  " (" << to_string(ogr) << ")" << endl;
	
	return ogr;
}

int filter_csr(int bits, vector<string> bit_strings)
{
	for (size_t pos = 0; pos < bits; pos++)
	{
		cout << "Iteration " << to_string(pos) << ": " << bit_strings.size() << endl;
		char c = getLeastCommon(pos, bit_strings);
		cout << "Remove strings with " << c << " at position pos." << endl;
		for (auto it = bit_strings.begin(); it != bit_strings.end();)
		{
			if (!hasCharAtPos(*it, c, pos))
			{
				it = bit_strings.erase(it);
			}
			else
			{
				++it;
			}
		}
		if (bit_strings.size() == 1)
		{
			cout << "Break!" << endl;
			break;
		}
	}

	int csr = convertToInt(bit_strings.front());

	cout << bit_strings.size() << " -> Should be 1!!!" << endl;
	cout << bit_strings.front() <<  " (" << to_string(csr) << ")" << endl;
	
	return csr;
}

int main(int argc, char *argv[])
{
	std::string str;
	int bits = -1;
	vector<string> bit_strings;

	// First command line argument is the file to open
	if (argc != 2)
	{
		cout << "Usage: day3b <input.txt>" << endl;
		return -1;
	}

	// Open file
	string path = argv[1];
	ifstream file(path);

	// Read file, line per line
	while (getline(file, str))
	{
		bit_strings.push_back(str);

		// Set number of bits
		if (bits == -1)
		{
			bits = str.length();
			cout << "#Bits = " << to_string(bits) << endl;
		}
	}

	// Calculate & show life support rating
	int ogr = filter_ogr(bits, bit_strings);
	int csr = filter_csr(bits, bit_strings);
	int lsr = ogr * csr;
	std::cout << std::endl << "Answer: " << std::to_string(lsr) << std::endl;

	return 0;
}

