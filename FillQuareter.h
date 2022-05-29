#pragma once
#include "Line.h"
#include "math.h"
#include<Windows.h>

//common function between FillCircleWithLines and FillCircleWithCircles 
//checks which octant the user clicked on (going anti-clockwise)
//will be called twice in both functions( FillCircleWithLines and FillCircleWithCircles ) becuase the user will be clicking on 2 octants (1 quadrant).
int checkOctant(int xc, int yc, int x1, int y1) {

    if (x1 > xc && y1 < yc && abs(x1 - xc) > abs(y1 - yc)) return 1;
    if (x1 > xc && y1 < yc && abs(x1 - xc) < abs(y1 - yc)) return 2;

    if (x1 < xc && y1 < yc && abs(x1 - xc) < abs(y1 - yc)) return 3;
    if (x1 < xc && y1 < yc && abs(x1 - xc) > abs(y1 - yc)) return 4;

    if (x1 < xc && y1 > yc && abs(x1 - xc) > abs(y1 - yc)) return 5;
    if (x1 < xc && y1 > yc && abs(x1 - xc) < abs(y1 - yc)) return 6;

    if (x1 > xc && y1 > yc && abs(x1 - xc) < abs(y1 - yc)) return 7;
    if (x1 > xc && y1 > yc && abs(x1 - xc) > abs(y1 - yc)) return 8;

}

// fill circle with circles 


//Draws only 2 octants of the surface of one circle.
void CircleDrawQuarter(HDC hdc, int xc, int yc, int octant1, int octant2, int a, int b, COLORREF color) {
    if (octant1 == 1 || octant2 == 1) {
        Points::addPoint(hdc, xc + b, yc - a, color);
    }
    if (octant1 == 2 || octant2 == 2) {
        Points::addPoint(hdc, xc + a, yc - b, color);
    }
    if (octant1 == 3 || octant2 == 3) {
        Points::addPoint(hdc, xc - a, yc - b, color);
    }
    if (octant1 == 4 || octant2 == 4) {
        Points::addPoint(hdc, xc - b, yc - a, color);
    }
    if (octant1 == 5 || octant2 == 5) {
        Points::addPoint(hdc, xc - b, yc + a, color);
    }
    if (octant1 == 6 || octant2 == 6) {
        Points::addPoint(hdc, xc - a, yc + b, color);
    }
    if (octant1 == 7 || octant2 == 7) {
        Points::addPoint(hdc, xc + a, yc + b, color);
    }
    if (octant1 == 8 || octant2 == 8) {
        Points::addPoint(hdc, xc + b, yc + a, color);
    }
}
//The Direct Circle Algorithm which draws the sircle but instead of calling Draw8Points, it will be calling CircleDrawQuarter which basically draws only 2 points according to the octants clicked on
void DrawOneDirectCircle(HDC hdc, int xc, int yc, double r, int octant1, int octant2, COLORREF c) {
    int x = 0;
    double y = r;
    CircleDrawQuarter(hdc, xc, yc, octant1, octant2, floor(x), floor(y), c);
    CircleDrawQuarter(hdc, xc, yc, octant1, octant2, ceil(x), ceil(y), c);
    while (x < y - 1) {
        x++;
        y = sqrt(r * r - x * x);
        CircleDrawQuarter(hdc, xc, yc, octant1, octant2, floor(x), floor(y), c);
        CircleDrawQuarter(hdc, xc, yc, octant1, octant2, ceil(x), ceil(y), c);
    }
}

//will draw circles starting from a radius of 0 to a radius of R
void FillCircleWithCircles(HDC hdc, int xc, int yc, int R, int x1, int y1, int x2, int y2, COLORREF color) {
    int octant1 = checkOctant(xc, yc, x1, y1);
    int octant2 = checkOctant(xc, yc, x2, y2);
    for (double i = 0; i <= R; i += 0.2) {
        DrawOneDirectCircle(hdc, xc, yc, i, octant1, octant2, color);
    }
}

// Fill circle with lines 

void LineDrawQuarter(HDC hdc, int xc, int yc, int octant1, int octant2, int a, int b, COLORREF color) {
    if (octant1 == 1 || octant2 == 1) {
        ParametricLine(hdc, xc, yc, xc + b, yc - a, color);
    }
    if (octant1 == 2 || octant2 == 2) {
        ParametricLine(hdc, xc, yc, xc + a, yc - b, color);
    }
    if (octant1 == 3 || octant2 == 3) {
        ParametricLine(hdc, xc, yc, xc - a, yc - b, color);
    }
    if (octant1 == 4 || octant2 == 4) {
        ParametricLine(hdc, xc, yc, xc - b, yc - a, color);
    }
    if (octant1 == 5 || octant2 == 5) {
        ParametricLine(hdc, xc, yc, xc - b, yc + a, color);
    }
    if (octant1 == 6 || octant2 == 6) {
        ParametricLine(hdc, xc, yc, xc - a, yc + b, color);
    }
    if (octant1 == 7 || octant2 == 7) {
        ParametricLine(hdc, xc, yc, xc + a, yc + b, color);
    }
    if (octant1 == 8 || octant2 == 8) {
        ParametricLine(hdc, xc, yc, xc + b, yc + a, color);
    }
}

//Direct circle algorithm which will draw a line between the center of the circle and each point on the border of the chosen quadrant
void FillCircleWithLines(HDC hdc, int xc, int yc, int r, int x1, int y1, int x2, int y2, COLORREF color) {
    int x = 0;
    double y = r;
    int octant1 = checkOctant(xc, yc, x1, y1);
    int octant2 = checkOctant(xc, yc, x2, y2);
    LineDrawQuarter(hdc, xc, yc, octant1, octant2, ceil(x), ceil(y), color);
    LineDrawQuarter(hdc, xc, yc, octant1, octant2, floor(x), floor(y), color);
    while (x < y) {
        x++;
        y = sqrt(r * r - x * x);
        LineDrawQuarter(hdc, xc, yc, octant1, octant2, ceil(x), ceil(y), color);
        LineDrawQuarter(hdc, xc, yc, octant1, octant2, floor(x), floor(y), color);
    }
}
