#include "qtvar.h"

bool qtvar::prunable(Node* node, const int tol) const {

    /* Your code here! */
    return (node->var < tol);
}
