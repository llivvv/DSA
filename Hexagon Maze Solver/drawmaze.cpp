#include "drawmaze.h"

#include <cmath>
#include <iostream>

#include "queue.h"

using std::pair;
using std::vector;

namespace DrawMaze {
    MazeParams get_draw_params(int size) {
        double a_size = size / 2.0;
        double height = size * sqrt(3);
        double b_size = height / 2.0;
        return {a_size, b_size, height};
    }

    CellParams get_cell_params(int size, int maze_radius, MazeParams maze_params, HexCell* cell) {
        pair<int, int> offset_coords = axial_to_offset(cell->qr);
        int x = offset_coords.first;
        int y = offset_coords.second;

        double cx = size + 3 * (x + maze_radius + 1) * maze_params.a_size;
        double cy = maze_params.b_size + (y + maze_radius + 1) * maze_params.height;

        if (x % 2 != 0) {
            cy += maze_params.b_size;
        }

        // f/n = far/near
        // n/s/e/w = north/south/east/west
        int x_fw = (cx - size);
        int x_nw = (cx - maze_params.a_size);
        int x_ne = (cx + maze_params.a_size);
        int x_fe = (cx + size);

        // m = middle
        int y_n = (cy - maze_params.b_size);
        int y_m = cy;
        int y_s = (cy + maze_params.b_size);

        return {cx, cy, x_fw, x_nw, x_ne, x_fe, y_n, y_m, y_s};
    }

    PNG draw_maze(const HexBoard& maze, int size, string colour_mode) {
        if (colour_mode != "none" && colour_mode != "path") {
            std::cerr << "Colour mode must be one of \"none\" or \"path\"." << std::endl;
            return PNG();
        }

        MazeParams params = get_draw_params(size);

        int rows = 2 * maze.radius + 3;
        int columns = 2 * maze.radius + 3;

        int img_width = 3 * params.a_size * columns + params.a_size + 0.5;
        int img_height = params.height * rows + params.b_size + 0.5;

        RGBAPixel cell_colour;
        RGBAPixel wall_colour(0, 0, 0);

        PNG img(img_width, img_height);

        vector<RGBAPixel> colours = spectrum(maze.longest_path_dist + 1);

        // draw backgrounds
        for (const auto& kv : maze.cells) {
            HexCell* cell = kv.second;
            CellParams cell_params = get_cell_params(size, maze.radius, params, cell);

            if (colour_mode == "none" && cell->qr != maze.start_coord) cell_colour = RGBAPixel(255, 255, 255);
            else cell_colour = colours[cell->path_dist_from_start];
            draw_hexagon(img, cell_params, cell_colour, cell_colour);
        }

        if (colour_mode == "none") {
            RGBAPixel start_colour = RGBAPixel(255, 0, 0);
            RGBAPixel end_colour = RGBAPixel(255, 0, 255);

            HexCell* start_cell = maze.cells.at(maze.start_coord);
            CellParams cell_params = get_cell_params(size, maze.radius, params, start_cell);
            draw_hexagon(img, cell_params, start_colour, start_colour);

            HexCell* end_cell = maze.cells.at(maze.end_coord);
            cell_params = get_cell_params(size, maze.radius, params, end_cell);
            draw_hexagon(img, cell_params, end_colour, end_colour);
        }

        // draw walls
        for (const auto& kv : maze.cells) {
            HexCell* cell = kv.second;
            CellParams cell_params = get_cell_params(size, maze.radius, params, cell);

            if (cell->walls[N]) draw_line(img, {cell_params.x_nw, cell_params.y_n}, {cell_params.x_ne, cell_params.y_n}, wall_colour);

            if (cell->walls[NE]) draw_line(img, {cell_params.x_ne, cell_params.y_n}, {cell_params.x_fe, cell_params.y_m}, wall_colour);

            if (cell->walls[SE]) draw_line(img, {cell_params.x_fe, cell_params.y_m}, {cell_params.x_ne, cell_params.y_s}, wall_colour);

            if (cell->walls[S]) draw_line(img, {cell_params.x_ne, cell_params.y_s}, {cell_params.x_nw, cell_params.y_s}, wall_colour);

            if (cell->walls[SW]) draw_line(img, {cell_params.x_fw, cell_params.y_m}, {cell_params.x_nw, cell_params.y_s}, wall_colour);

            if (cell->walls[NW]) draw_line(img, {cell_params.x_fw, cell_params.y_m}, {cell_params.x_nw, cell_params.y_n}, wall_colour);
        }

        return img;
    }

    PNG draw_solution(const HexBoard& maze, int size, string colour_mode) {
        PNG img = draw_maze(maze, size, colour_mode);

        MazeParams params = get_draw_params(size);

        RGBAPixel sol_colour = RGBAPixel(0, 0, 0);

        // draw out the solution path
        int sol_length = maze.solution.size();

        for (int i = 0; i < sol_length - 1; i++) {
            // get centre coorindates of cell
            HexCell* cell = maze.cells.at(maze.solution[i]);
            CellParams cell_params = get_cell_params(size, maze.radius, params, cell);

            // get centre coorindates of next cell
            HexCell* next_cell = maze.cells.at(maze.solution[i + 1]);
            CellParams next_cell_params = get_cell_params(size, maze.radius, params, next_cell);

            // draw line to connect the centre of cell with next cell
            draw_line(img, {cell_params.cx, cell_params.cy}, {next_cell_params.cx, next_cell_params.cy}, sol_colour);
        }
        return img;
    }

    pair<int, int> axial_to_offset(pair<int, int> axial_coord) {
        int q = axial_coord.first;
        int r = axial_coord.second;

        int x = q;
        int y = r + (q - (q & 1)) / 2;
        return {x, y};
    }

    vector<RGBAPixel> spectrum(int n) {
        vector<RGBAPixel> ret(n);

        for (int i = 0; i < n; ++i) {
            ret[i].from_hsl(330.0 * i / n, 1.0, 0.5);
        }

        return ret;
    }

    vector<pair<int, int>> draw_line(PNG& img, const pair<int, int>& p1, const pair<int, int>& p2, const RGBAPixel& colour) {
        vector<pair<int, int>> coords;
        int x1 = p1.first;
        int x2 = p2.first;
        int y1 = p1.second;
        int y2 = p2.second;

        // reorder so that x1 <= x2
        if (x1 > x2) {
            swap(x1, x2);
            swap(y1, y2);
        }

        int dx = abs(x2 - x1);
        int dy = abs(y2 - y1);
        int sx = x1 < x2 ? 1 : -1;
        int sy = y1 < y2 ? 1 : -1;
        int err = dx - dy;

        while (true) {
            // Colour the current pixel
            *img.getPixel(x1, y1) = colour;
            coords.push_back({x1, y1});

            // If the end of the line has been reached, break out of the loop
            if (x1 == x2 && y1 == y2)
                break;

            int e2 = 2 * err;

            if (e2 > -dy) {
                err -= dy;
                x1 += sx;
            }

            if (e2 < dx) {
                err += dx;
                y1 += sy;
            }
        }

        return coords;
    }

    void draw_hexagon(PNG& img, const CellParams& cell_params, const RGBAPixel& edge_colour, const RGBAPixel& fill_colour) {
        // get the points on the edges of the hexagon
        set<pair<int, int>> edgePoints;

        for (const auto& point : draw_line(img, {cell_params.x_fw, cell_params.y_m}, {cell_params.x_nw, cell_params.y_n}, edge_colour)) edgePoints.insert(point);

        for (const auto& point : draw_line(img, {cell_params.x_nw, cell_params.y_n}, {cell_params.x_ne, cell_params.y_n}, edge_colour)) edgePoints.insert(point);

        for (const auto& point : draw_line(img, {cell_params.x_ne, cell_params.y_n}, {cell_params.x_fe, cell_params.y_m}, edge_colour)) edgePoints.insert(point);

        for (const auto& point : draw_line(img, {cell_params.x_fe, cell_params.y_m}, {cell_params.x_ne, cell_params.y_s}, edge_colour)) edgePoints.insert(point);

        for (const auto& point : draw_line(img, {cell_params.x_ne, cell_params.y_s}, {cell_params.x_nw, cell_params.y_s}, edge_colour)) edgePoints.insert(point);

        for (const auto& point : draw_line(img, {cell_params.x_nw, cell_params.y_s}, {cell_params.x_fw, cell_params.y_m}, edge_colour)) edgePoints.insert(point);

        pair<int, int> midPoint = {cell_params.cx, cell_params.cy};
        pair<int, int> directions[4] = {{1, 0}, {-1, 0}, {0, -1}, {0, 1}};

        // keeps track of visited pixels
        set<pair<int, int>> visited;
        Queue<pair<int, int>> q;
        visited.insert(midPoint);
        q.enqueue(midPoint);

        // BFS
        while (!q.is_empty()) {
            pair<int, int> curr = q.dequeue();
            *img.getPixel(curr.first, curr.second) = fill_colour;

            for (auto& dir : directions) {
                pair<int, int> neigh = {curr.first + dir.first, curr.second + dir.second};

                // only explore a neighbour if it is not visited and not an edge
                if (visited.find(neigh) == visited.end() && edgePoints.find(neigh) == edgePoints.end()) {
                    visited.insert(neigh);
                    q.enqueue(neigh);
                }
            }
        }
    }
}