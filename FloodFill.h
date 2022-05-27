#pragma once

#include "math.h"
#include <stack>
using namespace std;
void recursiveFloodFill(HDC hdc, int x, int y, COLORREF borderC, COLORREF fillC)
{
    COLORREF C = GetPixel(hdc, x, y);
    if (C == borderC || C ==  fillC)return;
    SetPixel(hdc, x, y, fillC);
    recursiveFloodFill(hdc, x + 1, y, borderC, fillC);
    recursiveFloodFill(hdc, x - 1, y, borderC, fillC);
    recursiveFloodFill(hdc, x, y + 1, borderC, fillC);
    recursiveFloodFill(hdc, x, y - 1, borderC, fillC);
}

struct Vertex
{
    int x, y;
    Vertex(int x, int y) :x(x), y(y)
    {
    }
};
void NRFloodFill(HDC hdc, int x, int y, COLORREF borderC, COLORREF fillC)
{
    stack<Vertex> S;
    S.push(Vertex(x, y));
    while (!S.empty())
    {
        Vertex v = S.top();
        S.pop();
        COLORREF c = GetPixel(hdc, v.x, v.y);
        if (c == borderC || c == fillC)continue;
        SetPixel(hdc, v.x, v.y, fillC);
        S.push(Vertex(v.x, v.y - 1));
        S.push(Vertex(v.x, v.y + 1));
        S.push(Vertex(v.x + 1, v.y));
        S.push(Vertex(v.x - 1, v.y));


    }
}

