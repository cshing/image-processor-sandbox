/**
 * File: chain.cpp
 * Description: student implementation for Chain functions
 *              for CPSC 221 2023S PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
 */

#include <math.h>

#include "chain.h"

/**
 * Constructs a Chain from an input image
 * Each Node will cover a nodedimension x nodedimension (in pixels)
 * region in the source image.
 * @param img the input image
 * @param nodedimension number of pixels along the width and height of the Block region
 * @pre input image's width and height are evenly divisible by nodedimension
 */
Chain::Chain(PNG& img, unsigned int nodedimension) {
	// complete your implementation below
	unsigned int width = img.width();
    unsigned int height = img.height();

	// checks @pre
	// if (width % nodedimension != 0 || height % nodedimension != 0) {
    //     cout << "WIDTH/HEIGHT ERROR" << endl;
	// 	return; 
    // }

	NW = NULL;
	SE = NULL;
	length_ = 0;
	
    for (unsigned int y = 0; y < height; y += nodedimension) {
        for (unsigned int x = 0; x < width; x += nodedimension) {
            Block block;
            block.Build(img, x, y, nodedimension);

			InsertBack(block);
		}
	}
}

/**
 * Renders the Chain's pixel data into an output PNG.
 * The PNG should be sized according to the number of nodes
 * specified by the cols parameter.
 * It is possible that there may not be enough nodes to fill
 * the bottom row of the PNG. If this happens, just render
 * as many nodes as there are, from left to right, and leave
 * the rest of the row as opaque white pixels.
 * @pre this Chain is not empty
 * @param cols the number of Blocks to use for the width of the image
 */
PNG Chain::Render(unsigned int cols) {
	// replace the line below with your implementation
	// return PNG();

	// checks @pre
	// if (IsEmpty()) {
    //     cout << "EMPTY CHAIN" << endl;
	// 	return PNG();
    // }
	unsigned int rows = ((length_ % cols != 0) ? (unsigned int) (length_ / cols + 1) : (unsigned int) (length_ / cols));
	
	unsigned int node_ = NW->data.Dimension();

	unsigned int width = node_ * cols;
	unsigned int height = node_ * rows;

	PNG png = PNG(width, height);
	Node* curr = NW;

	for (unsigned int y = 0; y < height; y += node_) {
		for (unsigned int x = 0; x < width; x += node_) {
			curr->data.Render(png, x, y);
			
			if (curr->next == NULL) {
				x = width;
			} else {
				curr = curr->next;
			}
		}
	}
	return png;
}

/**
 * Inserts a new Node containing ndata at the back of the Chain
 */
void Chain::InsertBack(const Block& ndata) {
	// complete your implementation below
	Node* new_node = new Node(ndata);
	
	if (NW == NULL) {
		NW = new_node;
	} else {
		SE->next = new_node;
		new_node->prev = SE;
	}
	SE = new_node;

	length_ += 1;
}

/**
 * Reverses the structure of the list. This must be done by
 * pointer assignments. You may not deallocate or allocate any Nodes.
 * Example:
 *	before:	NW -> A <-> B <-> C <-> D <-> E <-> F <-> G <-> H <- SE
 *  
 *  after:	NW -> H <-> G <-> F <-> E <-> D <-> C <-> B <-> A <- SE
 */
void Chain::Reverse() {
	// complete your implementation below
	if (NW == NULL || NW == SE) {
	        return;
    }

	for (Node* curr = NW; curr != NULL; curr = curr->prev) {
		Node* temp = curr->prev;
		curr->prev = curr->next;
		curr->next = temp;
	}
	
	Node* temp = NW;
	NW = SE;
	SE = temp;	
}

/**
 * Rearranges the Node structure and internal pixel data to be flipped over a vertical axis.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *  before, flipping with 3 columns (2 rows):
 *
 *		NW -> A> <-> B> <-> C> <->
 *            D> <-> E> <-> F> <- SE
 * 
 *  after, visualized with 3 columns (2 rows):
 * 
 *		NW -> <C <-> <B <-> <A <->
 *            <F <-> <E <-> <D <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
 */
void Chain::FlipHorizontal(unsigned int cols) {
	// complete your implementation below
/* 	Reverse();
	Node* new_SE = NW;
	unsigned int SE_index = length_ / 2;
	unsigned int NW_index = length_ / 2 + 1;

	while (SE_index > 1) {
		new_SE = new_SE->next;
		SE_index--;
	}
	Node* new_NW = new_SE->next;

	SE->next = NW;
	NW->prev = SE;	
	new_SE->next = NULL;
	new_NW->prev = NULL;
	SE = new_SE;
	NW = new_NW;
 */

	Reverse();
	unsigned int rows = length_ / cols;
	Node* curr = NW;
	Node* head;
	Node* tail = NULL;
	Node* temp;

	for (unsigned int i = 0; i < rows; i++) {
		head = curr;
		for (unsigned int j = 0; j < cols - 1; j++) {			
			if (j == 0 && i == rows - 1) {
				NW = curr;
			}

			curr = curr->next;
		}

		if (i == 0) {
			SE = curr;
		}

		temp = curr->next;
		curr->next = tail;
		if (tail) {
			tail->prev = curr;
		}
		tail = head;
		curr = temp;
	}

	head->prev = NULL;

	curr = NW;
	while (curr) {
		curr->data.FlipHorizontal();
		curr = curr->next;
	}
}

/**
 * Rearranges the Node structure and internal pixel data to be rotated counter-clockwise.
 * This must be done using pointer assignments.
 * You may not deallocate or allocate any Nodes.
 * Example, length = 6:
 *	before, rotating with 3 columns (2 rows):
 *            ^     ^     ^
 *      NW -> A <-> B <-> C <->
 *            ^     ^     ^
 *            D <-> E <-> F <- SE
 * 
 *  after, visualized with 2 columns (3 rows):
 * 
 *      NW -> <C <-> <F <->
 *            <B <-> <E <->
 *            <A <-> <D <- SE
 * 
 * @param cols number of Blocks to use for the initial width of the image
 * @pre length_ is divisible by cols (i.e. no "partially-filled" rows)
 */
void Chain::RotateCCW(unsigned int cols) {
	// complete your implementation below
	
	unsigned int rows = length_ / cols;
	vector<vector<Node*>> temp;
	temp.resize(rows, vector<Node*>(cols));
	Node* curr = NW;

	// placing nodes in vector
	for (unsigned int row = 0; row < rows; row++) {
		for (unsigned int col = 0; col < cols; col++) {
			temp[row][col] = curr;
			curr->data.RotateCCW();
			curr = curr->next;
		}
	}
	
	// vector<vector<Node*>> rotate;
	// rotate.resize(cols, vector<Node*>(rows));

	// // rotating vector
	// for (unsigned int row = 0; row < rows; row++) {
	// 	unsigned int i = cols - 1;
	// 	for (unsigned int col = 0; col < cols; col++) {
	// 		rotate[i--][col] = temp[col][row];
	// 	}
	// }
	
	// unsigned int temp1 = cols;
	// cols = rows;
	// rows = temp1;

	// cout << "AM I HERE PLS SAY YES 2" << endl;

	// NW = rotate[0][0];
	// SE = rotate[cols - 1][rows - 1];
	
	// curr = NW; 
	
	// // assigning pointers
	// for (unsigned int row = 0; row < rows; row++) {
	// 	for (unsigned int col = 0; col < cols; col++) {
	// 		curr = rotate[row][col];
	// 		cout << "AM I HERE PLS SAY YES 4" << endl;
	// 		if (col == cols - 1) {
	// 		cout << "AM I HERE PLS SAY YES 5" << endl;

	// 			if (row == rows - 1) {
	// 				curr->prev = rotate[row][col-1];
	// 				curr->next = NULL;
	// 			} else {
	// 				curr->next = rotate[row+1][0];
	// 				rotate[row+1][0]->prev = curr;
	// 			}
	// 		} else if (row == 0 && col == 0) {
	// 			curr->next = rotate[row][col+1];
	// 			curr->prev = NULL;
	// 		cout << "AM I HERE PLS SAY YES 6" << endl;

	// 		} else {
	// 			curr->next = rotate[row][col+1];
	// 			rotate[row][col+1]->prev = curr;
	// 		cout << "AM I HERE PLS SAY YES 7" << endl;

	// 		}
	// 	}
	// }

	
	for (unsigned int row = 0; row < rows; row++) {
		cout << "AM I HERE PLS SAY YES 2" << endl;
		for (unsigned int col = 0; col < cols; col++) {
			curr = temp[row][col]; // normal execution
			cout << "AM I HERE PLS SAY YES 3" << endl;
			if (row == 0) {
				if (col == cols - 1) {
					curr->prev = NULL;
					// curr->next = temp[row+1][col]; 
					// temp[row+1][col]->prev = curr;
					cout << "I AM 4.1" << endl;
				} else {
					cout << "AM I HERE PLS SAY YES 4" << endl;
					curr->prev = temp[rows-1][col+1];
					cout << "AM I HERE PLS SAY YES 4.2" << endl;
					temp[rows-1][col+1]->next = curr;
				}
				cout << "AM I HERE PLS SAY YES 4.3" << endl;
			} else if (row == rows - 1) {
				if (col == 0) {
					curr->prev = temp[row-1][col];
					temp[row-1][col]->next = curr;
					curr->next = NULL; // end
					cout << "AM I HERE PLS SAY YES 5" << endl;
				} else {
					curr->prev = temp[row-1][col];
					temp[row-1][col]->next = curr;
				}
			} else { // normal execution
				curr->prev = temp[row-1][col];
				temp[row-1][col]->next = curr;
				cout << "AM I HERE PLS SAY YES 7" << endl;
			}
		}
	}

	NW = temp[0][cols-1];
	SE = temp[rows-1][0];

	cout << "AM I HERE PLS SAY YES 8" << endl;
	
}

/**
 * Destroys all dynamically allocated memory associated with
 * this Chain object. Called by destructor and operator=.
 * You must complete its implementation for PA1.
 */
void Chain::Clear() {
	// complete your implementation below
	Node* curr = NW;
	Node* temp = NULL;

	while (curr) {
		temp = curr->next;
		delete curr;
		curr = temp;
	}

	curr = NULL;
	temp = NULL;
	SE = NULL;
	NW = NULL;
	length_ = 0;
}

/**
 * Copies the parameter other Chain into the current Chain.
 * Does not free any memory. Called by copy constructor and
 * operator=.
 * You must complete its implementation for PA1.
 * @param other The Chain to be copied.
 */
void Chain::Copy(const Chain& other) {
	// complete your implementation below
	Node* curr = other.NW;

	NW = NULL;
	SE = NULL;
	length_ = 0;

	while (curr) {
		InsertBack(curr->data);
		// Node* copy = new Node(block);
		// length_ += 1;
		
		// if (!NW) {
		// 	NW = copy;
		// } else {
		// 	copy->prev = tail;
		// 	tail->next = copy;
			
		// }

		// tail = copy;
		curr = curr->next;
	} 
}

/**
 * If you have declared any private helper functions in chain_private.h,
 * add your completed implementations below.
 */

/* void Chain::Reverse(Node* curr) {
	if (curr == NULL) {
		return;
	}

    Node* temp = curr->prev;
    curr->prev = curr->next;
    curr->next = temp;
    Reverse(curr->prev);
} */