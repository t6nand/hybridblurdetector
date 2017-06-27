#ifndef DISTANCE_UTILS_H
#define DISTANCE_UTILS_H
#include <cmath>

static double manhattan_distance(double px, double py, double qx, double qy, int w, int h)
{
    return (fabs(px - qx) / w) + (fabs(py - qy) / h);
}

static double min_manhattan_distance(double x, double y, double *p, int n, int w, int h)
{
    double min = INFINITY, j;
    for (int i = 0; i < n; i++) {
        j = manhattan_distance(x, y, p[2 * i], p[2 * i + 1], w, h);
        if (j < min) min = j;
    }
    return min;
}

#endif // !DISTANCE_UTILS_H
