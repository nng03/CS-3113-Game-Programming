//
//  Particles.h
//  NYUCodebase
//
//  Created by fp on 5/9/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Particles__
#define __NYUCodebase__Particles__

#include <stdio.h>
#include "Vector.h"

class Particle
{
public:
    Vector position;
    Vector velocity;
    float lifeTime;
    
    Particle() :lifeTime(0.0f){
        velocity = Vector(0.0f, 0.0f, 0.0f);
        position = Vector(0.0f, 0.0f, 0.0f);
    };
};

class ParticleEmitter
{
public:
    
};

#endif /* defined(__NYUCodebase__Particles__) */
