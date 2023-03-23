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

        for (int i = word_starter; i < string.length() + 1; i++)
        {
            if ((string[i] == seporator_1) or (string[i] == seporator_2))
            {
                vector_strings_to_return.push_back(std::stod(string.substr(word_starter, abs(i - word_starter))));
                word_starter = i + 1;
            }
            if (vector_strings_to_return.size() == 3)
            {
                return vector_strings_to_return;
            }
        }
        return vector_strings_to_return;
    }


    double POW_2(double value)
    {
        return value * value;
    }
};

