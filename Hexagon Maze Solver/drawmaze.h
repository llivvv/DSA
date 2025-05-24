#ifndef _DRAWMAZE_H_
#define _DRAWMAZE_H_

#include <algorithm>
#include <cmath>
#include <map>
#include <vector>

#include "hexboard.h"
#include "cs221util/PNG.h"

using std::pair;
using std::vector;
using namespace cs221util;

struct MazeParams {
    double a_size;
    double b_size;
    double height;
};

struct CellParams {
    double cx;
    double cy;
    int x_fw;
    int x_nw;
    int x_ne;
    int x_fe;
    int y_n;
    int y_m;
    int y_s;
};

namespace DrawMaze {
    // Helper functions
    MazeParams get_draw_params(int size);
    CellParams get_cell_params(int size, int maze_radius, MazeParams maze_params, HexCell* cell);

    /**
     * Draws on a PNG, the given maze considering the colours, neighbours and links of all cells.
     * @param maze The maze to draw
     * @param size The radius of a hex cell
     * @param colour_mode Colour mode, one of:
     *                    "none" for maze with start and end coloured only, or
     *                    "path" for maze colouring each cell by path distance from start
     * @return A PNG output of the drawn maze
     */
    PNG draw_maze(const HexBoard& maze, int size, string colour_mode);
    /**
     * Draws the solution path on top of the output from draw_maze
     * @param maze The maze to draw
     * @param size The radius of a hex cell
     * @param colour_mode Colour mode, one of:
     *                    "none" for solution with start and end coloured only, or
     *                    "path" for solution colouring each cell by path distance from start
     * @return A PNG output of the drawn maze
     */
    PNG draw_solution(const HexBoard& maze, int size, string colour_mode);

    /**
     * Converts axial coorindates (q,r) to offset coorindates (x,y)
     * @param axial_coord Input coorindates in axial represented by a pair (q,r)
     * @return Converted offset coorindates represented by a pair (x,y)
     */
    pair<int, int> axial_to_offset(pair<int, int> axial_coord);

    /**
     * @param n number of colours in the spectrum
     * @return A colour spectrum of size n from pure red to pure blue.
     */
    vector<RGBAPixel> spectrum(int n);

    /**
     * Draws a line from p1 to p2 with specified colour
     * @param img The image to draw on.
     * @param p1 A pair {x1,y1} specifying the coordinates of the first point.
     * @param p2 A pair {x2,y2} specifying the coordinates of the second point.
     * @param colour An RGBAPixel specifying the colour of the line.
     * @return All the points on the line
     */
    vector<pair<int, int>> draw_line(PNG& img, const pair<int, int>& p1, const pair<int, int>& p2, const RGBAPixel& colour);

    /**
     * Draws a hexagon with specified vertices.
     * @param img The image to draw on.
     * @param cell_params A CellParams object from which to calculate the vertices.
     * @param edge_colour An RGBAPixel specifying the colour of the edges.
     * @param fill_colour An RGBAPixel specifying the colour of the inner region.
     */
    void draw_hexagon(PNG& img, const CellParams& cell_params, const RGBAPixel& edge_colour, const RGBAPixel& fill_colour);
};

#endif