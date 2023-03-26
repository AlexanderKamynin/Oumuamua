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
                // correct wrong symbol in string if they are
                std::string sub_str = string.substr(word_starter, abs(i - word_starter));
                std::string correct_str = "";
                for (int j = 0; j < sub_str.size(); j++) 
                {
                    if (sub_str[j] == '.' or (sub_str[j] >= '0' and sub_str[j] <= '9'))
                    {
                        correct_str += sub_str[j];
                    }
                }
                if (correct_str != "")
                {
                    vector_strings_to_return.push_back(std::stod(correct_str));
                }
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

