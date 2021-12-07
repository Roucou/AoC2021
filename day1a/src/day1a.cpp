#include <iostream>
#include <fstream>
#include <string>

int main(int, char**)
{
    int measure;
    bool first_measure = true;
    int increases = 0;
    int previous_measure;

    std::ifstream file("/home/koen/Projects/AoC2021/day1a/day1_input.txt");
    std::string str;
    while (std::getline(file, str))
    {
        measure = std::stoi(str);
        if (first_measure)
        {
            std::cout << str << " (N/A - no previous measurement)" << std::endl;
            first_measure = false;
        }
        else if (measure > previous_measure)
        {
            std::cout << str << " (increased)" << std::endl;
            increases = increases + 1;
        }
        else if (measure < previous_measure)
        {
            std::cout << str << " (decreased)" << std::endl;
        }
        else
        {
            std::cout << str << " (unchanged)" << std::endl;
        }
        previous_measure = measure;
    }
            std::cout << std::endl << "Number of increases: " << std::to_string(increases) << std::endl;
}
