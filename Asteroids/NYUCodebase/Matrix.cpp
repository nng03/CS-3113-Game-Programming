//
//  Matrix.cpp
//  NYUCodebase
//
//  Created by fp on 4/14/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include "Matrix.h"
#include "Vector.h"

Matrix::Matrix()
{
    
}

void Matrix::identity()
{
    m[0][0] = 1;
    m[0][1] = 0;
    m[0][2] = 0;
    m[0][3] = 0;
    m[1][0] = 0;
    m[1][1] = 1;
    m[1][2] = 0;
    m[1][3] = 0;
    m[2][0] = 0;
    m[2][1] = 0;
    m[2][2] = 1;
    m[2][3] = 0;
    m[3][0] = 0;
    m[3][1] = 0;
    m[3][2] = 0;
    m[3][3] = 1;
}

Matrix Matrix::inverse()
{
    float m00 = m[0][0], m01 = m[0][1], m02 = m[0][2], m03 = m[0][3];
    float m10 = m[1][0], m11 = m[1][1], m12 = m[1][2], m13 = m[1][3];
    float m20 = m[2][0], m21 = m[2][1], m22 = m[2][2], m23 = m[2][3];
    float m30 = m[3][0], m31 = m[3][1], m32 = m[3][2], m33 = m[3][3];
    
    float v0 = m20 * m31 - m21 * m30;
    float v1 = m20 * m32 - m22 * m30;
    float v2 = m20 * m33 - m23 * m30;
    float v3 = m21 * m32 - m22 * m31;
    float v4 = m21 * m33 - m23 * m31;
    float v5 = m22 * m33 - m23 * m32;
    
    float t00 = + (v5 * m11 - v4 * m12 + v3 * m13);
    float t10 = - (v5 * m10 - v2 * m12 + v1 * m13);
    float t20 = + (v4 * m10 - v2 * m11 + v0 * m13);
    float t30 = - (v3 * m10 - v1 * m11 + v0 * m12);
    
    float det = 1 / (t00 * m00 + t10 * m01 + t20 * m02 + t30 * m03);
    
    float d00 = t00 * det;
    float d10 = t10 * det;
    float d20 = t20 * det;
    float d30 = t30 * det;
    
    float d01 = - (v5 * m01 - v4 * m02 + v3 * m03) * det;
    float d11 = + (v5 * m00 - v2 * m02 + v1 * m03) * det;
    float d21 = - (v4 * m00 - v2 * m01 + v0 * m03) * det;
    float d31 = + (v3 * m00 - v1 * m01 + v0 * m02) * det;
    
    v0 = m10 * m31 - m11 * m30;
    v1 = m10 * m32 - m12 * m30;
    v2 = m10 * m33 - m13 * m30;
    v3 = m11 * m32 - m12 * m31;
    v4 = m11 * m33 - m13 * m31;
    v5 = m12 * m33 - m13 * m32;
    
    float d02 = + (v5 * m01 - v4 * m02 + v3 * m03) * det;
    float d12 = - (v5 * m00 - v2 * m02 + v1 * m03) * det;
    float d22 = + (v4 * m00 - v2 * m01 + v0 * m03) * det;
    float d32 = - (v3 * m00 - v1 * m01 + v0 * m02) * det;
    
    v0 = m21 * m10 - m20 * m11;
    v1 = m22 * m10 - m20 * m12;
    v2 = m23 * m10 - m20 * m13;
    v3 = m22 * m11 - m21 * m12;
    v4 = m23 * m11 - m21 * m13;
    v5 = m23 * m12 - m22 * m13;
    
    float d03 = - (v5 * m01 - v4 * m02 + v3 * m03) * det;
    float d13 = + (v5 * m00 - v2 * m02 + v1 * m03) * det;
    float d23 = - (v4 * m00 - v2 * m01 + v0 * m03) * det;
    float d33 = + (v3 * m00 - v1 * m01 + v0 * m02) * det;
    
    Matrix returnMat;
    
    returnMat.m[0][0] = d00;
    returnMat.m[0][1] = d01;
    returnMat.m[0][2] = d02;
    returnMat.m[0][3] = d03;
    returnMat.m[1][0] = d10;
    returnMat.m[1][1] = d11;
    returnMat.m[1][2] = d12;
    returnMat.m[1][3] = d13;
    returnMat.m[2][0] = d20;
    returnMat.m[2][1] = d21;
    returnMat.m[2][2] = d22;
    returnMat.m[2][3] = d23;
    returnMat.m[3][0] = d30;
    returnMat.m[3][1] = d31;
    returnMat.m[3][2] = d32;
    returnMat.m[3][3] = d33;
    
    return returnMat;
}

Matrix Matrix::operator*(const Matrix &m2)
{
    Matrix res;
    
    res.m[0][0] = (m[0][0] * m2.m[0][0]) + (m[1][0] * m2.m[0][1]) + (m[2][0] * m2.m[0][2]) + (m[3][0] * m2.m[0][3]);
    res.m[0][1] = (m[0][1] * m2.m[0][0]) + (m[1][1] * m2.m[0][1]) + (m[2][1] * m2.m[0][2]) + (m[3][1] * m2.m[0][3]);
    res.m[0][2] = (m[0][2] * m2.m[0][0]) + (m[1][2] * m2.m[0][1]) + (m[2][2] * m2.m[0][2]) + (m[3][2] * m2.m[0][3]);
    res.m[0][3] = (m[0][3] * m2.m[0][0]) + (m[1][3] * m2.m[0][1]) + (m[2][3] * m2.m[0][2]) + (m[3][3] * m2.m[0][3]);
    
    res.m[1][0] = (m[0][0] * m2.m[1][0]) + (m[1][0] * m2.m[1][1]) + (m[2][0] * m2.m[1][2]) + (m[3][0] * m2.m[1][3]);
    res.m[1][1] = (m[0][1] * m2.m[1][0]) + (m[1][1] * m2.m[1][1]) + (m[2][1] * m2.m[1][2]) + (m[3][1] * m2.m[1][3]);
    res.m[1][2] = (m[0][2] * m2.m[1][0]) + (m[1][2] * m2.m[1][1]) + (m[2][2] * m2.m[1][2]) + (m[3][2] * m2.m[1][3]);
    res.m[1][3] = (m[0][3] * m2.m[1][0]) + (m[1][3] * m2.m[1][1]) + (m[2][3] * m2.m[1][2]) + (m[3][3] * m2.m[1][3]);
    
    res.m[2][0] = (m[0][0] * m2.m[2][0]) + (m[1][0] * m2.m[2][1]) + (m[2][0] * m2.m[2][2]) + (m[3][0] * m2.m[2][3]);
    res.m[2][1] = (m[0][1] * m2.m[2][0]) + (m[1][1] * m2.m[2][1]) + (m[2][1] * m2.m[2][2]) + (m[3][1] * m2.m[2][3]);
    res.m[2][2] = (m[0][2] * m2.m[2][0]) + (m[1][2] * m2.m[2][1]) + (m[2][2] * m2.m[2][2]) + (m[3][2] * m2.m[2][3]);
    res.m[2][3] = (m[0][3] * m2.m[2][0]) + (m[1][3] * m2.m[2][1]) + (m[2][3] * m2.m[2][2]) + (m[3][3] * m2.m[2][3]);
    
    res.m[3][0] = (m[0][0] * m2.m[3][0]) + (m[1][0] * m2.m[3][1]) + (m[2][0] * m2.m[3][2]) + (m[3][0] * m2.m[3][3]);
    res.m[3][1] = (m[0][1] * m2.m[3][0]) + (m[1][1] * m2.m[3][1]) + (m[2][1] * m2.m[3][2]) + (m[3][1] * m2.m[3][3]);
    res.m[3][2] = (m[0][2] * m2.m[3][0]) + (m[1][2] * m2.m[3][1]) + (m[2][2] * m2.m[3][2]) + (m[3][2] * m2.m[3][3]);
    res.m[3][3] = (m[0][3] * m2.m[3][0]) + (m[1][3] * m2.m[3][1]) + (m[2][3] * m2.m[3][2]) + (m[3][3] * m2.m[3][3]);
    
    return res;}

Vector Matrix::operator*(const Vector &vec2)
{
    Vector returnV;
    
    returnV.x = (m[0][0] * vec2.x) + (m[1][0] * vec2.y) + (m[2][0] * vec2.z) + m[3][0];
    returnV.y = (m[0][1] * vec2.x) + (m[1][1] * vec2.y) + (m[2][1] * vec2.z) + m[3][1];
    returnV.z = (m[0][2] * vec2.x) + (m[1][2] * vec2.y) + (m[2][2] * vec2.z) + m[3][2];

    return returnV;
}