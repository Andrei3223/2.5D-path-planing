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

void A_star::save_to_txt(std::vector<std::pair<int, int>> path, char *name) {
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
    if (node.x + i < x_size_ && node.x + i >= 0 && node.y + j < y_size_ && node.y + j >= 0) {
        return true;
    }
    return false;
}

A_star::A_star(int argc, char **argv) : parents_vec_(std::stoi(argv[3]), std::vector<Node>(std::stoi(argv[4]))),
                                        used(std::stoi(argv[3]), std::vector<bool>(std::stoi(argv[4]), false)),
                                        x_size_(std::stoi(argv[3])), y_size_(std::stoi(argv[4])),
                                        landscape_(std::stoi(argv[3]), std::vector<double>(std::stoi(argv[4]))) {
    // Get all landscape data from file
    //  get size of vec from .txt
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
//            if (*(argv[i] + 1) == 'w')
            if (s == "-w") { // weight of height changing
                weight_h = std::stod(argv[i]);
            }
        }
    }

}





bool cmp::operator()(const Node &a, const Node &b) const {
    if (a.dist != b.dist) {
        return a.dist < b.dist;
    } else {
        return a.accumulated < b.accumulated;
    }
}


//struct pairhash {
//public:
//    std::size_t operator()(const std::pair<int, int> &x) const {
//        auto hash1 = std::hash<int>()(x.first);
//        auto hash2 = std::hash<int>()(x.first);
//        if (hash1 != hash2) {
//            return std::hash<int>()(x.first) ^ std::hash<int>()(x.second);
//        }
//        return std::hash<int>()(x.first);
//    }
//};