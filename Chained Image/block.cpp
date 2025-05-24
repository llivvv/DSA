#include "block.h"
#include <cmath>
#include <iostream>

int Block::width() const
{
    return data[0].size();
/* YOUR CODE HERE */
}
int Block::height() const
{
    return data.size();
/* YOUR CODE HERE */
}

void Block::render(PNG &im, int x, int y) const
{
    // idea:
    // wanna place the pixel from the block into the point in image
    // for each element of data (the vector)
    // for each item of the vector

    // elem represent vector<RGBAPixel>
    // item represent RGBAPixel

    for (unsigned int elem = 0; elem < height(); elem++) {
        if (im.height() <= y + elem) break;
        for (unsigned int item = 0; item < width(); item++) {

            if (im.width() <= x + item) break; 

            // the block pixel
            //vector<RGBAPixel> blkRow = data[elem];
            //RGBAPixel blkPixel = blkRow[item];
            RGBAPixel blkPixel = data[elem][item];
            RGBAPixel* imPixel = im.getPixel(x + item, y + elem);

            // changing pixel value
            *imPixel = blkPixel; 
            // imPixel->r = blkPixel.r; 
            // imPixel->g = blkPixel.g;
            // imPixel->b = blkPixel.b;
        }
    }
/* YOUR CODE HERE */
}

void Block::build(PNG &im, int column, int row, int width, int height)
{
    // data.resize(height);
    for (int y = row; y < row + height; y++) {
        vector<RGBAPixel> currRow;
        for (int x = column; x < column + width; x++) {
            RGBAPixel* pixel = im.getPixel(x,y);

            currRow.push_back(*pixel);
        }
        data.push_back(currRow);
    }
/* YOUR CODE HERE */

// mb here gotta initialize the height of the block? declare the vars in heap?

// idea: 
// start at x = column, go up to until x < column + width 
// initialize a vector for the entire column
// start at y = row, go up y < row + height
// get the img pixel
// add pixel to the vector for the column
}

void Block::flipVert()
{

    // idea:
    // for each of the vectors in the outer vector, flip the order of the list
    // need to swap the vectors?
    
    int tRow = 0; 
    int bRow = height() - 1;

    while (tRow < bRow) {

        // swapping the outermost rows in the block
        // vector<RGBAPixel> temp = data[tRow];
        // data[tRow] = data[bRow];
        // data[bRow] = temp;

        std::swap(data[tRow], data[bRow]);

        tRow++;
        bRow--;
    }
/* YOUR CODE HERE */
}

void Block::flipHoriz()
{
    for (int y = 0; y < height(); y++) {

        // it works if currRow is a pointer, but idk why?
        // the pixels dont seem to be modified if currRow is just a vector of pixels?
        
        //vector<RGBAPixel>* currRow = &(data[y]);

        int leftCol = 0; 
        int rightCol = width() - 1;

        while (leftCol < rightCol) {
            std::swap(data[y][leftCol], data[y][rightCol]);

            // RGBAPixel temp = (*currRow)[leftCol];
            // (*currRow)[leftCol] = (*currRow)[rightCol];
            // (*currRow)[rightCol] = temp; 
            leftCol++;
            rightCol--;
        }
    }
/* YOUR CODE HERE */
}

void Block::rotateRight()
{ 
    // setting up a new 2D array
    int height = data.size();
    vector<vector<RGBAPixel>> newData; 
    newData.resize(height); 

    // for each vector<RGBAPixel> in 2D array, allocating height spaces for each element
    for (int i = 0; i < height; i++) {
        vector<RGBAPixel> newRow;
        newRow.resize(height); 
        newData.insert(newData.begin() + i, newRow); 
    }  

    //oRow represents a row in block's original data
    for (int oRow = height - 1; oRow >= 0; oRow--) {
        //oldCurrRow is a copy of an old row of data
        vector<RGBAPixel> oldCurrRow = data[oRow]; 
        //within an old row, oCol represents a column
        for (int oCol = 0; oCol < height; oCol++) {
            //currNewRow is a row of the newData
            
            vector<RGBAPixel>* currNewRow = &(newData[oCol]);
            //vector<RGBAPixel> currNewRow = newData[oCol];

            //cout << oCol << endl;
            
            //newRowPos is the row that the item should go in
            int newColPos = height - 1 - oRow; 
            (*currNewRow).insert((*currNewRow).begin() + newColPos, oldCurrRow[oCol]);
            //currNewRow.insert(currNewRow.begin() + newColPos, oldCurrRow[oCol]);
        } 
    }
    data = newData; 
/* YOUR CODE HERE */
}
