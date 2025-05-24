#include "stats.h"

stats::stats(PNG& im) {
    int width = im.width();
    int height = im.height();

    sumRed.assign(height + 1, vector<long>(width + 1, 0));
    sumGreen.assign(height + 1, vector<long>(width + 1, 0));
    sumBlue.assign(height + 1, vector<long>(width + 1, 0));
    sumsqRed.assign(height + 1, vector<long>(width + 1, 0));
    sumsqGreen.assign(height + 1, vector<long>(width + 1, 0));
    sumsqBlue.assign(height + 1, vector<long>(width + 1, 0));
    
    for (int y = 1; y < height + 1; y++) {

        for (int x = 1; x < width + 1; x++) {
            RGBAPixel* pixel = im.getPixel(x-1, y-1);

            sumRed[y][x] = sumRed[y][x-1] + sumRed[y-1][x] - sumRed[y-1][x-1] + pixel->r; 
            sumGreen[y][x] = sumGreen[y][x-1] + sumGreen[y-1][x] - sumGreen[y-1][x-1] + pixel->g;
            sumBlue[y][x] = sumBlue[y][x-1] + sumBlue[y-1][x] - sumBlue[y-1][x-1] + pixel->b;
            sumsqRed[y][x] = sumsqRed[y][x-1] + sumsqRed[y-1][x] - sumsqRed[y-1][x-1] + pow(pixel->r, 2);
            sumsqGreen[y][x] = sumsqGreen[y][x-1] + sumsqGreen[y-1][x] - sumsqGreen[y-1][x-1] + pow(pixel->g, 2);
            sumsqBlue[y][x] = sumsqBlue[y][x-1] + sumsqBlue[y-1][x] - sumsqBlue[y-1][x-1] + pow(pixel->b, 2);
        }
    }
}


long stats::getSum(char channel, pair<int, int> ul, int dim) {

    vector<vector<long>>* vec; 
    if (channel == 'r') {
        vec = &(sumRed); 
    } else if (channel == 'g') {
        vec = &(sumGreen); 
    } else {
        vec = &(sumBlue); 
    } 

    unsigned int startX = ul.first + 1;
    unsigned int startY = ul.second + 1; 

    unsigned int endX = pow(2, dim) + startX - 1; 
    unsigned int endY = pow(2, dim) + startY - 1;

    return (*vec)[endY][endX] + (*vec)[startY - 1][startX - 1] - (*vec)[startY - 1][endX] - (*vec)[endY][startX - 1];
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {

    vector<vector<long>>* vec; 
    if (channel == 'r') {
        vec = &(sumsqRed); 
    } else if (channel == 'g') {
        vec = &(sumsqGreen); 
    } else {
        vec = &(sumsqBlue); 
    } 
    
    unsigned int startX = ul.first + 1;
    unsigned int startY = ul.second + 1; 

    unsigned int endX = pow(2, dim) + startX - 1; 
    unsigned int endY = pow(2, dim) + startY - 1;

    return (*vec)[endY][endX] + (*vec)[startY - 1][startX - 1] - (*vec)[startY - 1][endX] - (*vec)[endY][startX - 1];
}

long stats::rectArea(int dim) {

    return pow(2, 2*dim);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    
    long numPixels = rectArea(dim);

    double varR = getSumSq('r', ul, dim)-pow(getSum('r', ul, dim), 2)/numPixels;
    double varG = getSumSq('g', ul, dim)-pow(getSum('g', ul, dim), 2)/numPixels;
    double varB = getSumSq('b', ul, dim)-pow(getSum('b', ul, dim), 2)/numPixels;

    return (double) varR + varG + varB;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    long numPixels = rectArea(dim);

    long avgR = (getSum('r', ul, dim)) / numPixels;
    long avgG = (getSum('g', ul, dim)) / numPixels;
    long avgB = (getSum('b', ul, dim)) / numPixels;

    return RGBAPixel(avgR, avgG, avgB);
}