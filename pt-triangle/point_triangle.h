#pragma once

#include <cstdint>
#include <stdexcept>

struct Point {
    int64_t x, y;
};

struct Triangle {
    Point a, b, c;
};

int Position(Point a, Point b, Point c) { // 1 - ac on ab's left, 0 - co, -1 on ab's rigt
    int64_t x1 = b.x - a.x;
    int64_t y1 = b.y - a.y;
    int64_t x2 = c.x - a.x;
    int64_t y2 = c.y - a.y;
    int64_t vecprod = x1 * y2 - x2 * y1;
    if (vecprod > 0) {
        return 1;
    } else if (vecprod == 0) {
        return 0;
    }
    return -1;
}

bool InsideBox(Point a, Point b, Point c) {
    int x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
    if (x1 > x2) {
        std::swap(x1, x2);
    }
    if (y1 > y2) {
        std::swap(y1, y2);
    }
    return x1 <= c.x && c.x <= x2 && y1 <= c.y && c.y <= y2;
}

bool IsPointInTriangle(const Triangle& t, const Point& pt) {
    int a = Position(t.a, t.b, pt);
    int b = Position(t.b, t.c, pt);
    int c = Position(t.c, t.a, pt);

    if (a == 0) {
        return InsideBox(t.a, t.b, pt);
    }
    if (b == 0) {
        return InsideBox(t.b, t.c, pt);
    }
    if (c == 0) {
        return InsideBox(t.c, t.a, pt);
    }
    if (a * b * c == 0) {
        return true;
    }
    return a == b && b == c;
}
