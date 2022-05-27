#pragma once
#include<vector>
using namespace std;
class Points {
public:
	static vector<int> points;
	static vector<COLORREF> colors;
	static void addPoint(HDC hdc, int x, int y, COLORREF c) {
		Points::points.push_back(x);
		Points::points.push_back(y);
		Points::colors.push_back(c);
		SetPixel(hdc, x, y, c);
	}
	static void clear() {
		Points::points.clear();
		Points::colors.clear();
	}

};
vector<int> Points::points;
vector<COLORREF> Points::colors;

