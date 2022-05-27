#pragma once
#include"Line.h"
// rectangle
void Bezier(HDC hdc, int x[], int y[], COLORREF c) {
	int a1 = x[0];
	int a2 = 3 * (x[1] - x[0]);
	int a3 = 3 * x[0] - 6 * x[1] + 3 * x[2];
	int a4 = -1 * x[0] + 3 * x[1] - 3 * x[2] + x[3];
	int b1 = y[0];
	int b2 = 3 * (y[1] - y[0]);
	int b3 = 3 * y[0] - 6 * y[1] + 3 * y[2];
	int b4 = -1 * y[0] + 3 * y[1] - 3 * y[2] + y[3];

	for (double t = 0; t <= 1; t += 0.001) {
		double t2 = t * t, t3 = t2 * t;
		double x1 = a1 + a2 * t + a3 * t2 + a4 * t3;
		double y1 = b1 + b2 * t + b3 * t2 + b4 * t3;
		Points::addPoint(hdc, Round(x1), Round(y1), c);
	}
}
void drawRec(HDC hdc, int x1, int y1, int w, int h, COLORREF c) {
	int y2 = y1 + h, x2 = x1 + w;
	DDALine(hdc, x1, y1, x1, y2, c);
	DDALine(hdc, x1, y2, x2, y2, c);
	DDALine(hdc, x2, y2, x2, y1, c);
	DDALine(hdc, x2, y1, x1, y1, c);
	int cX = x1 + w / 2;
	int cy = y1 + h / 2;
	while (y1 <= y2) {
		int x[4];
		int y[4];
		x[0] = x1, x[1] = cX , x[2] = x2, x[3] = x2;
		y[0] = y1, y[1] = y1, y[2] = cy, y[3] = y1;
		Bezier(hdc, x, y, c);
		y1++;
	}
	y1 = y2 - h;
	while (y1 <= y2) {
		int x[4];
		int y[4];
		x[0] = x1, x[1] = x1, x[2] = cX, x[3] = x2;
		y[0] = y1, y[1] = cy, y[2] = y1, y[3] = y1;
		Bezier(hdc, x, y, c);
		y1++;
	}
}

//Draw and fill square
void Hermite(HDC hdc, int x[], int y[], COLORREF c) { // index 0 = point1, 1 = slope1, 2= slope2, 3 = point2 
	int a1 = x[0];
	int a2 = x[1];
	int a3 = -3 * x[0] - 2 * x[1] + 3 * x[3] - x[2];
	int a4 = 2 * x[0] + x[1] - 2 * x[3] + x[2];
	int b1 = y[0];
	int b2 = y[1];
	int b3 = -3 * y[0] - 2 * y[1] + 3 * y[3] - y[2];
	int b4 = 2 * y[0] + y[1] - 2 * y[3] + y[2];

	for (double t = 0; t <= 1; t += 0.001) {
		double t2 = t * t, t3 = t2 * t;
		double x1 = a1 + a2 * t + a3 * t2 + a4 * t3;
		double y1 = b1 + b2 * t + b3 * t2 + b4 * t3;
		Points::addPoint(hdc, Round(x1), Round(y1), c);
	}
}
void drawSquare(HDC hdc, int x1, int y1, int length, COLORREF c) {
	int x2 = x1 + length;
	DDALine(hdc, x1, y1, x2, y1, c);
	DDALine(hdc, x1, y1, x1, y1 + length, c);
	DDALine(hdc, x1 + length, y1 + length, x1, y1 + length, c);
	DDALine(hdc, x1 + length, y1 + length, x1 + length, y1, c);
	int cY = y1 + length / 2;
	int cX = x1 + length / 2;
//	DDALine(hdc, x1, cY, x2, cY, c);
	while (x1 <= x2) {
		int x[4], y[4];
		x[0] = x1, x[1] = (x1 - x1) * 3, x[2] = (x1 - cX) * 3, x[3] = x1;
		y[0] = y1, y[1] = 3 * (cY - y1); y[2] = 3 * (y1 - y1), y[3] = y1 + length;
		Hermite(hdc, x, y, c);
		x1++;
	}
	x1 = x2 - length;
	while (x1 <= x2) {
		int x[4], y[4];
		x[0] = x1, x[1] = (cX - x1) * 3, x[2] = (x1 - x1) * 3, x[3] = x1;
		y[0] = y1, y[1] = 3 * (y1 - y1); y[2] = 3 * (cY - y1), y[3] = y1 + length;
		Hermite(hdc, x, y, c);
		x1++;
	}
}

//spline
void Spline(HDC hdc, int x[], int y[], int num, COLORREF c) {
	int* u = new int[num];
	int* v = new int[num];
	for (int i = 1; i < num; i++) {
		u[i] = 0.5 * (x[i + 1] - x[i - 1]);
		v[i] = 0.5 * (y[i + 1] - y[i - 1]);
	}
	u[0] = 0.5 * (x[1] - x[0]);
	v[0] = 0.5 * (y[1] - y[0]);
	u[num-1] = 0.5 * (x[num-1] - x[num-2]);
	v[num-1] = 0.5 * (y[num-1] - y[num-2]);
	for (int i = 0; i < num - 1; i++) {
		int dx[4], dy[4];
		dx[0] = x[i]; dx[1] = u[i]; dx[2] = u[i + 1]; dx[3] = x[i + 1];
		dy[0] = y[i]; dy[1] = v[i]; dy[2] = v[i + 1]; dy[3] = y[i + 1];
		Hermite(hdc, dx, dy, c);
	}
	delete[] u;
	delete[] v;
}
//
//else if (HIWORD(wp) == CBN_SELCHANGE)
//		{
//		int ItemIndex = SendMessage((HWND)lp, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
//		TCHAR  ListItem[256];
//		(TCHAR)SendMessage((HWND)lp, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
//		hdc = GetDC(hwnd);
//		if (LOWORD(wp) == 2) {
//			if (!_tcscmp(ListItem, _T("DDA")) && cnt == 2) {
//				DDALine(hdc, x[0], y[0], x[1], y[1], c);
//				cnt = 0;
//			}
//			else if (!_tcscmp(ListItem, _T("Bresenham")) && cnt == 2) {
//				BresenhamLine(hdc, x[0], y[0], x[1], y[1], c);
//				cnt = 0;
//			}
//			else if (!_tcscmp(ListItem, _T("Parametric")) && cnt == 2) {
//				ParametricLine(hdc, x[0], y[0], x[1], y[1], c);
//				cnt = 0;
//			}
//			SendMessage(lineHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
//		}
//		else if (LOWORD(wp) == 3 && cnt == 2) {
//			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
//			if (!_tcscmp(ListItem, _T("Direct"))) {
//				DirectCircle(hdc, x[0], y[0], r, c);
//				cnt = 0;
//			}
//			else if (!_tcscmp(ListItem, _T("Polar"))) {
//				PolarCircle(hdc, x[0], y[0], r, c);
//				cnt = 0;
//			}
//			else if (!_tcscmp(ListItem, _T("Iterative"))) {
//				IterativeCircle(hdc, x[0], y[0], r, c);
//				cnt = 0;
//			}
//			else if (!_tcscmp(ListItem, _T("Modified Midpoint"))) {
//				modifiedMidpointCircle(hdc, x[0], y[0], r, c);
//				cnt = 0;
//			}
//			else if (!_tcscmp(ListItem, _T("Midpoint"))) {
//				MidpointCircle(hdc, x[0], y[0], r, c);
//				cnt = 0;
//			}
//			SendMessage(circleHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
//		}
//		else if (LOWORD(wp) == 4 && cnt == 3) {
//			int a = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
//			int b = sqrt((x[0] - x[2]) * (x[0] - x[2]) + (y[0] - y[2]) * (y[0] - y[2]));
//			if (!_tcscmp(ListItem, _T("Direct"))) {
//				DirectEllipse(hdc, x[0], y[0], a, b, c);
//			}
//			else if (!_tcscmp(ListItem, _T("Polar"))) {
//				PolarEllipse(hdc, x[0], y[0], a, b, c);
//			}
//			else if (!_tcscmp(ListItem, _T("Midpoint"))) {
//				MidEllipse(hdc, x[0], y[0], a, b, c);
//			}
//			cnt = 0;
//			SendMessage(ellipseHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
//		}
//		ReleaseDC(hwnd, hdc);
//		}