//
// Created by Rana Hisham on 27-May-22.
//


/* Union data structure to store the region code of an end-point. */
union region_code
{
    unsigned All: 4;

    struct {
        unsigned left : 1;
        unsigned right : 1;
        unsigned bottom : 1;
        unsigned top : 1;
    };
};


void rectangular_window(HDC hdc, int x1, int y1, int w, int h, COLORREF c) {
    int y2 = y1 + h, x2 = x1 + w;
    BresenhamLine(hdc, x1, y1, x1, y2, c);
    BresenhamLine(hdc, x1, y2, x2, y2, c);
    BresenhamLine(hdc, x2, y2, x2, y1, c);
    BresenhamLine(hdc, x2, y1, x1, y1, c);

}

/* Finds the region code of a point, given the co-ordinates of the point and the window edges. */
region_code get_region_code(float x, float y, float left, float right, float bottom, float top)
{
    region_code r;
    r.All = 0;
    if(x < left)
        r.left = 1;
    else if(x > right)
        r.right = 1;
    if(y < bottom)
        r.bottom = 1;
    else if(y > top)
        r.top = 1;

    return r;
}

/* Finds the intersection between a given point and a vertical line */
void vertical_intercept(float x1, float y1, float x2, float y2, float xedge, float& xintercept, float& yintercept)
{
    yintercept = y1 + (y2 - y1)/(x2 - x1) * (xedge - x1);
    xintercept = xedge;
}

/* Finds the intersection between a given point and a horizontal line */
void horizontal_intercept(float x1, float y1, float x2, float y2, float yedge, float& xintercept, float& yintercept)
{
    xintercept = x1 + (x2 - x1) * (yedge - y1)/(y2 - y1);
    yintercept = yedge;
}


/* Cohen-Sutherland algorithm returns true if line is completely inside the window,
 * returns false if line is completely outside the window.
 */
bool line_clipping_rect(float left, float right, float bottom, float top, float& x1, float& y1, float& x2, float& y2)
{

    region_code p1 = get_region_code(x1, y1, left, right, bottom, top);
    region_code p2 = get_region_code(x2, y2, left, right, bottom, top);

    while(true) {
        if (!(p1.All | p2.All)) // If p1 and p2 are inside the window
            return true;
        if ((p1.All & p2.All) != 0) // If p1 and p2 are outside the window and have a common region
            return false;
        // Case where it's unclear if the line is partially inside the window or completely outside the window
        if (p1.All)
        {
            if (p1.left) // If first end-point is in the left region
                vertical_intercept(x1, y1, x2, y2, left, x1, y1);
            else if (p1.right) // If first end-point is in the right region
                vertical_intercept(x1, y1, x2, y2, right, x1, y1);
            else if (p1.bottom) // If first end-point is in the bottom region
                horizontal_intercept(x1, y1, x2, y2, bottom, x1, y1);
            else // If first end-point is in the top region
                horizontal_intercept(x1, y1, x2, y2, top, x1, y1);

            p1 = get_region_code(x1, y1, left, right, bottom, top); // Get the new region code for the first end-point point after updating co-ordinates
        }

        else
        {
            if (p2.left) // If end-point is in the left region
                vertical_intercept(x1, y1, x2, y2, left, x2, y2);
            else if (p2.right) // If end-point is in the right region
                vertical_intercept(x1, y1, x2, y2, right, x2, y2);
            else if (p2.bottom) // If end-point is in the bottom region
                horizontal_intercept(x1, y1, x2, y2, bottom, x2, y2);
            else // If end-point is in the top region
                horizontal_intercept(x1, y1, x2, y2, top, x2, y2);

            p2 = get_region_code(x2, y2, left, right, bottom, top); // Get the new region code for the end-point point after updating co-ordinates

        }
    }
}


void line_clipping_circle(HDC hdc, int xc, int yc, int r, float& x1, float& y1, float& x2, float& y2, COLORREF c)
{
    double x, y;
    for (double i = 0; i <= 1; i += 0.001) {
        x = x1 + i * (x2 - x1);
        y = y1 + i * (y2 - y1);
        double diff_x = (xc - x);
        double diff_y = (yc - y);
        if(diff_x * diff_x + diff_y * diff_y > r * r)
            continue;
        Points::addPoint(hdc, Round(x), Round(y), c);
    }

}

void point_clipping_rectangular(HDC hdc, float left, float right, float bottom, float top, float x, float y, COLORREF c)
{
    if(x >= left && x <= right && y >= bottom && y <= top)
        Points::addPoint(hdc, Round(x), Round(y), c);
}

void point_clipping_circular(HDC hdc, int xc, int yc, int r, int x, int y, COLORREF c)
{
    int diff_x = (xc - x);
    int diff_y = (yc - y);
    if(diff_x * diff_x + diff_y * diff_y < r * r)
        Points::addPoint(hdc, Round(x), Round(y), c);
}