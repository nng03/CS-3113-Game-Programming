//
//  Entity.h
//  NYUCodebase
//
//  Created by fp on 2/16/15.
//

#ifndef __NYUCodebase__Entity__
#define __NYUCodebase__Entity__

class Entity
{
public:
    Entity();
    
    void draw() const;
    
    float x;
    float y;
    float rotation;
    
    int textureID;
    
    float width;
    float height;
    
    float directionx;
    float directiony;
    
    int score;
};

#endif /* defined(__NYUCodebase__Entity__) */
