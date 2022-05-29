//
// Created by Rana Hisham on 27-May-22.
//

typedef pair<int,int> Vertex;

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

bool in_left(Vertex vertex, int left) // Return true if a vertex is inside the left edge of the window
{
    return vertex.first >= left;

}
bool in_right(Vertex vertex, int right) // Return true if a vertex is inside the right edge of the window
{
    return vertex.first <= right;
}

bool in_bottom(Vertex vertex, int bottom) // Return true if a vertex is inside the bottom edge of the window
{
    return vertex.second >= bottom;
}

bool in_top(Vertex vertex, int top) // Return true if a vertex is inside the top edge of the window
{
    return vertex.second <= top;
}

vector<Vertex> clip_left(vector<Vertex> polygon, int left)
{
    vector<Vertex> outlist; // New polygon that is left-clipped
    Vertex vertex1 = polygon[polygon.size() - 1]; // Set vertex1 to last vertex in the polygon
    bool in1 = in_left(vertex1, left); // True if vertex1 is inside the left edge of the window, false otherwise

    for(int i = 0; i < polygon.size(); i++)
    {
        // For each vertex in the polygon check whether it's inside or outside the left edge of the window
        Vertex vertex2 = polygon[i];
        bool in2 = in_left(vertex2, left);

        // If two adjacent vertices are inside the left edge of the window, push the second vertex to the new polygon
        if(in1 && in2)
            outlist.push_back(vertex2);
        else if(in1) // If an inside vertex is followed by an outside vertex, find the intersection point of that side with the left edge
        {
            float x,y;
            vertical_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, left, x, y);
            outlist.push_back({x,y}); // Push the intersection point to the new polygon
        }
        else if(in2) // If an outside vertex is followed by an inside vertex, find the intersection point of that side with the left edge
        {
            float x,y;
            vertical_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, left, x, y);
            outlist.push_back({x,y}); // Push the intersection point to the new polygon
            outlist.push_back(vertex2); // Push the second vertex to the new polygon
        }
        vertex1 = vertex2;
        in1 = in2;
    }
    return outlist; // Return the left-clipped polygon
}

vector<Vertex> clip_right(vector<Vertex> polygon, int right)
{
    vector<Vertex> outlist;
    Vertex vertex1 = polygon[polygon.size() - 1];
    bool in1 = in_right(vertex1, right);

    for(int i = 0; i < polygon.size(); i++)
    {
        Vertex vertex2 = polygon[i];
        bool in2 = in_right(vertex2, right);
        if(in1 && in2)
            outlist.push_back(vertex2);
        else if(in1)
        {
            float x,y;
            vertical_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, right, x, y);
            outlist.push_back({x,y});
        }
        else if(in2)
        {
            float x,y;
            vertical_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, right, x, y);
            outlist.push_back({x,y});
            outlist.push_back(vertex2);
        }
        in1 = in2;
        vertex1 = vertex2;
    }
    return outlist;
}


vector<Vertex> clip_bottom(vector<Vertex> polygon, int bottom)
{
    vector<Vertex> outlist;
    Vertex vertex1 = polygon[polygon.size() - 1];
    bool in1 = in_bottom(vertex1, bottom);

    for(int i = 0; i < polygon.size(); i++)
    {
        Vertex vertex2 = polygon[i];
        bool in2 = in_bottom(vertex2, bottom);
        if(in1 && in2)
            outlist.push_back(vertex2);
        else if(in1)
        {
            float x,y;
            horizontal_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, bottom, x, y);
            outlist.push_back({x,y});
        }
        else if(in2)
        {
            float x,y;
            horizontal_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, bottom, x, y);
            outlist.push_back({x,y});
            outlist.push_back(vertex2);
        }
        vertex1 = vertex2;
        in1 = in2;
    }
    return outlist;
}

vector<Vertex> clip_top(vector<Vertex> polygon, int top)
{
    vector<Vertex> outlist;
    Vertex vertex1 = polygon[polygon.size() - 1];
    bool in1 = in_top(vertex1, top);

    for(int i = 0; i < polygon.size(); i++)
    {
        Vertex vertex2 = polygon[i];
        bool in2 = in_top(vertex2, top);
        if(in1 && in2)
            outlist.push_back(vertex2);
        else if(in1)
        {
            float x,y;
            horizontal_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, top, x, y);
            outlist.push_back({x,y});
        }
        else if(in2)
        {
            float x,y;
            horizontal_intercept(vertex1.first,vertex1.second,vertex2.first,vertex2.second, top, x, y);
            outlist.push_back({x,y});
            outlist.push_back(vertex2);
        }
        vertex1 = vertex2;
        in1 = in2;
    }
    return outlist;
}
vector<Vertex> polygon_clipping_rect(vector<Vertex> polygon, int left, int right, int bottom, int top)
{
    vector<Vertex> clipped_polygon = clip_bottom(polygon, bottom);
    clipped_polygon = clip_left(clipped_polygon, left);
    clipped_polygon = clip_top(clipped_polygon, top);
    clipped_polygon = clip_right(clipped_polygon, right);
    return clipped_polygon;
}