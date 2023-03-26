#include "Solution.h"


int main()
{
    //I love Oumuamua!!!
    unsigned int start_time = clock();
    Solution solve;
    solve.act();
    unsigned int end_time = clock();
    std::cout << "For all project need " << (end_time - start_time) / 1000 << " seconds" << std::endl;
    return 0;
}

