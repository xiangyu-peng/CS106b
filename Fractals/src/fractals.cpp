//Homework 3 Fractals and Graphics
//Xiangyu Peng (xypeng5) and Mingcheng Yi (mingchyi)
//This file creates Sierpinski Triangle, as well as perform flood fill

#include "fractals.h"
#include "gwindow.h"
#include <cmath>
#include <iomanip>
using namespace std;

void drawSierpinskiTriangleHelper(GWindow& window, double x, double y, double size, int order);
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order);
int floodFillHelper(GWindow& window, int x, int y, int& color, int& originColor);
int floodFill(GWindow& window, int x, int y, int color);

//This is function draws Sierplinski Triangle with the given origin, side length, and order
void drawSierpinskiTriangle(GWindow& window, double x, double y, double size, int order) {
    if (order > 0){
        window.drawLine(x, y, x+size, y);
        window.drawLine(x, y, x+size/2, y+size*sqrt(3)/2);
        window.drawLine(x+size, y, x+size/2, y+size*sqrt(3)/2);  // draw the big triangle when order=1
        drawSierpinskiTriangleHelper(window, x, y, size/2, order-1);  //use helper function to generate small triangles
    }
}

//This is a helper function for the Sierplinski Triangle drawing function
void drawSierpinskiTriangleHelper(GWindow& window, double x, double y, double size, int order){
    if (order>=1){
        window.drawLine(x + size, y, x + size/2, y + sqrt(3) * size/2);  //draw the triangle
        window.drawLine(x + size, y, x + 3 * size/2, y + sqrt(3) * size/2);
        window.drawLine(x + size/2, y + sqrt(3) * size/2, x + 3 * size/2, y + sqrt(3) * size/2);
        drawSierpinskiTriangleHelper(window, x, y, size/2, order-1);                             //recursive base
        drawSierpinskiTriangleHelper(window, x+size, y, size/2, order-1);
        drawSierpinskiTriangleHelper(window, x + size/2, y + sqrt(3) * size/2, size/2, order-1);
    }
}

//This is the function that fills the chosen shape with chosen color
int floodFill(GWindow& window, int x, int y, int color) {
    if (window.getPixel(x, y) == color){
        return 0;
    } else{
        int originColor = window.getPixel(x, y);
        floodFillHelper(window, x, y, color, originColor);
        return 0;
    }
}

//This is the helper function of the flood fill operation
int floodFillHelper(GWindow& window, int x, int y, int& color, int& originColor){
    if (window.inCanvasBounds(x, y)){                           //make sure the index is in bound
        if (window.getPixel(x, y) == color){                    //do nothing if the color matches
            return 0;
        }
        else if(window.getPixel(x, y) == originColor){
            window.setPixel(x, y, color);                       //change the color if the pixel has the original color
            floodFillHelper(window, x - 1, y, color, originColor);   //perform recursion around the pixel
            floodFillHelper(window, x + 1, y, color, originColor);
            floodFillHelper(window, x, y - 1, color, originColor);
            floodFillHelper(window, x, y + 1, color, originColor);
        }
    }
    return 0;
}


