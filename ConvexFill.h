
#include <tchar.h>
#include <windows.h>
#include <math.h>
#include <cmath>
#include <vector>
using namespace std;
#define MAXENTRIES 600

struct Entry {
	int xmin, xmax;
};
void InitEntries(Entry table[])
{
	int i;
	for (i = 0; i < MAXENTRIES; i++) {
		table[i].xmin = INT_MAX;
		table[i].xmax = -INT_MAX;
	}
}
void ScanEdge(POINT v1, POINT v2, Entry table[])
{
	if (v1.y == v2.y)
		return;
	if (v1.y > v2.y)
		swap(v1, v2);
	double minv = (double)(v2.x - v1.x) / (v2.y - v1.y);
	double x = v1.x;
	int y = v1.y;
	while (y < v2.y)
	{
		if (x < table[y].xmin)
			table[y].xmin = (int)ceil(x);
		if (x > table[y].xmax)
			table[y].xmax = (int)floor(x);
		y++;
		x += minv;
	}
}
void DrawScanLines(HDC hdc, Entry table[], COLORREF color)
{
	int y;
	for (y = 0; y < MAXENTRIES; y++) {
		if (table[y].xmin < table[y].xmax) {
			int x;
			for (x = table[y].xmin; x <= table[y].xmax; x++) {
				SetPixel(hdc, x, y, color);
			}
		}
	}
}
void ConvexFill(HDC hdc, POINT p[], int n, COLORREF color) {
	Entry* table = new Entry[MAXENTRIES];
	InitEntries(table);
	POINT v1 = p[n - 1];
	int i;
	for (i = 0; i < n; i++) {
		POINT v2 = p[i];
		ScanEdge(v1, v2, table);
		v1 = p[i];
	}
	DrawScanLines(hdc, table, color);
	delete table;
}
