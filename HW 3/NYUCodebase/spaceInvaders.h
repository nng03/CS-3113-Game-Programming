//
//  spaceInvaders.h
//  NYUCodebase
//
//  Created by fp on 2/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#ifndef __NYUCodebase__spaceInvaders__
#define __NYUCodebase__spaceInvaders__

#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <vector>
#include <string>
#include "Entity.h"

class spaceInvader
{
public:
    spaceInvader();
    ~spaceInvader();
    
    void Init();
    bool UpdateAndRender();
    
    void Render();
    void RenderTitleScreen();
    void RenderGameLevel();
    void RenderGameOver();
    
    void Update(float elapsed);
    void UpdateTitleScreen(float elapsed);
    void UpdateGameLevel(float elapsed);
    void UpdateGameOver(float elapsed);
    
    GLuint loadTexture(const char* imagePath);
    void drawFontToScreen(GLuint fontTexture, std::string text, float size, float spacing, float r, float g, float b, float a, float x, float y);
    
private:
    SDL_Window *displayWindow;
    
    GLuint fonts;
    GLuint spriteSheet;
    Entity *player;
    std::vector<Entity*> enemies;
    std::vector<Entity*> playerAmmo;
    std::vector<Entity*> enemyAmmo;
    
    bool done;
    bool alive;
    
    int state;
    int score;
    int damage;
    
    const Uint8* keys;
    
    float lastFrameTicks;
    float seconds;
};

#endif /* defined(__NYUCodebase__spaceInvaders__) */
