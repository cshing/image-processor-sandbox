/**
 * File: block.cpp
 * Description: student implementation for Block functions
 *              for CPSC 221 2023S PA1
 *
 *              THIS FILE WILL BE SUBMITTED.
 */

#include "block.h"

/**
 * Creates a square Block that is dimension x dimension pixels in size,
 * starting at (x, y) on img. Assumes that the requested
 * Block fits on the image (inputs are feasible)
 */
void Block::Build(PNG& img, unsigned int x, unsigned int y, unsigned int dimension) {
	// complete your implementation below
	data.resize(dimension, vector<HSLAPixel>(dimension));

	for (unsigned int row = 0; row < dimension; row++) {
		for (unsigned int col = 0; col < dimension; col++) {
			data[row][col] = *img.getPixel(x + col, y + row);
		}
	}
}

/**
 * Renders this Block onto img with its upper-left corner at (x, y).
 * Assumes block fits on the image.
 */
void Block::Render(PNG& img, unsigned int x, unsigned int y) const {
	// complete your implementation below
	unsigned int dimension = Dimension();

	for (unsigned int row = 0; row < dimension; row++) {
		for (unsigned int col = 0; col < dimension; col++) {
			*img.getPixel(x + col, y + row) = data[row][col];
		}
	}
}

/**
 * Flips the Block's pixel data horizontally by a simple reflection
 * across the vertical axis through its middle.
 */
void Block::FlipHorizontal() {
	// complete your implementation below
	unsigned int dimension = Dimension();

	for (unsigned int row = 0; row < dimension; row++) {
		int i = dimension - 1;
		for (unsigned int col = 0; col < dimension / 2; col++) {
			swap(data[row][col], data[row][i--]);
		}
	}
}

/**
 * Rotates the Block's pixel data 90 degrees counter-clockwise.
 */
void Block::RotateCCW() {
	// complete your implementation below
	unsigned int dimension = Dimension();
	vector<vector<HSLAPixel>> temp;
	temp.resize(dimension, vector<HSLAPixel>(dimension));

	for (unsigned int row = 0; row < dimension; row++) {
		unsigned int i = dimension - 1;
		for (unsigned int col = 0; col < dimension; col++) {
			temp[i--][row] = data[row][col];
		}
	}
	swap(temp, data);
}

/**
 * Returns the dimension of the block (its width or height).
 */
unsigned int Block::Dimension() const {
	// replace the statement below with your implementation
	return data.size();
}