#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>


class Helpers
{
public:
	std::vector<double> split(const std::string string, char seporator_1, char seporator_2)
    {
        std::vector<double> vector_strings_to_return;
        int word_starter = 0;
        
        for (int j = word_starter; j < string.length()+1; j++)
        {
            if ((string[j] == seporator_1) or (string[j] == seporator_2))
            {
                vector_strings_to_return.push_back(std::stod(string.substr(word_starter, j - word_starter)));
                word_starter = j+1;
            }
        }
        return vector_strings_to_return;
    }


    double POW_2(double value)
    {
        return value * value;
    }
};

