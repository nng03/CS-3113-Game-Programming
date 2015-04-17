//
//  Matrix.h
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Matrix__
#define __NYUCodebase__Matrix__

#include <iostream>

class Vector;

class Matrix
{
public:
    Matrix();
    
    union {
        float m[4][4];
        float ml[16];
    };
    
    void identity();
    Matrix inverse();
    Matrix operator*(const Matrix &mat2);
    Vector operator*(const Vector &vec2);
    
};

#endif /* defined(__NYUCodebase__Matrix__) */
