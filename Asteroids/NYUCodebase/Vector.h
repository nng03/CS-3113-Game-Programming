//
//  Vector.h
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Vector__
#define __NYUCodebase__Vector__

#include <stdio.h>

class Vector
{
public:
    Vector();
    Vector(float x, float y, float z);
    
    float length();
    void normalize();
    
    float x;
    float y;
    float z;
};

#endif /* defined(__NYUCodebase__Vector__) */
