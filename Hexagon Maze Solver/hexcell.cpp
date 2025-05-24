#include "hexcell.h"

/**
 * HexCell constructor. Initializes all walls to be true and path_dist_from_start to 0.
 * @param QR Axial coordinates of the hex cell
 */
HexCell::HexCell(std::pair<int, int> QR) {
    /**
     * @todo Your code here!
     */
    qr = QR;

    for (int i = 0; i < 6; i++) {
        walls[i] = true;
    }
    path_dist_from_start = 0;
}

/**
 * Gets the side of a neighbour. Each side is represented with an int in [0,5].
 * See above for the definition of the sides
 * @param cell A neighbouring HexCell
 * @return The side of cell
 * REQUIRE: cell is a neighbour of this cell
 */
int HexCell::get_neighbour_side(const HexCell* cell) const {
    /**
     * @todo Your code here! You will need to replace the following line.
     */
    int q = qr.first; 
    int r = qr.second;
    int q2 = cell->qr.first; 
    int r2 = cell->qr.second; 

    if (q == q2) {
        if (r > r2) return 0; 
        return 3;
    }
    if (r == r2) {
        if (q < q2) return 2; 
        return 5; 
    }
    if (q < q2) return 1; 
    return 4; 



    ////
    // if (r == cell->qr.second) {
    //     if (q < cell->qr.first) {
    //         return 2;
    //     }
    //     return 5;
    // }
    // if (q == cell->qr.first) {
    //     if (r < cell->qr.second) {
    //         return 3;
    //     }
    //     return 0;
    // }
    // if (q < cell->qr.first) {
    //     return 1;
    // }
    // return 4;
}