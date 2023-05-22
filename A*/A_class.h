#include <cmath>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <numbers>
#include <set>
#include <vector>


enum {
    MISSED_VALUE = -9999
}; // all the data less than this number will be ignored (some data can be lost, those nodes = MISSED_value)

struct Node {
    Node() : x(-1), y(-1), accumulated(-1), dist(std::numeric_limits<double>::max()) {};

    Node(int a, int b, double h, double d) : x(a), y(b), accumulated(h), dist(d) {};
    int x;
    int y;
    double accumulated;
    double dist;
};

struct cmp {
    bool operator()(const Node &a, const Node &b) const;
};


class A_star {
public:
    explicit A_star(std::vector<std::vector<double>> &vec) : landscape_(vec),
                                                             parents_vec_(vec.size(), std::vector<Node>(vec[0].size())),
                                                             used(vec.size(), std::vector<bool>(vec[0].size(), false)),
                                                             x_size_(vec.size()), y_size_(vec[0].size()) {};

    A_star(char *input, int size_x, int size_y);

    A_star(int argc, char *argv[]);

    std::vector<std::pair<int, int>> find_path(std::pair<int, int> s, std::pair<int, int> t);

    std::vector<std::pair<int, int>> return_path(std::pair<int, int> s, std::pair<int, int> t);

    void save_to_txt(std::vector<std::pair<int, int>>& path, char *name);

    std::pair<int, int> source, target;
private:
// c = dist between two adjacent grids
// g = sum(c) - from start to the grid
// h = heuristic function (Euclidean space between node and the target)
    double h(int x, int y, int target_x, int target_y);

    double g(Node &parent, int x, int y);

    bool valid_coordinate(Node &node, int i, int j) const;

    std::vector<std::vector<double>> landscape_; // initial landscape
    std::vector<std::vector<bool>> used;
    std::vector<std::vector<Node>> parents_vec_; // coordinates of parent and current distance to the node
    std::set<Node, cmp> set_;
    size_t x_size_; // vec.size()
    size_t y_size_; // vec[0].size()

    // parameters
    double weight_h = 1;
    double min_threshold = std::numeric_limits<double>::lowest();
    double max_threshold = std::numeric_limits<double>::max();
    bool four_neighbours = false;

    double cell_len = 1;
    double pi = acos(-1);
    double angle_vert = pi / 2;
};


