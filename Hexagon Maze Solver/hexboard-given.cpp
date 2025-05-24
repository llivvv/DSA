#include "hexboard.h"

HexCell* HexBoard::choose_neighbour(const pair<int, int>& coord, const set<pair<int, int>>& visited, double branch_prob) {
    vector<pair<int, int>> neigh_same_dist;
    vector<pair<int, int>> neigh_diff_dist;

    for (const auto& neigh_coord : get_neigh_coords(coord)) {
        if (cells.find(neigh_coord) != cells.end() && visited.find(neigh_coord) == visited.end()) {
            if (distance(neigh_coord, start_coord) == distance(coord, start_coord)) {
                neigh_same_dist.push_back(neigh_coord);
            } else {
                neigh_diff_dist.push_back(neigh_coord);
            }
        }
    }

    if (neigh_same_dist.empty() && neigh_diff_dist.empty()) {
        return nullptr;
    } else if (neigh_same_dist.empty()) {
        int idx = rng() % neigh_diff_dist.size();
        return cells.at(neigh_diff_dist[idx]);
    } else if (neigh_diff_dist.empty()) {
        int idx = rng() % neigh_same_dist.size();
        return cells.at(neigh_same_dist[idx]);
    } else {
        int x = ((double) rng()) / ((double) mt19937::max());

        if (x <= branch_prob) {
            int idx = rng() % neigh_diff_dist.size();
            return cells.at(neigh_diff_dist[idx]);
        } else {
            int idx = rng() % neigh_same_dist.size();
            return cells.at(neigh_same_dist[idx]);
        }
    }
}