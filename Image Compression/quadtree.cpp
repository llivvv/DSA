
/**
 *
 * quadtree (pa3)
 * quadtree.cpp
 * This file will be used for grading.
 *
 */

#include "quadtree.h"
using namespace std;

// Node constructor, given.
quadtree::Node::Node(pair<int, int> ul, int d, RGBAPixel a, double v)
    : upLeft(ul), dim(d), avg(a), var(v), NW(nullptr), NE(nullptr), SE(nullptr), SW(nullptr) {}

// quadtree destructor, given.
quadtree::~quadtree() {
    clear();
}
// quadtree copy constructor, given.
quadtree::quadtree(const quadtree& other) {
    copy(other);
}
// quadtree assignment operator, given.
quadtree& quadtree::operator=(const quadtree& rhs) {
    if (this != &rhs) {
        clear();
        copy(rhs);
    }
    return *this;
}

quadtree::quadtree(PNG& imIn) {


    edge = min(imIn.width(), imIn.height());
    int dim = log2(edge);
    edge = pow(2, dim);

    stats s(imIn);
    root = buildTree(s, {0, 0}, dim);
}

quadtree::Node* quadtree::buildTree(stats& s, pair<int, int> ul, int dim) {
    if (dim == 0) {
        RGBAPixel a = s.getAvg(ul, dim);
        double varC = s.getVar(ul, dim);
        Node* leaf = new Node(ul, dim, a, varC);
        return leaf; 
    }
    
    RGBAPixel a = s.getAvg(ul, dim);
    double varC = s.getVar(ul, dim);
    Node* subTree = new Node(ul, dim, a, varC);

    dim--; 
    int halfW = pow(2, dim);

    subTree->NW = buildTree(s, {ul.first, ul.second}, dim);
    subTree->NE = buildTree(s, {ul.first + halfW, ul.second}, dim);
    subTree->SW = buildTree(s, {ul.first, ul.second + halfW}, dim);
    subTree->SE = buildTree(s, {ul.first + halfW, ul.second + halfW}, dim);
    return subTree;
}

PNG quadtree::render() const {
    PNG png = PNG(edge, edge);
 
    render(root, png);

    return png; 
}
// private render helper
void quadtree::render(Node* subtree, PNG & png) const {
    if (subtree->NW == NULL) {
        int ogY = subtree->upLeft.second; 
        int ogX = subtree->upLeft.first; 

        int width_ = pow(2, subtree->dim);

        for (int y = ogY; y < width_ + ogY; y++) {
            for (int x = ogX; x < width_ + ogX; x++) {

                RGBAPixel* imgPixel = png.getPixel(x, y);

                (*imgPixel)=subtree->avg;

            } 
        }
        return; 
    } 
    render(subtree->NW, png);
    render(subtree->NE, png);
    render(subtree->SW, png);
    render(subtree->SE, png);


}

int quadtree::idealPrune(const int leaves) const {
    int min = 0; 
    int max = 3*(pow(255, 2));

    int rsf = 0; 

    while (min <= max) {
        int mid = (min + max) / 2;
        int pruneL = pruneSize(mid);

        if (pruneL <= leaves) {
            rsf = mid; 
            max = mid - 1; 
        } else {
            min = mid + 1;
        }
    } 
    return rsf; 
}

int quadtree::pruneSize(const int tol) const {
    /* Your code here! */
    int currNum = pow(2, 2*(root->dim)); // the current number of leaves in tree
    return pruneSize(root, tol, currNum);
}

int quadtree::pruneSize(Node* subtree, const int tol, int & currNum) const {

    if (subtree == nullptr) return currNum; 

    if (subtree->NW == NULL) return currNum;  
    
    if (prunable(subtree, tol)) {

        int treeLeaves = pow(2, 2*(subtree->dim)); 
        treeLeaves--; // eg. tree with 4 leaves becomes 1 leaf (just the root)
        currNum -= treeLeaves; 
        return currNum;  
    }

    pruneSize(subtree->NW, tol, currNum);
    pruneSize(subtree->NE, tol, currNum);
    pruneSize(subtree->SW, tol, currNum);
    pruneSize(subtree->SE, tol, currNum);

    return currNum; 
}


void quadtree::prune(const int tol) {
    /* Your code here! */

    prune(root, tol);

}

void quadtree::prune(Node*& subtree, const int tol) {

    if (subtree->NW == NULL) return; 

    if (prunable(subtree, tol)) {

        clear(subtree->NW);
        clear(subtree->NE);
        clear(subtree->SW);
        clear(subtree->SE);

        return; 
    }

    prune(subtree->NW, tol);
    prune(subtree->NE, tol);
    prune(subtree->SW, tol);
    prune(subtree->SE, tol);

    return; 

}

void quadtree::clear() {

    clear(root);
}

void quadtree::clear(Node* & subtree) {

    if (subtree->NW == NULL) {
        delete subtree; 
        subtree = NULL; 
        return; 
    }

    clear(subtree->NW);
    clear(subtree->NE);
    clear(subtree->SW);
    clear(subtree->SE);

    delete subtree; 
    subtree = NULL; 
    return; 
}

void quadtree::copy(const quadtree& orig) {
    edge = pow(2, orig.root->dim);

    Node* origR = orig.root;

    root = new Node(origR->upLeft, origR->dim, origR->avg, origR->var);

    if (origR->NW == NULL) return;
    
    root->NW = copy(origR->NW);
    root->SW = copy(origR->SW);
    root->NE = copy(origR->NE);
    root->SE = copy(origR->SE);
}

quadtree::Node* quadtree::copy(Node* subtree) {

    Node* node = new Node(subtree->upLeft, subtree->dim, subtree->avg, subtree->var);

    if (subtree->NW == NULL) return node; 

    node->NW = copy(subtree->NW);
    node->SW = copy(subtree->SW);
    node->SE = copy(subtree->SE);
    node->NE = copy(subtree->NE);

    return node;
}