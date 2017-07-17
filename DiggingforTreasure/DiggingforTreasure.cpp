//Defines the entry point for the console application.
#include <iostream>
#include <vector>
#include <stack>
#include <math.h>
using namespace std;

/*
This function reads a line of int values into a vector function and returns that
vector.
*/
vector<int> readlineofints(int count) {
    // Input values
    vector<int> linevalues(count);
    for (int j = 0; j < count; j++) {
        int val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}

vector<long> readlineoflongs(int count) {
    // Input values
    vector<long> linevalues(count);
    for (int j = 0; j < count; j++) {
        long val;
        cin >> val;
        linevalues[j] = val;
    }
    return linevalues; // Return line values as a vector
}

/*This code calculates a determinant of order n
I took it from http://www.sanfoundry.com/cpp-program-find-determinant-given-matrix/
It requires math.h */
double det(int n, double mat[10][10]) {
    double d = 0;
    int c, subi, i, j, subj;
    double submat[10][10];
    if (n == 2) {
        return((mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));
    }
    else {
        for (c = 0; c < n; c++) {
            subi = 0;
            for (i = 1; i < n; i++) {
                subj = 0;
                for (j = 0; j < n; j++) {
                    if (j == c) {
                        continue;
                    }
                    submat[subi][subj] = mat[i][j];
                    subj++;
                }
                subi++;
            }
            d = d + (pow(-1, c) * mat[0][c] * det(n - 1, submat));
        }
    }
    return d;
}

struct Point {
    double x;
    double y;

    //Add back default constructor
    Point() = default;

    //Constructor
    Point(double x_in, double y_in) :x(x_in), y(y_in) {};

    //Copy constructor
    Point(const Point& input) :x(input.x), y(input.y) {};
};


//Taken from http://www.geeksforgeeks.org/convex-hull-set-2-graham-scan/
// A globle point needed for  sorting points with reference
// to  the first point Used in compare function of qsort()
Point p0;

// A utility function to find next to top in a stack
Point nextToTop(stack<Point> &S) {
    Point p = S.top();
    S.pop();
    Point res = S.top();
    S.push(p);
    return res;
}

// A utility function to swap two points
int swap(Point &p1, Point &p2) {
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

// A utility function to return square of distance
// between p1 and p2
double distSq(Point p1, Point p2) {
    return (p1.x - p2.x)*(p1.x - p2.x) +
        (p1.y - p2.y)*(p1.y - p2.y);
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int orientation(Point p, Point q, Point r) {
    int val = (q.y - p.y) * (r.x - q.x) -
        (q.x - p.x) * (r.y - q.y);

    if (val == 0) return 0;  // colinear
    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

// A function used by library function qsort() to sort an array of
// points with respect to the first point
int compare(const void *vp1, const void *vp2) {
    Point *p1 = (Point *)vp1;
    Point *p2 = (Point *)vp2;

    // Find orientation
    int o = orientation(p0, *p1, *p2);
    if (o == 0)
        return (distSq(p0, *p2) >= distSq(p0, *p1)) ? -1 : 1;

    return (o == 2) ? -1 : 1;
}

// Finds convex hull of a set of n points.
vector<Point> convexHull(Point points[], int n) {
    // Find the bottommost point
    int ymin = points[0].y, min = 0;
    for (int i = 1; i < n; i++) {
        int y = points[i].y;

        // Pick the bottom-most or chose the left
        // most point in case of tie
        if ((y < ymin) || (ymin == y &&
            points[i].x < points[min].x))
            ymin = points[i].y, min = i;
    }

    // Place the bottom-most point at first position
    swap(points[0], points[min]);

    // Sort n-1 points with respect to the first point.
    // A point p1 comes before p2 in sorted ouput if p2
    // has larger polar angle (in counterclockwise
    // direction) than p1
    p0 = points[0];
    qsort(&points[1], n - 1, sizeof(Point), compare);

    // If two or more points make same angle with p0,
    // Remove all but the one that is farthest from p0
    // Remember that, in above sorting, our criteria was
    // to keep the farthest point at the end when more than
    // one points have same angle.
    int m = 1; // Initialize size of modified array
    for (int i = 1; i<n; i++) {
        // Keep removing i while angle of i and i+1 is same
        // with respect to p0
        while (i < n - 1 && orientation(p0, points[i],
            points[i + 1]) == 0)
            i++;


        points[m] = points[i];
        m++;  // Update size of modified array
    }

    // If modified array of points has less than 3 points,
    // convex hull is not possible
    if (m < 3) return;

    // Create an empty stack and push first three points
    // to it.
    stack<Point> S;
    S.push(points[0]);
    S.push(points[1]);
    S.push(points[2]);

    // Process remaining n-3 points
    for (int i = 3; i < m; i++) {
        // Keep removing top while the angle formed by
        // points next-to-top, top, and points[i] makes
        // a non-left turn
        while (orientation(nextToTop(S), S.top(), points[i]) != 2)
            S.pop();
        S.push(points[i]);
    }

    vector<Point> output_points;
    // Now stack has the output points, print contents of stack
    while (!S.empty()) {
        Point p = S.top();
        output_points.push_back(p);
        S.pop();
    }
    return output_points;
}

int main() {
    std::ios_base::sync_with_stdio(false);

    // get test case count
    int t;
    std::cin >> t;

    //! loop over all the test cases
    for (int i = 1; i <= t; i++) {
        // Read in params
        vector<int> params = readlineofints(1);
        long n = params[0];

        //Loop over inputs
        Point points[1000];
        for (int j = 0; j < n; j++) {
            vector<int> this_point = readlineofints(2);
            Point x = {(double)this_point[0],(double)this_point[1]};
            points[j] = x;
        }

        //Get convex hull points
        vector<Point> hull_points = convexHull(points, n);
        //Code taken from similar code at https://gist.github.com/listochkin/1200393
        double area = 0.0;
        for (int j = 0; j < hull_points.size() - 1; j++) {
            int k = (j + 1) % hull_points.size();
            area += hull_points[j].x * hull_points[k].y;
            area -= hull_points[j].y * hull_points[k].x;
        }

        //Print output
        std::cout << "Case #" << i << ": " << area << std::endl;
        
    }
    return 0;
}