#include "BSTSet.h"
#include "MyStack.h"
#include <iostream>
#include <vector>
using namespace std;

BSTSet::BSTSet() // time: O(1), space: O(1)
{
	root = NULL;
}

BSTSet::BSTSet(const std::vector<int>& input) // time: O(n), space: O(n)
{
	if(input.size() == 0) {
		root = NULL;
	} else {
		root = new TNode(input.at(0), NULL, NULL);
		TNode* current = root;

		for(int i = 0; i < input.size(); i++) { // iterate through vector
			while(1) { // go through tree, add new nodes
				if(input.at(i) == current->element) { // if node already exists
					break;
				} else if(input.at(i) < current->element) {
					if(current->left == NULL) {
						current->left = new TNode(input.at(i), NULL, NULL);
						break;
					}
					current = current->left;
				} else {
					if(current->right == NULL) {
						current->right = new TNode(input.at(i), NULL, NULL);
						break;
					}
					current = current->right;
				}
			}
			current = root;
		}
	}
}

BSTSet::~BSTSet() // time: O(n), space: O(1)
{
	deleteTree(root);
}

void BSTSet::deleteTree(TNode* t) { // time: O(n), space: O(1)
	if(t != NULL) {
		deleteTree(t->left);
		deleteTree(t->right);
		remove(t->element);
	}
}

bool BSTSet::isIn(int v) // time: O(n), space: O(1)
{
	TNode* current = root;

	while(current != NULL) { // go through tree, look for v
		if(current->element == v) {
			return true;
		} else if(current->element > v) {
			current = current->left;
		} else {
			current = current->right;
		}
	}

    return false; // return false if not found
}

void BSTSet::add(int v) // time: O(n), space: O(n)
{
	if(isIn(v)) { // if node already exists in tree
		return;
	}

	if(root == NULL) { // if tree is empty, set root
		root = new TNode(v, NULL, NULL);
		return;
	}

	TNode* current = root;

	while(1) { // go through tree, find place to add new node
		if(v < current->element) {
			if(current->left == NULL) {
				current->left = new TNode(v, NULL, NULL);
				break;
			}
			current = current->left;
		} else {
			if(current->right == NULL) {
				current->right = new TNode(v, NULL, NULL);
				break;
			}
			current = current->right;
		}
	}
}

bool BSTSet::remove(int v) // time: O(n), space: O(1)
{
	TNode* current = root;
	TNode* parent = NULL;

	if(!isIn(v)) { // if node does not exist in tree
		return false;
	}

	while(current->element != v) { // find node to be removed
		parent = current;
		if(v > current->element) {
			current = current->right;
		}
		else {
			current = current->left;
		}
	}

	if(current->left != NULL && current->right != NULL) { // if node has two children
		TNode* minParent = NULL;
		TNode* min = current->right;

		while(min->left != NULL) { // find minimum node in right tree
			minParent = min;
			min = min->left;
		}

		// remove min node from tree to be able to use its value for current node
		if(minParent == NULL) { // if min is current->right
			current->right = min->right;
		} else {
			minParent->left = min->right;
		}

		current->element = min->element; // set current element to min element
		delete min; // delete min node
		return true;
	} else { // if node has one or no children
		if(parent == NULL) { // if current is root
			root = (current->left != NULL) ? current->left : current->right; // set root to its min child
			delete current; // delete current node
			return true;
		} else {
			if(parent->right == current) { // if current node is the right child of its parent
				parent->right = (current->left != NULL) ? current->left : current->right; // set parent right to current min child
			} else { // if current node is the left child of its parent
				parent->left = (current->left != NULL) ? current->left : current->right; // set parent left to current min child
			}
			delete current; // delete current node
			return true;
		}
	}

	return false; // return false if something didn't work
}

void BSTSet::Union(const BSTSet& s) // time: O(n), space: O(1)
{
	if(s.root != NULL) {
		Union2(s.root); // go through each element of s tree
	}
}

void BSTSet::Union2(TNode* t) // time: O(n), space: O(1)
{
	if(t != NULL) {
		add(t->element); // add element to this if it is not already there
		Union2(t->left); // go through left subtree
		Union2(t->right); // go through right subtree
	}
}

void BSTSet::intersection(const BSTSet& s) // time: O(n), space: O(1)
{
	BSTSet tree = s;
	if(s.root == NULL) { // if s is empty, make this tree empty
		deleteTree(root);
	} else if(root == NULL) { // if this tree is empty, keep it that way
		return;
	}
	else {
		intersection2(s.root, *this); // go through s tree, compare against this tree
		intersection2(root, tree); // go through this tree, compare against s tree
	}
}

void BSTSet::intersection2(TNode* t, BSTSet& s) { // time: O(n), space: O(1)
	if(t != NULL) {
		intersection2(t->left, s); // go through left subtree
		intersection2(t->right, s); // go through right subtree
		if(!s.isIn(t->element)) { // if node element is not in the set it is being compared against
			remove(t->element); // remove element from this tree
		}
	}
}

void BSTSet::difference(const BSTSet& s) // time: O(n), space: O(1)
{
	if(s.root != NULL) {
		difference2(s.root); // go through each element of s tree
	}
}

void BSTSet::difference2(TNode* t) // time: O(n), space: O(1)
{
	if(t != NULL) {
		remove(t->element); // remove t element from this tree if it exists in the tree
		difference2(t->left); // go through left tree
		difference2(t->right); // go through right tree
	}
}

int BSTSet::size() // time: O(n), space: O(1)
{
	return size2(root); // go through each node of this tree
}

int BSTSet::size2(TNode* t) { // time: O(n), space: O(1)
	int count = 0;

	if(t != NULL) {
		count ++; // increase count by 1 (count node t)
		count += size2(t->left); // go through left subtree, adding to count each time
		count += size2(t->right); // go through right subtree, adding to count each time
	}

	return count;
}

int BSTSet::height() // time: O(n), space: O(1)
{
	if(root == NULL) { // if tree is empty, return -1
		return -1;
	}

    return height2(root); // go through each node of this tree
}

int BSTSet::height2(TNode* t) { // time: O(n), space: O(1)
	int countR = 0;
	int countL = 0;

	if(t->left != NULL) { // count layers of left subtree
		countL++;
		countL += height2(t->left);
	}
	if(t->right != NULL) { // count layers of right subtree
		countR++;
		countR += height2(t->right);
	}

	return max(countR, countL); // return the biggest number out of the left and right subtree layer count
}

// create and use class MyStack
void BSTSet::printNonRec() // time: O(n), space: O(1)
{
	MyStack* stack = new MyStack(size());
	TNode* current = root;

	while(current != NULL || stack->isEmpty() == false) { // only stop when current is NULL and stack is empty
		while(current != NULL) { // go through left side of tree, pushing elements to the stack
			stack->push(current->element);
			current = current->left;
		}

		int val = stack->pop(); // pop value from top of stack
		cout << val << ","; // print the value
		current = root; // reset current to root

		while(current != NULL) { // go through tree, look for node with element equal to val
			if(current->element > val) {
				current = current->left;
			} else if(current->element < val) {
				current = current->right;
			} else {
				break;
			}
		}

		current = current->right; // now go to right of val node to continue going through the tree
	}
}

// Do not modify following provided methods

void BSTSet::printBSTSet()
{
    if (root == NULL)
        std::cout << "";
    else {
        printBSTSet(root);
    }
}

void BSTSet::printBSTSet(TNode* t)
{
    if (t != NULL) {
        printBSTSet(t->left);
        std::cout << t->element << ",";
        printBSTSet(t->right);
    }
}
