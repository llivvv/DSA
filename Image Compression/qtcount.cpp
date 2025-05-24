#include "qtcount.h"

bool qtcount::prunable(Node* node, const int tol) const {
    // A node is pruned if all of the leaves in its subtree have 
    //colour-distance within tolerance of its average. 
    //Distances between colours are computed as the sum, 
    //over each colour channel, of the pixel value differences, squared.
    /* Your code here */ 

    // call a recursive function to get the leaves 
    // then return a vector so that can compare 


    RGBAPixel pixel = node->avg; 
    bool canPrune = true; 

    return prunable(node, tol, pixel, canPrune); 
} 

bool qtcount::prunable(Node* subtree, const int tol, RGBAPixel& pixel, bool& canPrune) const {

    if (!canPrune) return canPrune; 

    if (subtree->NW == NULL) {
        RGBAPixel* curr = &(subtree->avg);
        int diffR = pow(pixel.r - curr->r, 2);
        int diffG = pow(pixel.g - curr->g, 2);
        int diffB = pow(pixel.b - curr->b, 2);

        if (diffR + diffG + diffB > tol) {
            canPrune = false; 
            //return canPrune; 
        }
        return canPrune; 
    } 

    prunable(subtree->NW, tol, pixel, canPrune);
    prunable(subtree->NE, tol, pixel, canPrune);
    prunable(subtree->SW, tol, pixel, canPrune);
    prunable(subtree->SE, tol, pixel, canPrune);

    return canPrune; 

}
