#include <iostream>
#include "A_class.h"

int main(int argc, char *argv[]) {
    //  heuristics, threshold, barriers, get size from .txt data, parameter 4 nodes around, param.txt
//    A_star solution(argv[1], std::stoi(argv[3]), std::stoi(argv[4]));
    A_star solution(argc, argv);

    std::vector<std::pair<int, int>> result = solution.find_path({0, 0}, {3, 3});

    solution.save_to_txt(result, argv[2]);

    return 0;
}
