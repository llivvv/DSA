#include "stats.h"

stats::stats(PNG& im) {
    unsigned int width = im.width();
    unsigned int height = im.height();

    sumRed.resize(height + 1);
    sumGreen.resize(height + 1);
    sumBlue.resize(height + 1);

    sumsqRed.resize(height + 1);
    sumsqGreen.resize(height + 1);
    sumsqBlue.resize(height + 1);
    
    for (unsigned int y = 0; y < height + 1; y++) {
        
        sumRed[y].resize(width + 1);
        sumGreen[y].resize(width + 1);
        sumBlue[y].resize(width + 1);

        sumsqRed[y].resize(width + 1);
        sumsqGreen[y].resize(width + 1);
        sumsqBlue[y].resize(width + 1);

        for (unsigned int x = 0; x < width + 1; x++) {

            if (x == 0 || y == 0) {
                sumRed[y][x] = 0;
                sumGreen[y][x] = 0;
                sumBlue[y][x] = 0;
                sumsqRed[y][x] = 0;
                sumsqGreen[y][x] = 0;
                sumsqBlue[y][x] = 0; 

            } else {
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
}

long stats::getSum(char channel, pair<int, int> ul, int dim) {
    unsigned int startX = ul.first + 1;
    unsigned int startY = ul.second + 1; 

    unsigned int endX = pow(2, dim) + startX - 1; 
    unsigned int endY = pow(2, dim) + startY - 1;

    if (channel == 'r') {
        return (sumRed[endY][endX] + sumRed[startY - 1][startX - 1] - sumRed[startY - 1][endX] - sumRed[endY][startX - 1]);
    } else if (channel == 'g') {
        return (sumGreen[endY][endX] + sumGreen[startY - 1][startX - 1] - sumGreen[startY - 1][endX] - sumGreen[endY][startX - 1]);
    } else {
        return (sumBlue[endY][endX] + sumBlue[startY - 1][startX - 1] - sumBlue[startY - 1][endX] - sumBlue[endY][startX - 1]);
    }
}

long stats::getSumSq(char channel, pair<int, int> ul, int dim) {

    unsigned int startX = ul.first + 1;
    unsigned int startY = ul.second + 1; 

    unsigned int endX = pow(2, dim) + startX - 1; 
    unsigned int endY = pow(2, dim) + startY - 1;

    if (channel == 'r') {
        return (sumsqRed[endY][endX] + sumsqRed[startY - 1][startX - 1] - sumsqRed[startY - 1][endX] - sumsqRed[endY][startX - 1]);
    } else if (channel == 'g') {
        return (sumsqGreen[endY][endX] + sumsqGreen[startY - 1][startX - 1] - sumsqGreen[startY - 1][endX] - sumsqGreen[endY][startX - 1]);
    } else {
        return (sumsqBlue[endY][endX] + sumsqBlue[startY - 1][startX - 1] - sumsqBlue[startY - 1][endX] - sumsqBlue[endY][startX - 1]);
    }
}

long stats::rectArea(int dim) {

    return pow(2, 2*dim);
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int, int> ul, int dim) {
    
    /* Your code here!! */
    long numPixels = rectArea(dim);

    double varR = getSumSq('r', ul, dim)-pow(getSum('r', ul, dim), 2)/numPixels;
    double varG = getSumSq('g', ul, dim)-pow(getSum('g', ul, dim), 2)/numPixels;
    double varB = getSumSq('b', ul, dim)-pow(getSum('b', ul, dim), 2)/numPixels;

    return varR + varG + varB;
}

RGBAPixel stats::getAvg(pair<int, int> ul, int dim) {
    long numPixels = rectArea(dim);

    //if (numPixels == 0) return RGBAPixel(0, 0, 0);

    long avgR = (getSum('r', ul, dim)) / numPixels;
    long avgG = (getSum('g', ul, dim)) / numPixels;
    long avgB = (getSum('b', ul, dim)) / numPixels;

    return RGBAPixel(avgR, avgG, avgB);
}