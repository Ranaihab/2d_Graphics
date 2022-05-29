#pragma once
#include"Line.h"
void draw4(int xc, int yc, int x, int y, HDC hdc, COLORREF c) {
	Points::addPoint(hdc, xc + x, yc + y, c);
	Points::addPoint(hdc, xc - x, yc + y, c);
	Points::addPoint(hdc, xc - x, yc - y, c);
	Points::addPoint(hdc, xc + x, yc - y, c);
}
void DirectEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
	int x = 0;
	double y = b;
	draw4(xc, yc, x, Round(y), hdc, c);
	while (x * b * b < y * a * a) {
		x++;
		y = b * sqrt(1 - (x * x * 1.0) / (a * a));
		draw4(xc, yc, x, Round(y), hdc, c);
	}
	int y1 = 0;
	double x1 = a;
	draw4(xc, yc, Round(x1), y1, hdc, c);
	while (x1 * b * b > y1 * a * a) {
		y1++;
		x1 = a * sqrt(1 - (y1 * y1 * 1.0) / (b * b));
		draw4(xc, yc, Round(x1), y1, hdc, c);
	}
}
void PolarEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
	double x = a;
	double y = 0;
	double theta = 0;
	double dtheta = 1.0 / ((a+b));
	double cd = cos(dtheta);
	double sd = sin(dtheta);

	draw4(xc, yc, Round(x), Round(y), hdc, c);
	while (x>0) {
		x = a * cos(theta);
		y = b * sin(theta);
		theta += dtheta;
		draw4(xc, yc, Round(x), Round(y), hdc, c);
	}
}
void IterativePolarEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
	double x = 0;
	double y = b;
	double dtheta = 1.0 / max(a, b);
	double cd = cos(dtheta);
	double sd = sin(dtheta);

	draw4(xc, yc, Round(x), Round(y), hdc, c);
	while (y > 0) {
		double x1 = x * cd + a * (y / b) * sd;
		y = y * cd - (x / a) * b * sd;
		x = x1;
		draw4(xc, yc, Round(x), Round(y), hdc, c);
	}
}
void MidEllipse(HDC hdc, int xc, int yc, int a, int b, COLORREF c) {
	int x = 0, y = b;
	int b2 = b * b;
	int a2 = a * a;
	double d = b2 + a2 * pow((b - 0.5), 2) - a2 * b2;
	draw4(xc, yc, x, y, hdc, c);
	while (x * b * b < y * a * a) {
		if (d <= 0) {
			d += b2 * (2 * x + 3);
			x++;
		}
		else {
			d += b2 * (2 * x + 3) + a2 * (-2 * y + 2);
			x++;
			y--;
		}
		draw4(xc, yc, x, y, hdc, c);
	}
	x = a;
	y = 0;
	d = b2 * pow((a - 0.5), 2) + a2 - a2 * b2;
	draw4(xc, yc, x, y, hdc, c);
	while (x * b * b > y * a * a) {
		if (d <= 0) {
			d += a2 * (2 * y + 3);
			y++;
		}
		else {
			d += a2 * (2 * y + 3) + b2 * (-2 * x + 2);
			x--;
			y++;
		}
		draw4(xc, yc, x, y, hdc, c);
	}
}