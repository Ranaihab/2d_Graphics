#pragma once
#include"Line.h"
//implement circle algorithms
void Draw8Points(HDC hdc, int xc, int yc, int x, int y, COLORREF c) {
	Points::addPoint(hdc, xc + x, yc + y, c);
	Points::addPoint(hdc, xc - x, yc + y, c);
	Points::addPoint(hdc, xc - x, yc - y, c);
	Points::addPoint(hdc, xc + x, yc - y, c);
	Points::addPoint(hdc, xc + y, yc + x, c);
	Points::addPoint(hdc, xc - y, yc + x, c);
	Points::addPoint(hdc, xc - y, yc - x, c);
	Points::addPoint(hdc, xc + y, yc - x, c);
}
void DirectCircle(HDC hdc, int xc, int yc, int r, COLORREF c) {
	int x = 0;
	double y = r;
	Draw8Points(hdc, xc, yc, 0, r, c);
	while (x < y) {
		x++;
		y = sqrt(r * r - x * x);
		Draw8Points(hdc, xc, yc, x, Round(y), c);
	}
}
void PolarCircle(HDC hdc, int xc, int yc, int r, COLORREF c) {
	double theta = 0;
	double dtheta = 1.0 / r;
	double x = r;
	double y = 0;
	Draw8Points(hdc, xc, yc, Round(x), Round(y), RGB(255, 255, 255));
	while (x > y) {
		theta += dtheta;
		x = r * cos(theta);
		y = r * sin(theta);
		Draw8Points(hdc, xc, yc, Round(x), Round(y), c);
	}
}
void IterativeCircle(HDC hdc, int xc, int yc, int r, COLORREF c) {
	double theta = 1.0 / r;
	double cd = cos(theta);
	double sd = sin(theta);
	double x = r;
	double y = 0;
	Draw8Points(hdc, xc, yc, Round(x), Round(y), RGB(255, 255, 255));
	while (x > y) {
		double x1 = x * cd - y * sd;
		y = x * sd + y * cd;
		x = x1;
		Draw8Points(hdc, xc, yc, Round(x), Round(y), c);
	}
}
void modifiedMidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF c) {
	int x = 0, y = r;
	int d = 1 - r;
	int d1 = 3;
	int d2 = 5 - 2 * r;
	Draw8Points(hdc, xc, yc, x, y, c);
	while (x < y) {
		if (d < 0) {
			d += d1;
			d2 += 2;
			d1 += 2;
			x++;
		}
		else {
			d += d2;
			d2 += 4;
			d1 += 2;
			x++;
			y--;
		}
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}
void MidpointCircle(HDC hdc, int xc, int yc, int r, COLORREF c) {
	int x = 0, y = r;
	Draw8Points(hdc, xc, yc, x, y, c);
	while (x < y) {
		double d = (x + 1) * (x + 1) + (y - 0.5) * (y - 0.5) - r * r;
		if (d >= 0)
			y--;
		x++;
		Draw8Points(hdc, xc, yc, x, y, c);
	}
}