#include "hexboard.h"

#include <iostream>

#include "queue.h"
#include "stack.h"

/**
 * Constructor. Creates a board with given radius, creating new HexCells and inserting them into this->cells by calling createBoard(). The default start_coord and end_coord are (0, 0).
 * @note There is no maze yet! The board is just a beehive for now.
 * @param rad The radius of the board
 */
HexBoard::HexBoard(int rad) {
    radius = rad; 
    longest_path_dist = 0;
    create_board();
    start_coord = {0, 0};
    end_coord = {0, 0};
    //longest_path_dist = 0;
}

// Frees all dynamically allocated memory associated with the board.
HexBoard::~HexBoard() {
    for (auto kv : cells) {
        delete kv.second;
        kv.second = NULL;
    } 
    cells.clear();
    solution.clear();
}

/**
 * Creates all neccessary HexCells up to the given radius and insert them into this->cells.
 */
void HexBoard::create_board() {
    for (int q = -radius; q <= radius; q++) {
        int rMin = max(-radius, -q-radius);
        int rMax = min(radius, -q + radius);

        for (int r = rMin; r <= rMax; r++) {
            std::pair<int, int> coord = {q, r};
            HexCell* newCell = new HexCell(coord);
            this->cells[coord] = newCell;

        }
    }
}

/**
 * Gets the coordinates of the six neighbours of coord, in the sides order specified in HexCell.h.
 * @param coord The input coordinate.
 */
vector<pair<int, int>> HexBoard::get_neigh_coords(const pair<int, int>& coord) const {

    int q = coord.first; 
    int r = coord.second; 
    vector<pair<int, int>> coords;
    coords.resize(6);
    coords[0] = {q, r-1};
    coords[1] = {q+1, r-1};
    coords[2] = {q+1, r};
    coords[3] = {q, r+1};
    coords[4] = {q-1, r+1};
    coords[5] = {q-1, r};

    return coords; 
}

/**
 * Link two cells at the given coordinates by setting each cell's wall to false in the other cell's direction.
 * @param coord1 A coordinate of a HexCell to be linked.
 * @param coord2 A coordinate of a HexCell to be linked.
 * @note remember to link both directions!
 * REQUIRE: the two coords are neighbours
 */
void HexBoard::link(const pair<int, int>& coord1, const pair<int, int>& coord2) const {
   
    auto lookup1 = cells.find(coord1);
    auto lookup2 = cells.find(coord2);

    HexCell* cell1 = lookup1->second;
    HexCell* cell2 = lookup2->second;

    int wallc1 = cell1->get_neighbour_side(cell2); 
    int wallc2 = cell2->get_neighbour_side(cell1);
    cell1->walls[wallc1] = false;
    cell2->walls[wallc2] = false;
}

/**
 * @param coord1 A coordinate of a HexCell
 * @param coord2 A coordinate of a HexCell
 * @return The cell distance (ignoring walls) between coord1 and coord2.
 */
int HexBoard::distance(const pair<int, int>& coord1, const pair<int, int>& coord2) const {
    
    int diffQ = coord2.first - coord1.first; 
    int diffR = coord2.second - coord1.second; 
    // there is a difference in only q or only r; not both
    if (diffQ == 0) return abs(diffR);
    if (diffR == 0) return abs(diffQ);

    // diagonals and others

    // difference is equal and opposite sign; q increase while r decrease, or vice versa
    if (diffQ == (-diffR)) return abs(diffQ);

    // if the difference has the same sign
    if (((diffQ > 0) && (diffR > 0)) || ((diffQ < 0) && (diffR < 0))) return abs(diffQ) + abs(diffR);

    // last case: if the difference is opposite sign and not equal
    return max(abs(diffQ), abs(diffR));
}

/**
 * @return true if coord is at the edge of the board, false otherwise.
 */
bool HexBoard::is_edge(const pair<int, int>& coord) const {
    
    int q = coord.first; 
    int r = coord.second; 
    if (abs(q) == radius || abs(r) == radius) return true; 
    if (abs(q + r) == radius) return true; 
    return false;
}

/**
 * Traverses the board via dfs to generate the maze, updating and keeping track of:
 * A. The largest path distance from start to an edge cell so far (this is not longest_path_dist!)

 * The algorithm goes as follows:
 * 0. Set the start_coord member variable
 * 1. Seed this->rng with seed
 * 2. Add the start_coord to a stack, and mark it as visited
 * 3. While the stack is not empty:

 *   a. Peek the top of the stack (call it curr_coord)
 *   b. Use choose_neighbour to choose the neighbour to visit
 *   c. If the neighbour exists, for that neighbour (call it neigh_cell) do:

 *     i. Link curr_coord with neigh_coord
 *     ii. Push neigh_coord into the stack, and mark it as visited
 *     iii. Update the neigh_cell's path distance to start
 *     iv. If necessary, update length of longest path so far (longest_path_dist)
 *     v. Update the end coordinate only if its path distance to start is strictly greater than the largest path distance from start to an edge cell so far.

 *   d. Otherwise (if choose_neighbour returns nullptr), pop the stack.

 * @param start The {q, r} coordinate of the start of the maze.
 * @param branch_prob Specifies how often it should "branch out" when choosing neighbours.
 *                    A small branch_prob means neighbours of the same dist from the start_coord are preferred
 *                    A large branch_prob means neighbours of a different dist from the start_coord are preferred
 * @param seed The seed used for the rng
 * REQUIRE: create_board() is already called
 */
void HexBoard::generate_maze(pair<int, int> start, double branch_prob, int seed) {
    
    start_coord = start; 
    this->rng.seed(seed); 
    set<pair<int, int>> visited = {start_coord};
    Stack<pair<int, int>> myStack;
    myStack.push(start_coord);
    int psf = 0; // represents the path distance from start so far
    longest_path_dist = 0;
    int largestToEdge = 0; 

    while (!myStack.is_empty()) { 

        pair<int, int> curr_coord = myStack.peek();
        HexCell* neigh_cell = choose_neighbour(curr_coord, visited, branch_prob); 

        if (neigh_cell != nullptr) {

            pair<int, int> neigh_coord = neigh_cell->qr; 
            visited.insert(neigh_coord);
            link(curr_coord, neigh_coord);
            myStack.push(neigh_coord); 

            //neigh_cell's path distance from start
            neigh_cell->path_dist_from_start = ++psf; 
            int neighLen = neigh_cell->path_dist_from_start;


            if (neighLen > longest_path_dist) {
                longest_path_dist = neighLen;
            } 
            if (neighLen > largestToEdge) {
                end_coord = neigh_coord; 
            }
            if (is_edge(neigh_coord)) {
                largestToEdge = max(largestToEdge, neighLen);
            }
        } else {
            myStack.pop();
            psf--;
        }
    }
}

/**
 * Populate (member variable) solution as a list of coordinates from start_coord to end_coord, represented as {q, r} pairs.

 * REQUIRE: generate_maze must be called already
 */
void HexBoard::solve_maze() {
    set<pair<int, int>> visited; 
    vector<pair<int, int>> path;
    solve_maze(visited, start_coord, path);
    
}


void HexBoard::solve_maze(set<pair<int, int>> visited, pair<int, int> currCoord, vector<pair<int, int>> path) {

    if (currCoord == end_coord) {
        path.push_back(currCoord);
        solution = path; 
        return;
    }

    // already visited or cell does not exist 
    if (visited.find(currCoord)!= visited.end()) return;

    // looking to see if cell exists
    auto lookup = cells.find(currCoord);
    if (lookup == cells.end()) return;

    HexCell* currCell = lookup->second;
    visited.insert(currCoord);
    path.push_back(currCoord);

    vector<pair<int, int>> neighBours = get_neigh_coords(currCoord);
    int i = 0; 
    for (auto neighBour : neighBours) {
        if (currCell->walls[i] == false) {
            if (neighBour == end_coord) {
                // path.push_back(currCoord);
                // solution = path;
                solve_maze(visited, neighBour, path);
                return;
            } 
            solve_maze(visited, neighBour, path);
        }
        i++;
    }


}