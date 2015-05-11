//
//  App.h
//  NYUCodebase
//
//  Created by fp on 4/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__App__
#define __NYUCodebase__App__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <cmath>

#include "Entity.h"
#include "Particles.h"

#define FIXED_TIMESTEP 0.0166666f
#define MAX_TIMESTEPS 6

using namespace std;

class App
{
public:
    App();
    ~App();
    
    void Init();
    void resetGame();
    
    void loadMap();
    bool readHeader(ifstream &stream);
    bool readLayerData(ifstream &stream);
    bool readEntityData(ifstream &stream);
    void placeEntity(string type, float placeX, float placeY);
    
    void update(float elapsed);
    void updateMainMenu(float elapsed);
    void updateGameLevel(float elapsed);
    void updateGameOver(float elapsed);
    void fixedUpdate();
    bool updateAndRender();
    
    void render();
    void renderMainMenu();
    void renderGameLevel();
    void renderGameOver();
    void renderTiles();
    
    void animatePlayer(bool walking);
    void animateEnemies(float elapsed);
    
    void worldToTileCoords(float worldX, float worldY, int* gridX, int* gridY);
    bool isSolid(unsigned char tile);
    
    void handleXCollision(Entity *entity);
    void handleYCollision(Entity *entity);
    float checkXCoordCollision(float num1, float num2);
    float checkYCoordCollision(float num1, float num2);
    
    GLuint loadTexture(const char* imagePath);
    void drawText(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
    
private:
    bool done = false;
    float lastFrameTicks = 0.0f;
    float timeLeftOver = 0.0f;
    SDL_Window *displayWindow;
    
    int state;
    
    ParticleEmitter *particles;
    int particleCount;
    
    const Uint8* keys;
    vector<Entity*> entities;
    vector<Entity*> enemies;
    Entity *player;
    
    bool reachedDoor = false;
    
    float animateTime;
    
    unsigned char ** levelData;
    int mapWidth;
    int mapHeight;
    
    GLuint fontTexture;
    GLuint playerTexture;
    GLuint tilesTexture;
    GLuint enemyTexture;
    
    Mix_Music *gameLevelMusic;
    Mix_Music *gameOverMusic;
    Mix_Music *gameWonMusic;
    
    Mix_Chunk *jumpSound;
    Mix_Chunk *jumpedOnEnemy;
    Mix_Chunk *startedGame;
    Mix_Chunk *gotInjured;
    
};

#endif /* defined(__NYUCodebase__App__) */
