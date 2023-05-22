#include <iostream>
#include "A_class.h"

int main(int argc, char *argv[]) {
    // TODO get size from .txt data, param.txt
    A_star solution(argc, argv);

    std::vector<std::pair<int, int>> result = solution.find_path(solution.source, solution.target);

    solution.save_to_txt(result, argv[2]);
    return 0;
}
