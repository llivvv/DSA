#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain()
{
    clear(); 
/* YOUR CODE HERE */
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata)
{
    Node* after = walk(head_, length_);
    Node* newNode = new Node(ndata);
    newNode->next = after->next; 
    after->next = newNode;
    length_++; 
/* YOUR CODE HERE */
} 


/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j)
{
    // idea:
    // need 4 cases: 
    // if (j-i) is 0 or list only has 1 element, or no elements
    // if (j-1) is 1 (nothing between them)
    // if (j-i)-1 is 1 (aka only 1 node between them)
    // else 
    // nope
    // only need 3 cases: no swap, beside and not beside

    if (j - 1 == 0 || length_ <= 1) return;

    // make sure that i < j
    if (j < i) {
        swap(j, i);
        return; 
    } 
    Node* prior = walk(head_, i - 1);
    Node* first = walk(head_, i);
    Node* second = walk(head_, j);

    if (j - i == 1) {
        // no nodes between i and j
        // prior is the node before i
        prior->next = second; 
        first->next = second->next; 
        second->next = first; 
    } else {
        Node* secondPrior = walk(head_, j-1);
        prior->next = second; 
        second = second->next;
        prior->next->next = first->next;
        first->next = second; 
        secondPrior->next = first; 
    } 
} 

/**
 * Reverses the chain
 */
void Chain::reverse()
{ 
    if (length_ <= 1) return;
    int front = 1; 
    int back = length_; 
    while (front < back) {
        swap(front, back);
        front++; 
        back--;
    }
}

/*
* Modifies the current chain by "rotating" every k nodes by one position.
* In every k node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k)
{ 
    //nothing to rotate
    if (length_ < k || k == 1) return;
    
    //initializing pointers
    Node* prior = head_; 
    Node* first = head_->next; 
    Node* kth = walk(head_, k);

    int subLength = length_; 

    while (true) {

        // rotating 1 set of k nodes
        prior->next = first->next;
        first->next = kth->next; 
        kth->next = first; 

        //ensure that there is another multiple of k to rotate
        subLength = subLength - k; 
        if (subLength < k) return; 

        //resetting pointers for next rotation
        prior = first; 
        first = first->next; 
        
        //setting kth pointer to the position of next multiple of k
        for (int step = 0; step <= k; step++) {
            kth = kth->next; 
        }
    }
/* YOUR CODE HERE */
}

/**
 * Destroys ALL dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear()
{
    Node* temp;  
    while (length_ > 0) {
        temp = head_; 
        head_ = head_->next; 
        delete temp;
        length_--;

    }
    delete head_;
    head_ = NULL;
/* YOUR CODE HERE */
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */ 
void Chain::copy(Chain const &other)
{ 
    width_ = other.width_;
    height_ = other.height_; 
    head_ = new Node();
    head_->next = head_; 

    length_ = 0; 

    if (other.length_ == 0) return; 
    Node* temp = other.head_->next;  
    while (length_ < other.length_) {
        insertBack(temp->data);
        temp = temp->next; 
    }
/* YOUR CODE HERE */
}
