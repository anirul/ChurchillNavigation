#pragma once

#include "point_search.h"

Rect MakeRect(float lx, float ly, float hx, float hy);
bool RectContains(const Rect& r, float x, float y);
bool RectCompletelyContains(const Rect& out, const Rect& in);
bool RectIntersects(const Rect& l, const Rect& r);
Rect RectExtend(const Rect& r, float x, float y);
