#pragma once
#include<iostream>
#include"Points.h"
void swap(int& x1, int& y1, int& x2, int& y2)
{
	int tmp = x1;
	x1 = x2;
	x2 = tmp;
	tmp = y1;
	y1 = y2;
	y2 = tmp;
}
int Round(double x)
{
	return (int)(x + 0.5);
}
//implement line algorithms
void BresenhamLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	std::cout << "hi";
	if (abs(dx) >= abs(dy)) {
		if (x1 > x2) {
			swap(x1, y1, x2, y2);
			dx = x2 - x1;
			dy = y2 - y1;
		}
		int x = x1;
		int y = y1;
		Points::addPoint(hdc, x, y, c);
		if (dy <= 0) {
			while (x < x2) {
				if (dx * (y - 0.5 - y1) - dy * (x + 1 - x1) < 0) {
					x++;

				}
				else {
					x++;
					y--;
				}
				Points::addPoint(hdc, x, y, c);
			}
		}
		else {
			while (x < x2) {
				if (dx * (y + 0.5 - y1) - dy * (x + 1 - x1) > 0) {
					x++;
				}
				else {
					x++;
					y++;
				}
				Points::addPoint(hdc, x, y, c);
			}
		}
	}
	else {
		if (y1 > y2) {
			swap(x1, y1, x2, y2);
			dx = x2 - x1;
			dy = y2 - y1;
		}
		int x = x1;
		int y = y1;
		Points::addPoint(hdc, x, y, c);
		if (dx <= 0) {
			while (y < y2) {
				if (dx * (y + 1 - y1) - dy * (x - 0.5 - x1) < 0) {
					y++;
					x--;
				}
				else {
					y++;
				}
				Points::addPoint(hdc, x, y, c);
			}
		}
		else {
			while (y < y2) {
				if (dx * (y + 1 - y1) - dy * (x + 0.5 - x1) < 0) {
					y++;
				}
				else {
					y++;
					x++;
				}
				Points::addPoint(hdc, x, y, c);
			}
		}
	}
}
void ParametricLine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c)
{
	double x, y;
	for (double i = 0; i <= 1; i += 0.001) {
		x = x1 + i * (x2 - x1);
		y = y1 + i * (y2 - y1);
		Points::addPoint(hdc, Round(x), Round(y), c);
	}

}
void DDALine(HDC hdc, int x1, int y1, int x2, int y2, COLORREF c) {
	int dx = x2 - x1;
	int dy = y2 - y1;
	if (abs(dx) >= abs(dy)) {
		if (x1 > x2)
			swap(x1, y1, x2, y2);
		int x = x1;
		double y = y1;
		double m = (double)dy / dx;
		Points::addPoint(hdc, x1, y1, c);
		while (x<x2) {
			x++;
			y += m;
			Points::addPoint(hdc, x, Round(y), c);
		}
	}
	else {
		if (y1 > y2)
			swap(x1, y1, x2, y2);
		double x = x1;
		int y = y1;
		double m = (double)dx / dy;
		Points::addPoint(hdc, x1, y1, c);
		while (y<y2) {
			y++;
			x += m;
			Points::addPoint(hdc, Round(x), y, c);
		}
	}
}