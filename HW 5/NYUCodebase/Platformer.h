//
//  Platformer.h
//  NYUCodebase
//
//  Created by fp on 3/11/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__Platformer__
#define __NYUCodebase__Platformer__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include <iostream>
#include <vector>
#include <fstream>

#include "Entity.h"

#define GRAVITY -4
#define COLLISION_OFFSET 0.001f
#define MAX_TIMESTEP 6
#define FIXED_TIMESTEP 0.0166666f
#define TILE_SIZE 0.1f
#define SPRITE_COUNT_X 2
#define SPRITE_COUNT_Y 3

class Platformer
{
public:
    Platformer();
    ~Platformer();
    
    void init();
    void buildWorld();
    bool updateAndRender();
    
    void render();
    void update(float elapsed);
    void fixedUpdate();
    
    void loadMap();
    bool readHeader(std::ifstream &stream);
    bool readLayerData(std::ifstream &stream);
    bool readEntityData(std::ifstream &stream);
    void placeEntity(std::string type, float placeX, float placeY);
    
    void worldToTileCoords(float worldX, float worldY, int* gridX, int* gridY);
    bool isSolid(unsigned char tile);
    
    GLuint loadTexture(const char* imagePath);
    void drawText(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
    
    void handleXCollision(Entity *entity);
    void handleYCollision(Entity *entity);
    float checkXCoordCollision(float num1, float num2);
    float checkYCoordCollision(float num1, float num2);

private:
    
    GLuint spriteSheet;
    GLuint tileSprites;
    bool done;
    float lastFrameTicks;
    SDL_Window* displayWindow;
    float timeRemaining = 0.0f;
    const Uint8* keys;
    std::vector<Entity*> entities;
    int mapWidth;
    int mapHeight;
    Entity* player;
    unsigned char ** levelData;
    std::vector<Entity*> hearts;
    bool gotAllHearts = false;
    Mix_Music* bgm;
    Mix_Chunk* jump;
    Mix_Chunk* gotHeart;
};
#endif /* defined(__NYUCodebase__Platformer__) */
