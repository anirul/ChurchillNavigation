#include "rect_util.h"

#include <algorithm>

Rect MakeRect(float lx, float ly, float hx, float hy) {
    return Rect{ lx, ly, hx, hy };
}

Rect RectExtend(const Rect& r, float x, float y) {
    Rect result = r;
    result.lx = std::min(r.lx, x);
    result.ly = std::min(r.ly, y);
    result.hx = std::max(r.hx, x);
    result.hy = std::max(r.hy, y);
    return result;
}

bool RectContains(const Rect& rect, float x, float y) {
    return 
        (x > rect.lx || std::abs(x - rect.lx) < 1e-9) &&
        (x < rect.hx || std::abs(x - rect.hx) < 1e-9) &&
        (y > rect.ly || std::abs(y - rect.ly) < 1e-9) &&
        (y < rect.hy || std::abs(y - rect.hy) < 1e-9);
}

bool RectIntersects(const Rect& a, const Rect& b) {
    return 
        (a.lx < b.hx || std::abs(a.lx - b.hx) < 1e-9) &&
        (a.hx > b.lx || std::abs(a.hx - b.lx) < 1e-9) &&
        (a.ly < b.hy || std::abs(a.ly - b.hy) < 1e-9) &&
        (a.hy > b.ly || std::abs(a.hy - b.ly) < 1e-9);
}
