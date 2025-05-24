#define CATCH_CONFIG_MAIN
#include "cs221util/catch.hpp"

#include <iostream>

#include "drawmaze.h"

using namespace std;

// FOR PRODUCING OUTPUT IMAGES
const bool writeoutputs = true;

TEST_CASE("HexCell::constructor", "[weight=1][part=hexcell]") {
    HexCell cell({5, 7});
    REQUIRE(cell.qr.first == 5);
    REQUIRE(cell.qr.second == 7);

    for (int i = 0; i < 6; i++) {
        REQUIRE(cell.walls[i]);
    }

    REQUIRE(cell.path_dist_from_start == 0);
}

TEST_CASE("HexBoard::distance", "[weight=1][part=hexboard]") {
    HexBoard board(4);
    vector<pair<int, int>> cells = {{-2, -2}, {-2, 1}, {3, -3}, {1, 1}, {0, -2}};

    // distance of cell to itself should be 0
    for (size_t i = 0; i < cells.size(); ++i) {
        REQUIRE(board.distance(cells[i], cells[i]) == 0);
    }

    REQUIRE(board.distance(cells[0], cells[1]) == 3);
    REQUIRE(board.distance(cells[0], cells[2]) == 5);
    REQUIRE(board.distance(cells[0], cells[3]) == 6);
    REQUIRE(board.distance(cells[0], cells[4]) == 2);

    REQUIRE(board.distance(cells[1], cells[0]) == 3);
    REQUIRE(board.distance(cells[1], cells[2]) == 5);
    REQUIRE(board.distance(cells[1], cells[3]) == 3);
    REQUIRE(board.distance(cells[1], cells[4]) == 3);

    REQUIRE(board.distance(cells[2], cells[0]) == 5);
    REQUIRE(board.distance(cells[2], cells[1]) == 5);
    REQUIRE(board.distance(cells[2], cells[3]) == 4);
    REQUIRE(board.distance(cells[2], cells[4]) == 3);

    REQUIRE(board.distance(cells[3], cells[0]) == 6);
    REQUIRE(board.distance(cells[3], cells[1]) == 3);
    REQUIRE(board.distance(cells[3], cells[2]) == 4);
    REQUIRE(board.distance(cells[3], cells[4]) == 4);

    REQUIRE(board.distance(cells[4], cells[0]) == 2);
    REQUIRE(board.distance(cells[4], cells[1]) == 3);
    REQUIRE(board.distance(cells[4], cells[2]) == 3);
    REQUIRE(board.distance(cells[4], cells[3]) == 4);
}

TEST_CASE("HexBoard::create_board r = 0::correct number of cells created", "[weight=1][part=hexboard]") {
    HexBoard board(0);
    REQUIRE(board.cells.size() == 1);
}

TEST_CASE("HexBoard::create_board r = 0::correct base board image", "[weight=1][part=hexboard]") {
    HexBoard board(0);
    PNG output = DrawMaze::draw_maze(board, 20, "none");
    if (writeoutputs) {
        output.writeToFile("output_images/board_r0.png");
    }
    PNG expected;
    expected.readFromFile("soln_images/board_r0.png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::create_board r = 1::correct number of cells created", "[weight=1][part=hexboard]") {
    HexBoard board(1);
    REQUIRE(board.cells.size() == 7);
}

TEST_CASE("HexBoard::create_board r = 1::correct base board image", "[weight=1][part=hexboard]") {
    HexBoard board(1);
    PNG output = DrawMaze::draw_maze(board, 20, "none");
    if (writeoutputs) {
        output.writeToFile("output_images/board_r1.png");
    }
    PNG expected;
    expected.readFromFile("soln_images/board_r1.png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::create_board r = 4::correct number of cells created", "[weight=1][part=hexboard]") {
    HexBoard board(4);
    REQUIRE(board.cells.size() == 61);
}

TEST_CASE("HexBoard::create_board r = 4::correct base board image", "[weight=1][part=hexboard]") {
    HexBoard board(4);
    PNG output = DrawMaze::draw_maze(board, 20, "none");
    if (writeoutputs) {
        output.writeToFile("output_images/board_r4.png");
    }
    PNG expected;
    expected.readFromFile("soln_images/board_r4.png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::configure_cells::is_edge", "[weight=1][part=hexboard]") {
    HexBoard board(2);
    REQUIRE(board.is_edge({0, -2}));
    REQUIRE(board.is_edge({1, -2}));
    REQUIRE(board.is_edge({2, -2}));
    REQUIRE(board.is_edge({2, -1}));
    REQUIRE(board.is_edge({2, 0}));
    REQUIRE(board.is_edge({1, 1}));
    REQUIRE(board.is_edge({0, 2}));
    REQUIRE(board.is_edge({-1, 2}));
    REQUIRE(board.is_edge({-2, 2}));
    REQUIRE(board.is_edge({-2, 1}));
    REQUIRE(board.is_edge({-2, 0}));
    REQUIRE(board.is_edge({-1, -1}));
}

TEST_CASE("HexBoard::link", "[weight=1][part=hexboard]") {
    HexBoard board(2);
    pair<int, int> coord1(1, 0);
    pair<int, int> coord2(2, -1);
    board.link(coord1, coord2);

    for (int i = 0; i < 6; i++) {
        if (i == NE) {
            REQUIRE_FALSE(board.cells[coord1]->walls[i]);
            REQUIRE(board.cells[coord2]->walls[i]);
        } else if (i == SW) {
            REQUIRE_FALSE(board.cells[coord2]->walls[i]);
            REQUIRE(board.cells[coord1]->walls[i]);
        } else {
            REQUIRE(board.cells[coord1]->walls[i]);
            REQUIRE(board.cells[coord2]->walls[i]);
        }
    }
}

TEST_CASE("HexBoard::generate_maze::correct walls removed", "[weight=1][part=hexboard]") {
    int seed = 7;
    HexBoard board(4);
    board.generate_maze({0, 0}, 0.5, seed);
    PNG output = DrawMaze::draw_maze(board, 20, "none");
    if (writeoutputs) {
        output.writeToFile("output_images/maze_r4.png");
    }
    PNG expected;
    expected.readFromFile("soln_images/maze_r4.png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::generate_maze::correct longest path length stored", "[weight=1][part=hexboard]") {
    int seed = 10;
    HexBoard board(4);
    board.generate_maze({0, 0}, 0.5, seed);
    REQUIRE(board.longest_path_dist == 46);
}

TEST_CASE("HexBoard::generate_maze::correct path distance from start for each cell", "[weight=1][part=hexboard]") {
    int seed = 221;
    HexBoard board(4);
    board.generate_maze({0, 0}, 0.5, seed);
    PNG output = DrawMaze::draw_maze(board, 20, "path");
    if (writeoutputs) {
        output.writeToFile("output_images/maze_colour_r4.png");
    }
    PNG expected;
    expected.readFromFile("soln_images/maze_colour_r4.png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::generate_maze::correct path distance from start for each cell, different start", "[weight=1][part=hexboard]") {
    int seed = 221;
    HexBoard board(4);
    board.generate_maze({3, -2}, 0.5, seed);
    PNG output = DrawMaze::draw_maze(board, 20, "path");
    if (writeoutputs) {
        output.writeToFile("output_images/maze_colour_r4_(3,-2).png");
    }
    PNG expected;
    expected.readFromFile("soln_images/maze_colour_r4_(3,-2).png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::solve_maze::correct solution length", "[weight=1][part=hexboard]") {
    int seed = 10;
    HexBoard board(4);
    board.generate_maze({0, 0}, 0.5, seed);
    board.solve_maze();
    REQUIRE(board.solution.size() == 47);
}

TEST_CASE("HexBoard::solve_maze::correct solution length, different start", "[weight=1][part=hexboard]") {
    int seed = 10;
    HexBoard board(4);
    board.generate_maze({4, 0}, 0.5, seed);
    board.solve_maze();
    REQUIRE(board.solution.size() == 39);
}

TEST_CASE("HexBoard::solve_maze::correct solution vector", "[weight=1][part=hexboard]") {
    int seed = 42;
    HexBoard board(4);
    board.generate_maze({0, 0}, 1, seed);
    board.solve_maze();
    vector<pair<int, int>> soln_path = {{0, 0}, {0, -1}, {1, -2}, {1, -3}, {2, -4}, {2, -3}, {3, -4}, {3, -3}, {4, -4}, {4, -3}, {3, -2}, {2, -1}, {1, -1}, {1, 0}, {1, 1}, {1, 2}, {2, 2}, {1, 3}, {0, 3}, {0, 2}, {0, 1}, {-1, 2}, {-1, 3}, {-2, 4}, {-2, 3}, {-2, 2}, {-3, 2}, {-2, 1}, {-1, 0}, {-2, 0}, {-3, 0}, {-4, 1}, {-4, 0}, {-3, -1}, {-2, -1}, {-1, -1}, {-1, -2}, {0, -2}, {0, -3}, {0, -4}, {-1, -3}, {-2, -2}};
    REQUIRE(board.solution == soln_path);
}

TEST_CASE("HexBoard::solve_maze::correct solution vector, different start", "[weight=1][part=hexboard]") {
    int seed = 42;
    HexBoard board(4);
    board.generate_maze({-2, 2}, 1, seed);
    board.solve_maze();
    vector<pair<int, int>> soln_path = {{-2, 2}, {-2, 1}, {-1, 0}, {-1, -1}, {0, -2}, {1, -3}, {2, -4}, {2, -3}, {3, -4}, {3, -3}, {4, -4}, {4, -3}, {3, -2}, {2, -2}, {1, -1}, {1, -2}, {0, -1}, {0, 0}, {1, 0}, {2, 0}, {3, 0}, {2, 1}, {3, 1}, {2, 2}, {1, 2}, {1, 3}, {0, 3}, {0, 4}, {-1, 4}, {-2, 4}, {-2, 3}, {-3, 4}, {-3, 3}, {-4, 3}, {-3, 2}, {-3, 1}, {-3, 0}, {-4, 0}, {-3, -1}, {-2, -1}, {-1, -2}, {0, -3}, {1, -4}, {0, -4}, {-1, -3}, {-2, -2}};
    REQUIRE(board.solution == soln_path);
}

TEST_CASE("HexBoard::solve_maze::correct solution image", "[weight=1][part=hexboard]") {
    int seed = 99;
    HexBoard board(4);
    board.generate_maze({0, 0}, 0.5, seed);
    board.solve_maze();
    PNG output = DrawMaze::draw_solution(board, 20, "none");
    if (writeoutputs) {
        output.writeToFile("output_images/maze_sol_r4.png");
    }
    PNG expected;
    expected.readFromFile("soln_images/maze_sol_r4.png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::solve_maze::correct solution image, different start", "[weight=1][part=hexboard]") {
    int seed = 99;
    HexBoard board(4);
    board.generate_maze({2, -3}, 0.5, seed);
    board.solve_maze();
    PNG output = DrawMaze::draw_solution(board, 20, "none");
    if (writeoutputs) {
        output.writeToFile("output_images/maze_sol_r4_(2,-3).png");
    }
    PNG expected;
    expected.readFromFile("soln_images/maze_sol_r4_(2,-3).png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::solve_maze::correct solution image with path colour", "[weight=1][part=hexboard]") {
    int seed = 1337;
    HexBoard board(4);
    board.generate_maze({0, 0}, 0.5, seed);
    board.solve_maze();
    PNG output = DrawMaze::draw_solution(board, 20, "path");
    if (writeoutputs) {
        output.writeToFile("output_images/maze_sol_colour_r4.png");
    }
    PNG expected;
    expected.readFromFile("soln_images/maze_sol_colour_r4.png");
    REQUIRE(output == expected);
}

TEST_CASE("HexBoard::solve_maze::correct solution image with path colour, different start", "[weight=1][part=hexboard]") {
    int seed = 1337;
    HexBoard board(4);
    board.generate_maze({-2, 1}, 0.5, seed);
    board.solve_maze();
    PNG output = DrawMaze::draw_solution(board, 20, "path");
    if (writeoutputs) {
        output.writeToFile("output_images/maze_sol_colour_r4_(-2,1).png");
    }
    PNG expected;
    expected.readFromFile("soln_images/maze_sol_colour_r4_(-2,1).png");
    REQUIRE(output == expected);
}