#ifndef _HEXBOARD_H_
#define _HEXBOARD_H_

#include <algorithm>
#include <cmath>
#include <map>
#include <random>
#include <set>
#include <vector>

#include "hexcell.h"

using std::map;
using std::mt19937;
using std::pair;
using std::set;
using std::vector;
using namespace cs221util;

class HexBoard {
    public:
        // variables

        // Longest path distance from the start_coord to any cell.
        int longest_path_dist;

        // Radius of the board
        int radius;

        // The start coordinate of the maze
        pair<int, int> start_coord;

        // The end coordinate of the maze
        pair<int, int> end_coord;

        // Solution of the maze. Contains pairs representing the path from the start_coord to the exit
        vector<pair<int, int>> solution;

        // A mapping from the coordinates of the cells to a pointer to the cell.
        map<pair<int, int>, HexCell*> cells;

        // The RNG to use for generate_maze and chooseNeighbour
        mt19937 rng;

        // functions

        /**
         * Constructor. Creates a board with given radius, creating new HexCells and inserting them into this->cells by calling createBoard(). The default start_coord and end_coord are (0, 0).
         * @note There is no maze yet! The board is just a beehive for now.
         * @param rad The radius of the board
         */
        HexBoard(int rad);

        // Frees all dynamically allocated memory associated with the board.
        ~HexBoard();

        /**
         * Link two cells at the given coordinates by setting each cell's wall to false in the other cell's direction.
         * @param coord1 A coordinate of a HexCell to be linked.
         * @param coord2 A coordinate of a HexCell to be linked.
         * @note remember to link both directions!
         * REQUIRE: the two coords are neighbours
         */
        void link(const pair<int, int>& coord1, const pair<int, int>& coord2) const;

        /**
         * @param coord1 A coordinate of a HexCell
         * @param coord2 A coordinate of a HexCell
         * @return The cell distance (ignoring walls) between coord1 and coord2.
         */
        int distance(const pair<int, int>& coord1, const pair<int, int>& coord2) const;

        /**
         * @return true if coord is at the edge of the board, false otherwise.
         */
        bool is_edge(const pair<int, int>& coord) const;

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
        void generate_maze(pair<int, int> start, double branch_prob, int seed);

        /**
          * Populate (member variable) solution as a list of coordinates from start_coord to end_coord, represented as {q, r} pairs.

          * REQUIRE: generate_maze must be called already
          */
        void solve_maze();

    private:
        /**
         * Creates all neccessary HexCells up to the given radius and insert them into this->cells.
         */
        void create_board();

        /**
         * Gets the coordinates of the six neighbours of coord, in the sides order specified in HexCell.h.
         * @param coord The input coordinate.
         */
        vector<pair<int, int>> get_neigh_coords(const pair<int, int>& coord) const;

        /**
         * THIS IS A GIVEN FUNCTION.
         *
         * Choose a random unvisited neighbour of cell at given coordinate, and returns either
         *    a. a neighbour in the same ring as cell (with probability 1 - branch_prob)
         *    b. a neighbour in a different ring as cell (with probability branch_prob)
         *
         * If all neighbours are in the same ring as cell, then returned neighbour will be
         * in the same ring. Similarly, if all neighbours are in different ring(s) as cell,
         * then returned neighbour will be in a different ring.
         *
         * If there are no unvisited neighbours, returns nullptr.
         *
         * @param coord The coord to choose a random neighbour from.
         * @param visited A set of visited coordinates.
         * @param branch_prob The probability that a random neighbour from a different ring
         * is chosen, if there are neighbours both in the same ring and different rings.
         * @note rings are centred at the *start_coord*, not the origin.
         */
        HexCell* choose_neighbour(const pair<int, int>& coord, const set<pair<int, int>>& visited, double branch_prob);

        /**************************************************************************************/
        /* IF YOU WISH TO DEFINE ANY PRIVATE MEMBER FUNCTIONS, ADD THEM TO hexboard-private.h */
        /**************************************************************************************/

        #include "hexboard-private.h"
};

#endif