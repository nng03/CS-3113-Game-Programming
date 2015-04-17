//
//  Vector.cpp
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "Vector.h"
#include <cmath>

Vector::Vector()
{
    
}

Vector::Vector(float x, float y, float z)
{
    x = x;
    y = y;
    z = z;
}

float Vector::length()
{
    return std::sqrt((x * x) + (y * y) + (z * z));
}

void Vector::normalize()
{
    float length = this->length();
    
    x = x / length;
    y = y / length;
    z = z / length;
}