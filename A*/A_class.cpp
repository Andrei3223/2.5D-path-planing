#include "A_class.h"
#include "string"

struct Node_small {
    int x;
    int y;
};

std::vector<std::pair<int, int>> A_star::return_path(std::pair<int, int> s, std::pair<int, int> t) {
    // check if target is reachable
    if (parents_vec_[t.first][t.second].x == -1 && parents_vec_[t.first][t.second].y == -1) {
        printf("Target is unreachable!!!\n");
        exit(1);
    }
    Node_small cur_node = {t.first, t.second};
    std::vector<std::pair<int, int>> path = {t};
    while (cur_node.x != s.first || cur_node.y != s.second) {
        int tmp = cur_node.x;
        cur_node.x = parents_vec_[cur_node.x][cur_node.y].x;
        cur_node.y = parents_vec_[tmp][cur_node.y].y;
        path.emplace_back(cur_node.x, cur_node.y);
    }
    // If you want to print the path in terminal - uncomment:
//    for (const auto& pair: path) {
//        printf("%d %d\n", pair.first, pair.second);
//    }
    return path;
}

A_star::A_star(char *input, int size_x, int size_y) : parents_vec_(size_x, std::vector<Node>(size_y)),
                                                      used(size_x, std::vector<bool>(size_y, false)),
                                                      x_size_(size_x), y_size_(size_y),
                                                      landscape_(size_x, std::vector<double>(size_y)) {
    // Get all landscape data from file
    //  get size of vec from .txt
    std::ifstream input_st(input, std::ios::in);
    if (!input_st.is_open()) {
        throw std::runtime_error("can't open input file");
    }
    for (int i = 0; i < size_x; ++i) {
        for (int j = 0; j < size_y; ++j) {
            input_st >> landscape_[i][j];
        }
    }

}

void A_star::save_to_txt(std::vector<std::pair<int, int>> &path, char *name) {
    std::ofstream output_st(name, std::ios::out);
    if (!output_st.is_open()) {
        throw std::runtime_error("can't open output file");
    }
    for (const auto &pair: path) {
        output_st << pair.first << " " << pair.second << " " << landscape_[pair.first][pair.second] << '\n';
    }
}

std::vector<std::pair<int, int>> A_star::find_path(std::pair<int, int> s, std::pair<int, int> t) {
    set_.insert({s.first, s.second, 0., 0.});
    while (!set_.empty()) {
        Node cur = *set_.begin();
        set_.erase(set_.begin());
        used[cur.x][cur.y] = true;

        if (cur.x == t.first && cur.y == t.second) {
            break;
        }
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) {
                    continue;
                }
                if (!valid_coordinate(cur, i, j)) {
                    continue;
                }
                // check if node is already visited
                if (used[cur.x + i][cur.y + j]) {
                    continue;
                }
                // check if node exist
                if (landscape_[cur.x + i][cur.y + j] <= MISSED_VALUE) {
                    continue;
                }
                // check if height is acceptable
                if (landscape_[cur.x + i][cur.y + j] < min_threshold || landscape_[cur.x + i][cur.y + j] > max_threshold) {
                    continue;
                }

                double h_val = h(cur.x + i, cur.y + j, t.first, t.second);
                double g_val = g(cur, cur.x + i, cur.y + j);

                if (parents_vec_[cur.x + i][cur.y + j].dist > h_val + g_val) {
                    set_.insert({cur.x + i, cur.y + j, g_val, h_val + g_val});
                    parents_vec_[cur.x + i][cur.y + j] = {cur.x, cur.y, g_val, h_val + g_val};
                }
            }
        }
    }

    return return_path(s, t);
}

bool A_star::valid_coordinate(Node &node, int i, int j) const {
    if (four_neighbours) {
        // we can't move diagonally
        if (std::abs(i) + std::abs(j) > 1) {
            return false;
        }
    }
    if (node.x + i >= x_size_ || node.x + i < 0 || node.y + j >= y_size_ || node.y + j < 0) {
        return false;
    }

    // check if angle is acceptable
    double dh = landscape_[node.x][node.y] - landscape_[node.x + i][node.y + j];
    if (dh < 0) {
        dh *= -1.;
    }
    double atg = atan(dh / cell_len);
    if (atan(dh / cell_len) > angle_vert) {
        return false;
    }
    return true;
}


A_star::A_star(int argc, char **argv) : parents_vec_(std::stoi(argv[3]), std::vector<Node>(std::stoi(argv[4]))),
                                        used(std::stoi(argv[3]), std::vector<bool>(std::stoi(argv[4]), false)),
                                        x_size_(std::stoi(argv[3])), y_size_(std::stoi(argv[4])),
                                        landscape_(std::stoi(argv[3]), std::vector<double>(std::stoi(argv[4]))) {
    // Get all landscape data from file
    // TODO get size of vec from .txt
    std::ifstream input_st(argv[1], std::ios::in);
    if (!input_st.is_open()) {
        throw std::runtime_error("can't open input file");
    }
    for (int i = 0; i < x_size_; ++i) {
        for (int j = 0; j < y_size_; ++j) {
            input_st >> landscape_[i][j];
        }
    }

    // read parameters
    for (int i = 5; i < argc; ++i) {
        if (*argv[i] == '-') {
            std::string s = argv[i];
            ++i;
            if (s == "-w") { // weight of height changing
                weight_h = std::stod(argv[i]);
            } else if (s == "-s") { // source (start)
                source.first = std::stoi(argv[i++]);
                source.second = std::stoi(argv[i]);
            } else if (s == "-t") { // target (finish)
                target.first = std::stoi(argv[i++]);
                target.second = std::stoi(argv[i]);
            } else if (s == "-lb" || s == "--lower_bound") {
                min_threshold = std::stod(argv[i]);
            } else if (s == "-ub" || s == "--upper_bound") {
                max_threshold = std::stod(argv[i]);
            } else if (s == "-f" || s == "--4_neighbour") {
                four_neighbours = true;
                --i;
            } else if (s == "-l" || s == "--cell_len") {
                cell_len = std::stod(argv[i]);
            } else if (s == "-av" || s == "--angle_vertical") {
                angle_vert = std::stod(argv[i]) * pi / 180.;
            }
        }
    }
    if (source.first == -1 && source.second == -1) {
        throw std::runtime_error("start is no defined\n");
    }
    if (target.first == -1 && target.second == -1) {
        throw std::runtime_error("target is no defined\n");
    }
    if (source.first < 0 || source.first >= x_size_ || source.second < 0 || source.second >= y_size_) {
        throw std::runtime_error("start is out of the map limits\n");
    }
    if (target.first < 0 || target.first >= x_size_ || target.second < 0 || target.second >= y_size_) {
        throw std::runtime_error("start is out of the map limits\n");
    }
}


double A_star::h(int x, int y, int target_x, int target_y) {
    double dif_h = landscape_[x][y] - landscape_[target_x][target_y];
    dif_h *= weight_h;
    auto dif_x = static_cast<double>(target_x - x);
    auto dif_y = static_cast<double>(target_y - y);

    if (!four_neighbours) { // 8 neighbours
        // Euclidean distance
        return std::sqrt(dif_h * dif_h + dif_x * dif_x + dif_y * dif_y);
    } else {
        // Manhattan distance for x y
        double dif_xy = dif_y + dif_x;
        return std::sqrt(dif_h * dif_h + dif_xy * dif_xy);
    }
}

double A_star::g(Node &parent, int x, int y) {
    double dif_h = landscape_[parent.x][parent.y] - landscape_[x][y];
    dif_h *= weight_h;
    auto dif_x = static_cast<double>(parent.x - x);
    auto dif_y = static_cast<double>(parent.y - y);
    double c = std::sqrt(dif_h * dif_h + dif_x * dif_x + dif_y * dif_y);
    return parent.accumulated + c;
}

bool cmp::operator()(const Node &a, const Node &b) const {
    if (a.dist != b.dist) {
        return a.dist < b.dist;
    } else {
        return a.accumulated < b.accumulated;
    }
}