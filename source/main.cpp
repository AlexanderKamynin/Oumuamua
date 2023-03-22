#include "Solution.h"


int main() {
	//I love Oumuamua!!!
    Solution solution;
    solution.read_data();
    solution.convert_observations();
    solution.convert_observatory();
    solution.integrate();
    return 0;
}