//
//  Point.cpp
//  Strategy
//
//  Created by Paul on 15/5/9.
//

#include "Point.h"

std::ostream& operator << (std::ostream& out, const Point& pt)
{
    out << "(" << pt.x << "," << pt.y << ")";
    return out;
}
