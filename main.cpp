#include <Windows.h>
#include <CommCtrl.h>
#include<string>
#include <tchar.h>
#include <fstream>
#include<iostream>
#include <math.h>
#include"Line.h"
#include"Circle.h"
#include"Ellipse.h"
#include"Curves.h"
#include"Points.h"
#include"Clipping.h"
using namespace std;
enum Action{
	parametricLine, dDALine, bresenhamLine, 
	directCircle, polarCircle, iterativeCircle, modifiedCircle, midCircle, 
	directEllipse, polarEllipse, midEllipse,
	splines, squareHermite, recBezier,
	fillCircleWithLines, fillCircleWithCircles, convexFill, nonConvexFill, recursiveFlood, nonRecursiveFlood,
	clipRecPoint, clipRecLine, clipRecPolygon, clipSquarePoint, clipSquareLine, clipCirclePoint, clipCircleLine
};

HWND createLineDropDown(HWND hwnd) {
	HWND lineHwnd = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE, 10, 0, 100, 550, hwnd, (HMENU)2, NULL, NULL);
	SendMessage(lineHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Line"));
	SendMessage(lineHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("DDA"));
	SendMessage(lineHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Bresenham"));
	SendMessage(lineHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Parametric"));
	SendMessage(lineHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return lineHwnd;
}

HWND createCircleDropDown(HWND hwnd) {
	HWND circleHwnd = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE, 110, 0, 120, 550, hwnd, (HMENU)3, NULL, NULL);
	SendMessage(circleHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Circle"));
	SendMessage(circleHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Direct"));
	SendMessage(circleHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Polar"));
	SendMessage(circleHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Iterative"));
	SendMessage(circleHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Modified Midpoint"));
	SendMessage(circleHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Midpoint"));
	SendMessage(circleHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return circleHwnd;
}

HWND createEllipseDropDown(HWND hwnd) {
	HWND ellipseHwnd = CreateWindow(WC_COMBOBOX, TEXT(""),
		CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE, 230, 0, 100, 550, hwnd, (HMENU)4, NULL, NULL);
	SendMessage(ellipseHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Ellipse"));
	SendMessage(ellipseHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Direct"));
	SendMessage(ellipseHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Polar"));
	SendMessage(ellipseHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Midpoint"));
	SendMessage(ellipseHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return ellipseHwnd;
}

HWND createCurveDropDown(HWND hwnd) {
	HWND curveHwnd = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE, 330, 0, 150, 550, hwnd, (HMENU)5, NULL, NULL);
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Curve"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Cardinal Splines"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("fill square with hermite"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("fill rec with bezier"));
	SendMessage(curveHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return curveHwnd;
}

HWND createFillingDropDown(HWND hwnd) {
	HWND curveHwnd = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE | WS_HSCROLL, 480, 0, 170, 550, hwnd, NULL, NULL, NULL);
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Filling"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Circle with Lines"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Circle with Circles"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Convex Filling"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Non-Convex Filling"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Recursive Flood Fill"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Non-Recursive Flood Fill"));
	SendMessage(curveHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return curveHwnd;
}

HWND createClippingDropDown(HWND hwnd) {
	HWND curveHwnd = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE, 650, 0, 150, 550, hwnd, NULL, NULL, NULL);
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Clip"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Rectangle, Point"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Rectangle, Line"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Rectangle, Polygon"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Square, Point"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Square, Line"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Circle, Point"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Circle, Line"));
	SendMessage(curveHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return curveHwnd;
}
HWND createTextBox(HWND hwnd)
{
    HWND textBox = CreateWindow("EDIT", "Enter number of sides", WS_BORDER | WS_CHILD | WS_VISIBLE, 10, 30, 150, 22, hwnd, NULL, NULL, NULL);
    return textBox;
}

HWND createColorDropDown(HWND hwnd) {
	HWND colorHwnd = CreateWindow(WC_COMBOBOX, TEXT("color"), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE, 800, 0, 100, 550, hwnd, (HMENU)1, NULL, NULL);
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Color"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Red"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Blue"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Green"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Yellow"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Orange"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Grey"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Pink"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Purple"));
	SendMessage(colorHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Black"));
	SendMessage(colorHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return colorHwnd;
}

HWND createScreenDropDown(HWND hwnd) {
	HWND curveHwnd = CreateWindow(WC_COMBOBOX, TEXT(""), CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE | WS_VISIBLE, 900, 0, 100, 550, hwnd, NULL, NULL, NULL);
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Screen"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Clear"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Load Data"));
	SendMessage(curveHwnd, (UINT)CB_ADDSTRING, 0, (LPARAM)TEXT("Save Data"));
	SendMessage(curveHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return curveHwnd;
}

COLORREF selectColor(LPARAM lp) {
	COLORREF c = RGB(0,0,0);
	int ItemIndex = SendMessage((HWND)lp, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	TCHAR  ListItem[256];
	(TCHAR)SendMessage((HWND)lp, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
	cout << "Color " << ListItem << " is chosen." << endl;
	if (!_tcscmp(ListItem, _T("Red"))) {
		c = RGB(255, 0, 0);
	}
	else if (!_tcscmp(ListItem, _T("Blue"))) {
		c = RGB(0, 0, 255);
	}
	else if (!_tcscmp(ListItem, _T("Green"))) {
		c = RGB(0, 255, 0);
	}
	else if (!_tcscmp(ListItem, _T("Yellow"))) {
		c = RGB(255, 255, 0);
	}
	else if (!_tcscmp(ListItem, _T("Orange"))) {
		c = RGB(255, 69, 0);
	}
	else if (!_tcscmp(ListItem, _T("Grey"))) {
		c = RGB(128, 128, 128);
	}
	else if (!_tcscmp(ListItem, _T("Pink"))) {
		c = RGB(255, 0, 255);
	}
	else if (!_tcscmp(ListItem, _T("Purple"))) {
		c = RGB(148, 0, 211);
	}
	else if (!_tcscmp(ListItem, _T("Black"))) {
		c = RGB(0, 0, 0);
	}
	return c;
}

void saveData() {
	ofstream f;
	f.open("file.txt");
	auto c = Points::colors.begin();
	for (auto i = Points::points.begin(); i != Points::points.end(); i++) {
		f << *c << " " << *i << " ";
		cout << "Saving Point:" << *c << " " << *i << " ";
		i++;
		c++;
		f << *i << endl;
		cout << *i << endl;
	}
	f.close();
}

void loadData(HDC hdc) {
	ifstream f("file.txt");
	if (!f.is_open())
	{
		cout << "File doesn't exist" << endl;
		return;
	}
	while (!f.eof()) {
		int x, y;
		COLORREF c;
		f >> c >> x >> y;
		Points::addPoint(hdc, x, y, c);
	}
	f.close();
}

LRESULT WINAPI MyWndProc(HWND hwnd, UINT mcode, WPARAM wp, LPARAM lp)
{
	HDC hdc;
	static HWND lineHwnd, colorHwnd;
	static HWND circleHwnd, curveHwnd;
	static HWND ellipseHwnd, fillingHwnd, clipHwnd, screenHwnd;
	static int x[100];
	static int y[100];
	static int cnt = 0;
	static Action action;
	static COLORREF c;
    static vector<Vertex> vertices;
	switch (mcode)
	{
	case WM_CREATE:
		lineHwnd = createLineDropDown(hwnd);
		circleHwnd = createCircleDropDown(hwnd);
		ellipseHwnd = createEllipseDropDown(hwnd);
		curveHwnd = createCurveDropDown(hwnd);
		fillingHwnd = createFillingDropDown(hwnd);
		clipHwnd = createClippingDropDown(hwnd);
		colorHwnd = createColorDropDown(hwnd);
		screenHwnd = createScreenDropDown(hwnd);
		break;
	case WM_COMMAND:
		if (LOWORD(wp) == 1) {
			if (HIWORD(wp) == CBN_SELCHANGE) {
				c = selectColor(lp);
			}
		}
		else if (HIWORD(wp) == CBN_SELCHANGE)
		{
			int ItemIndex = SendMessage((HWND)lp, (UINT)CB_GETCURSEL, (WPARAM)0, (LPARAM)0);
			TCHAR  ListItem[256];
			(TCHAR)SendMessage((HWND)lp, (UINT)CB_GETLBTEXT, (WPARAM)ItemIndex, (LPARAM)ListItem);
			if (!_tcscmp(ListItem, _T("DDA"))) {
				action = dDALine;
				SendMessage(lineHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Bresenham"))) {
				action = bresenhamLine;
				SendMessage(lineHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Parametric"))) {
				action = parametricLine;
				SendMessage(lineHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Direct")) && LOWORD(wp)==3) {
				action = directCircle;
				SendMessage(circleHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Polar")) && LOWORD(wp) == 3) {
				action = polarCircle;
				SendMessage(circleHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Iterative")) && LOWORD(wp) == 3) {
				action = iterativeCircle;
				SendMessage(circleHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Modified Midpoint")) && LOWORD(wp) == 3) {
				action = modifiedCircle;
				SendMessage(circleHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Midpoint")) && LOWORD(wp) == 3) {
				action = midCircle;
				SendMessage(circleHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Direct")) && LOWORD(wp) == 4) {
				action = directEllipse;
				SendMessage(ellipseHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Polar")) && LOWORD(wp) == 4) {
				action = polarEllipse;
				SendMessage(ellipseHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Midpoint")) && LOWORD(wp) == 4) {
				action = midEllipse;
				SendMessage(ellipseHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Cardinal Splines"))) {
				action = splines;
				SendMessage(curveHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("fill square with hermite"))) {
				action = squareHermite;
				SendMessage(curveHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("fill rec with bezier"))) {
				action = recBezier;
				SendMessage(curveHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Circle with Lines"))) {
				action = fillCircleWithLines;
				SendMessage(fillingHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Circle with Circles"))) {
				action = fillCircleWithCircles;
				SendMessage(fillingHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Convex Filling"))) {
				action = convexFill;
				SendMessage(fillingHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Non-Convex Filling"))) {
				action = nonConvexFill;
				SendMessage(fillingHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Recursive Flood Fill"))) {
				action = recursiveFlood;
				SendMessage(fillingHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Non-Recursive Flood Fill"))) {
				action = nonRecursiveFlood;
				SendMessage(fillingHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Rectangle, Point"))) {
				action = clipRecPoint;
				SendMessage(clipHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Rectangle, Line"))) {
				action = clipRecLine;
				SendMessage(clipHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Rectangle, Polygon"))) {
				action = clipRecPolygon;
				SendMessage(clipHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}

			else if (!_tcscmp(ListItem, _T("Square, Point"))) {
				action = clipSquarePoint;
				SendMessage(clipHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Square, Line"))) {
				action = clipSquareLine;
				SendMessage(clipHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Circle, Point"))) {
				action = clipCirclePoint;
				SendMessage(clipHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Circle, Line"))) {
				action = clipCircleLine;
				SendMessage(clipHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Clear"))) {
				InvalidateRect(hwnd, NULL, TRUE);
				Points::clear();
				cout << "Window is Cleared" << endl;
				SendMessage(screenHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Load Data"))) {
				hdc = GetDC(hwnd);
				loadData(hdc);
				ReleaseDC(hwnd, hdc);
				cout << "Data is Loaded" << endl;
				SendMessage(screenHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			else if (!_tcscmp(ListItem, _T("Save Data"))) {
				cout << "Data is Saved" << endl;
				saveData();
				SendMessage(screenHwnd, CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
			}
			cnt = 0;
		}
		break;
	case WM_LBUTTONDOWN:
		hdc = GetDC(hwnd);
		x[cnt] = LOWORD(lp);
		y[cnt++] = HIWORD(lp);
		cout << "Clicked point: " << x[cnt - 1] << " " << y[cnt - 1] << endl;
		if (cnt == 2 && action == directCircle) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			DirectCircle(hdc, x[0], y[0], r, c);
			cnt = 0;
			cout << "Drawing direct circle" << endl;
		}
		else if (cnt == 2 && action == dDALine) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			DDALine(hdc, x[0], y[0], x[1], y[1], c);
			cnt = 0;
			cout << "Drawing dda line" << endl;
		}
		else if (cnt == 2 && action == parametricLine) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			ParametricLine(hdc, x[0], y[0], x[1], y[1], c);
			cnt = 0;
			cout << "Drawing parametric line" << endl;
		}
		else if (cnt == 2 && action == bresenhamLine) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			BresenhamLine(hdc, x[0], y[0], x[1], y[1], c);
			cnt = 0;
			cout << "Drawing Bresenham Line" << endl;
		}
		else if (cnt == 2 && action == polarCircle) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			PolarCircle(hdc, x[0], y[0], r, c);
			cnt = 0;
			cout << "Drawing polar circle" << endl;
		}
		else if (cnt == 2 && action == iterativeCircle) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			IterativeCircle(hdc, x[0], y[0], r, c);
			cnt = 0;
			cout << "Drawing iterative circle" << endl;
		}
		else if (cnt == 2 && action == midCircle) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			MidpointCircle(hdc, x[0], y[0], r, c);
			cnt = 0;
		}
		else if (cnt == 2 && action == modifiedCircle) {
			int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			modifiedMidpointCircle(hdc, x[0], y[0], r, c);
			cnt = 0;
			cout << "Drawing modified midpoint circle" << endl;
		}
		else if (cnt == 3 && action == directEllipse) {
			int a = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			int b = sqrt((x[0] - x[2]) * (x[0] - x[2]) + (y[0] - y[2]) * (y[0] - y[2]));
			DirectEllipse(hdc, x[0], y[0], a, b, c);
			cnt = 0;
			cout << "Drawing direct ellipse" << endl;
		}
		else if (cnt == 3 && action == polarEllipse) {
			int a = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			int b = sqrt((x[0] - x[2]) * (x[0] - x[2]) + (y[0] - y[2]) * (y[0] - y[2]));
			PolarEllipse(hdc, x[0], y[0], a, b, c);
			cnt = 0;
			cout << "Drawing polar ellipse" << endl;
		}
		else if (cnt == 3 && action == midEllipse) {
			int a = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			int b = sqrt((x[0] - x[2]) * (x[0] - x[2]) + (y[0] - y[2]) * (y[0] - y[2]));
			MidEllipse(hdc, x[0], y[0], a, b, c);
			cnt = 0;
			cout << "Drawing midpoint ellipse" << endl;
		}
		else if (cnt == 2 && action == squareHermite) {
			int length = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			drawSquare(hdc, x[0], y[0], length, c);
			cnt = 0;
			cout << "Filling square with hermite" << endl;
		}
		else if (cnt == 3 && action == recBezier) {
			int width = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
			int height = sqrt((x[0] - x[2]) * (x[0] - x[2]) + (y[0] - y[2]) * (y[0] - y[2]));
			drawRec(hdc, x[0], y[0], width, height, c);
			cnt = 0;
			cout << "Filling rectangle with hermite" << endl;
		}
		else if(action == clipRecLine)
        {
		    if(cnt == 1)
                rectangular_window(hdc, x[0], y[0], 400, 200, c); // Draw a rectangular window

             else if(cnt == 3)
            {
                float x1 = (float)x[1];
                float y1 = (float)y[1];
                float x2 = (float)x[2];
                float y2 = (float)y[2];
                if(line_clipping_rect(x[0], x[0] + 400, y[0], y[0] + 200, x1, y1, x2, y2))
                    BresenhamLine(hdc, x1, y1, x2, y2, c);
                cnt = 1;
            }

             cnt = 0;
             cout << "Clipping line in rectangular window" << endl;
        }

        else if(action == clipSquareLine)
        {
            if(cnt == 1)
                rectangular_window(hdc, x[0], y[0], 300, 300, c); // Draw a square window

            else if(cnt == 3)
            {
                float x1 = (float)x[1];
                float y1 = (float)y[1];
                float x2 = (float)x[2];
                float y2 = (float)y[2];
                if(line_clipping_rect(x[0], x[0] + 300, y[0], y[0] + 300, x1, y1, x2, y2))
                    BresenhamLine(hdc, x1, y1, x2, y2, c);
                cnt = 1;
            }
            cnt = 0;
            cout << "Clipping line in square window" << endl;
        }

        else if(action == clipCircleLine)
        {
            int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
            if(cnt == 2)
            {

                PolarCircle(hdc, x[0], y[0], r, c); // Draw a circular window
            }

            else if(cnt == 4)
            {
                float x1 = (float)x[2];
                float y1 = (float)y[2];
                float x2 = (float)x[3];
                float y2 = (float)y[3];

                line_clipping_circle(hdc, x[0], y[0], r, x1, y1, x2, y2, c);
                cnt = 2;
            }
            cnt = 0;
            cout << "Clipping line in circular window" << endl;
        }

        else if(action == clipRecPoint)
        {
            if(cnt == 1)
                rectangular_window(hdc, x[0], y[0], 400, 200, c); // Draw a rectangular window

            else if(cnt == 2)
            {
                point_clipping_rectangular(hdc, x[0], x[0] + 400, y[0], y[0] + 200, x[1], y[1], c);
                cnt = 1;
            }
            cnt = 0;
            cout << "Clipping point in rectangular window" << endl;
        }

        else if(action == clipSquarePoint)
        {
            if(cnt == 1)
                rectangular_window(hdc, x[0], y[0], 300, 300, c); // Draw a square window

            else if(cnt == 2)
            {
                point_clipping_rectangular(hdc, x[0], x[0] + 300, y[0], y[0] + 300, x[1], y[1], c);
                cnt = 1;
            }
            cnt = 0;
            cout << "Clipping point in square window" << endl;
        }

        else if(action == clipCirclePoint)
        {
            int r = sqrt((x[0] - x[1]) * (x[0] - x[1]) + (y[0] - y[1]) * (y[0] - y[1]));
            if(cnt == 2)
            {

                PolarCircle(hdc, x[0], y[0], r, c); // Draw a circular window
            }

            else if(cnt == 3)
            {
                point_clipping_circular(hdc, x[0], y[0], r, x[2], y[2], c);
                cnt = 2;
            }
            cnt = 0;
            cout << "Clipping point in circular window" << endl;
        }
        else if(action == clipRecPolygon)
        {
            if(cnt == 1)
                rectangular_window(hdc, x[0], y[0], 400, 200, c); // Draw a rectangular window

            else if(cnt == 3) // Store the vertices of the first side of the polygon
            {
                //BresenhamLine(hdc, x[1], y[1], x[2], y[2], c);

                // Store the end-points of the first side in the vertex vector
                vertices.push_back({x[1], y[1]});
                vertices.push_back({x[2], y[2]});
            }

            else if(cnt > 3) // Store the vertices the next sides, each side's first end-point is the previous side's second end-point
            {
                //BresenhamLine(hdc, x[cnt - 2], y[cnt - 2], x[cnt - 1], y[cnt - 1], c);

                // Store every second end-point
                vertices.push_back({x[cnt - 1], y[cnt - 1]});
            }

        }
            ReleaseDC(hwnd, hdc);
		break;
	case WM_RBUTTONDOWN:
		hdc = GetDC(hwnd);
		if(cnt>=4 && action==splines){
			Spline(hdc, x, y, cnt, c);
			cnt = 0;
			cout << "Drawing cardinal splines curves" << endl;
		}

		else if(action == clipRecPolygon)
        {
            //BresenhamLine(hdc, x[cnt - 1], y[cnt - 1], x[1], y[1], c);

            vector<Vertex> polygon = polygon_clipping_rect(vertices, x[0], x[0] + 400, y[0], y[0] + 200);

            // Draw the sides of the clipped polygon except the last one.
            for(int i = 0; i < polygon.size() - 1; i++)
            {
                BresenhamLine(hdc, polygon[i].first, polygon[i].second, polygon[i + 1].first, polygon[i + 1].second, c);
            }

            // Draw the last side of the clipped polygon
            BresenhamLine(hdc, polygon[polygon.size() - 1].first, polygon[polygon.size() - 1].second, polygon[0].first, polygon[0].second, c);

            vertices.clear(); // Clear the vertices vector
		    cnt = 0;
        }
		ReleaseDC(hwnd, hdc);
		break;
	/*case WM_SETCURSOR:
		if (LOWORD(lp) == HTCLIENT)
		{
			hCursor = LoadCursor(NULL, IDC_CROSS);
			SetCursor(hCursor);
			return TRUE;
		}
		break;*/
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default: return DefWindowProc(hwnd, mcode, wp, lp);
	}
	return 0;
}
int APIENTRY WinMain(HINSTANCE hinst, HINSTANCE pinst, LPSTR cmd, int nsh)
{
	WNDCLASS wc;
	wc.cbClsExtra = wc.cbWndExtra = 0;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.hCursor = LoadCursor(NULL, IDC_HAND);
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hInstance = hinst;
	wc.lpfnWndProc = MyWndProc;
	wc.lpszClassName = TEXT("MyClass");
	wc.lpszMenuName = NULL;
	wc.style = CS_HREDRAW | CS_VREDRAW;
	RegisterClass(&wc);
	HWND hwnd = CreateWindow(TEXT("MyClass"), TEXT("2D Package"), WS_OVERLAPPEDWINDOW, 200, 100, 1020, 600, NULL, NULL, hinst, 0);
	ShowWindow(hwnd, nsh);
	UpdateWindow(hwnd);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;
}
