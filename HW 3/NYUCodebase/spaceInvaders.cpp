//
//  spaceInvaders.cpp
//  NYUCodebase
//
//  Created by fp on 2/23/15.
//  Copyright (c) 2015 Ivan Safrin. All rights reserved.
//

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

#include "spaceInvaders.h"
#include "Entity.h"
#include <vector>
#include <string>
#include <iostream>

using namespace std;

enum gameState {STATE_TITLE_SCREEN, STATE_GAME_LEVEL, STATE_GAME_OVER};

GLuint spaceInvader::loadTexture(const char *imagePath)
{
    SDL_Surface *surface = IMG_Load(imagePath);
    
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    SDL_FreeSurface(surface);
    
    return texID;
}

void spaceInvader::drawFontToScreen(GLuint fontTexture, string text, float size, float spacing, float r, float g, float b, float a, float x, float y)
{
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    
    float texture_size = 1.0/16.0f;
    
    vector<float> vertexData;
    vector<float> texCoordData;
    vector<float> colorData;
    
    for(int i=0; i < text.size(); i++) {
        float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
        float texture_y = (float)(((int)text[i]) / 16) / 16.0f;
        
        colorData.insert(colorData.end(), {r,g,b,a, r,g,b,a, r,g,b,a, r,g,b,a});
        
        vertexData.insert(vertexData.end(), {((size+spacing) * i) + (-0.5f * size), 0.5f * size, ((size+spacing) * i) + (-0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), -0.5f * size, ((size+spacing) * i) + (0.5f * size), 0.5f * size});
        
        texCoordData.insert(texCoordData.end(), {texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +texture_size, texture_y + texture_size, texture_x + texture_size, texture_y});
    }
    
    glColorPointer(4, GL_FLOAT, 0, colorData.data());
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
    glEnableClientState(GL_VERTEX_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glDrawArrays(GL_QUADS, 0, text.size() * 4.0f);
    
    glDisableClientState(GL_COLOR_ARRAY);
    
}

spaceInvader::spaceInvader()
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow =  SDL_CreateWindow("Space Invaders", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    state = 0;
    keys = SDL_GetKeyboardState(nullptr);
    done = false;
    lastFrameTicks = 0.0f;
    fonts = loadTexture("pixel_font.png");
    spriteSheet = loadTexture("slice01_01.png");
    
    player = new Entity(spriteSheet, 0.0f, -0.9f, 0.0f, 0.0f, 0.0f, 0.0f / 256.0f, 156.0f / 512.0f, 112.0f / 256.0f, 75.0f / 512.0f);
    
    Init();
}

spaceInvader::~spaceInvader()
{
    delete player;
    for (GLuint i = 0; i < playerAmmo.size(); i++)
    {
        delete playerAmmo[i];
    }
    for (GLuint i = 0; i < enemyAmmo.size(); i++)
    {
        delete enemyAmmo[i];
    }
    for (GLuint i = 0; i < enemies.size(); i++)
    {
        delete enemies[i];
    }
    SDL_Quit();
}

void spaceInvader::Init()
{
    for (GLuint i = 0; i < playerAmmo.size(); i++)
    {
        delete playerAmmo[i];
    }
    for (GLuint i = 0; i < enemyAmmo.size(); i++)
    {
        delete enemyAmmo[i];
    }
    for (GLuint i = 0; i < enemies.size(); i++)
    {
        delete enemies[i];
    }
    playerAmmo.clear();
    enemyAmmo.clear();
    enemies.clear();
    
    // Create new enemies
    for (float i = 0.3f; i < 1.0f; i+=0.3f)
    {
        enemies.push_back(new Entity(spriteSheet, -0.8f, i, 0.2f, -0.05f, 0.0f, 114.0f / 256.0f, 0.0f / 512.0f, 93.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new Entity(spriteSheet, -0.4f, i, 0.2f, -0.05f, 0.0f, 0.0f / 256.0f, 311.0f / 512.0f, 104.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new Entity(spriteSheet, 0.0f, i, 0.2f, -0.05f, 0.0f, 0.0f / 256.0f, 397.0f / 512.0f, 103.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new Entity(spriteSheet, 0.4f, i, 0.2f, -0.05f, 0.0f, 114.0f / 256.0f, 86.0f / 512.0f, 82.0f / 256.0f, 84.0f / 512.0f));
        enemies.push_back(new Entity(spriteSheet, 0.8f, i, 0.2f, -0.05f, 0.0f, 106.0f / 256.0f, 311.0f / 512.0f, 97.0f / 256.0f, 84.0f / 512.0f));
    }
    
    state = STATE_TITLE_SCREEN;
    score = 0;
    alive = true;
    damage = 0;
    
}

bool spaceInvader::UpdateAndRender()
{
    float ticks = (float)SDL_GetTicks()/1000.0f;
    float elapsed = ticks - lastFrameTicks;
    lastFrameTicks = ticks;
    
    Update(elapsed);
    Render();
    return done;
}

void spaceInvader::Render()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    switch(state)
    {
        case STATE_TITLE_SCREEN:
            RenderTitleScreen();
            break;
        case STATE_GAME_LEVEL:
            RenderGameLevel();
            break;
        case STATE_GAME_OVER:
            RenderGameOver();
            break;
    }
    SDL_GL_SwapWindow(displayWindow);
}

void spaceInvader::RenderTitleScreen()
{
    glLoadIdentity();
    drawFontToScreen(fonts, "SPACE INVADERS", 0.1f, -0.01f, 0.0f, 1.0f, 1.0f, 0.3f, -1.0f, 0.3f);
    drawFontToScreen(fonts, "Press Enter to Play", 0.1f, -0.02f, 1.0f, 1.0f, 0.0f, 0.3f, -1.0f, 0.0f);
}

void spaceInvader::RenderGameLevel()
{
    glLoadIdentity();
    glTranslatef(-1.2f, 0.8f, 0.0);
    drawFontToScreen(fonts, "SCORE: ", 0.05f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -1.25f, -0.75f);
    drawFontToScreen(fonts, to_string(score), 0.05f, -0.02f, 1.0f, 1.0f, 1.0f, 1.0f, -0.95f, -0.75f);
    glTranslatef(0.0f, 0.1f, 0.0);
    for (GLuint i = damage; i < 3; i++)
    {
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, spriteSheet);
        glMatrixMode(GL_MODELVIEW);
        GLfloat quad[] = { -112.0f / 256.0f * 0.1, 75.0f / 512.0f * 0.2, -112.0f / 256.0f * 0.1, -75.0f / 512.0f * 0.2,
            112.0f / 256.0f * 0.1, -75.0f / 512.0f * 0.2, 112.0f / 256.0f * 0.1, 75.0f / 512.0f * 0.2 };
        glVertexPointer(2, GL_FLOAT, 0, quad);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat quadUVs[] = { 0.0f / 256.0f, 156.0f / 512.0f, 0.0f / 256.0f, 156.0f / 512.0f + 75.0f / 512.0f, 0.0f / 256.0f + 112.0f / 256.0f, 156.0f / 512.0f + 75.0f / 512.0f, 0.0f / 256.0f + 112.0f / 256.0f, 156.0f / 512.0f };
        glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDrawArrays(GL_QUADS, 0, 4);
        glDisable(GL_TEXTURE_2D);
        glTranslatef(0.1f, 0.0f, 0.0);
    }
    glLoadIdentity();
    player->drawSprite(1, 1, 3, 0.25f);
    
    for (GLuint i = 0; i < enemies.size(); i++)
    {
        enemies[i]->drawSprite(1, 1, 3, 0.2f);
    }
    for (GLuint i = 0; i < playerAmmo.size(); i++)
    {
        playerAmmo[i]->drawSprite(1, 1, 3, 0.2f);
    }
    for (GLuint i = 0; i < enemyAmmo.size(); i++)
    {
        enemyAmmo[i]->drawSprite(1, 1, 3, 0.2f);
    }
    
}

void spaceInvader::RenderGameOver()
{
    if (alive)
    {
        drawFontToScreen(fonts, "YOU ARE A WIENER", 0.1f, -0.01f, 0.0f, 1.0f, 1.0f, 0.3f, -1.0f, 0.3f);
        drawFontToScreen(fonts, "Press SPACE to Play Again", 0.1f, -0.02f, 1.0f, 1.0f, 0.0f, 0.3f, -1.0f, 0.0f);
    }
    else
    {
        drawFontToScreen(fonts, "YOU SUCK", 0.1f, -0.01f, 0.0f, 1.0f, 1.0f, 0.3f, -1.0f, 0.3f);
        drawFontToScreen(fonts, "Press SPACE to Play Again", 0.1f, -0.02f, 1.0f, 1.0f, 0.0f, 0.3f, -1.0f, 0.0f);
    }
}

void spaceInvader::Update(float elapsed)
{
    switch(state)
    {
        case STATE_TITLE_SCREEN:
            UpdateTitleScreen(elapsed);
            break;
        case STATE_GAME_LEVEL:
            UpdateGameLevel(elapsed);
            break;
        case STATE_GAME_OVER:
            UpdateGameOver(elapsed);
            break;
    }
}

void spaceInvader::UpdateTitleScreen(float elapsed)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.type == SDL_KEYUP)
        {
            if (event.key.keysym.scancode == SDL_SCANCODE_RETURN && !event.key.repeat) {
                state = STATE_GAME_LEVEL;
            }
        }
    }
}

void spaceInvader::UpdateGameLevel(float elapsed)
{
    // Player Ammo
    for (GLuint i = 0; i < playerAmmo.size(); i++)
    {
        if (playerAmmo[i]->y > 1.1)
        {
            if (playerAmmo[i])
            {
                delete playerAmmo[i];
                playerAmmo.erase(playerAmmo.begin() + i);
            }
        }
        else
        {
            playerAmmo[i]->y += playerAmmo[i]->yDirect*playerAmmo[i]->speed*elapsed;
        }
    }
    
    // Enemy Ammo
    for (GLuint i = 0; i < enemyAmmo.size(); i++)
    {
        if (enemyAmmo[i]->y < -1.1)
        {
            if (enemyAmmo[i]){
                delete enemyAmmo[i];
                enemyAmmo.erase(enemyAmmo.begin() + i);
            }
        }
        else
        {
            enemyAmmo[i]->y += enemyAmmo[i]->yDirect*enemyAmmo[i]->speed*elapsed;
        }
    }
    
    // Enemy stuff
    
    seconds += elapsed;
    
    for (GLuint i = 0; i < enemies.size(); i++)
    {
        enemies[i]->x += enemies[i]->xDirect*enemies[i]->speed*elapsed;
        if (enemies[i]->y < -0.75f)
        {
            state = STATE_GAME_OVER;
            alive = false;
        }
        if (enemies[i]->x>1.2f || enemies[i]->x<-1.2f)
        {
            for (GLuint k = 0; k < enemies.size(); k++)
            {
                enemies[k]->xDirect= -(enemies[k]->xDirect);
                enemies[k]->x += enemies[k]->xDirect*enemies[k]->speed*elapsed;
                enemies[k]->y += enemies[k]->yDirect;
                enemies[k]->speed += 0.2f;
            }
        }
        if (seconds >= 1.0)
        {
            enemyAmmo.push_back(new Entity(spriteSheet, enemies[i]->x, enemies[i]->y, 0.0f, -1.0f, 0.0f, 173.0f / 256.0f, 211.0f / 512.0f, 13.0f / 256.0f, 57.0f / 512.0f));
            seconds = 0;
        }
        for (GLuint j = 0; j < playerAmmo.size(); j++)
        {
            if ((enemies[i]->x + (enemies[i]->w*0.3f) > playerAmmo[j]->x) &&
                (enemies[i]->x - (enemies[i]->w*0.3f) < playerAmmo[j]->x) &&
                (enemies[i]->y + (enemies[i]->h*0.3f) > playerAmmo[j]->y) &&
                (enemies[i]->y - (enemies[i]->h*0.3f) < playerAmmo[j]->y))
            {
                if (playerAmmo[j])
                {
                    delete playerAmmo[j];
                    playerAmmo.erase(playerAmmo.begin() + j);
                }
                if (enemies[i])
                {
                    delete enemies[i];
                    enemies.erase(enemies.begin() + i);
                    score += 250;
                }
                if (!enemies.size())
                {
                    state = STATE_GAME_OVER;
                }
                break;
            }
        }
    }
    
    // Player health
    for (GLuint i = 0; i < enemyAmmo.size(); i++)
    {
        if ((player->x + (player->w*0.3f) > enemyAmmo[i]->x) &&
            (player->x - (player->w*0.3f) < enemyAmmo[i]->x) &&
            (player->y + (player->h*0.3f) > enemyAmmo[i]->y) &&
            (player->y - (player->h*0.3f) < enemyAmmo[i]->y))
        {
            damage++;
            if (enemyAmmo[i])
            {
                delete enemyAmmo[i];
                enemyAmmo.erase(enemyAmmo.begin() + i);
            }
        }
    }
    if (damage > 3)
    {
        alive = false;
        state = STATE_GAME_OVER;
    }
    
    // Keyboard Inputs
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
        {
            done = true;
        }
    }
    
    if (keys[SDL_SCANCODE_SPACE])
    {
        if (state == STATE_GAME_LEVEL)
        {
            playerAmmo.push_back(new Entity(spriteSheet, player->x, -0.8f, 0.0f, 1.0f, 0.0f, 158.0f / 256.0f, 211.0f / 512.0f, 13.0f / 256.0f, 57.0f / 512.0f));
        }
    }
    
    if(keys[SDL_SCANCODE_LEFT] || keys[SDL_SCANCODE_A])
    {
        player->xDirect = -1.0f;
        player->move(elapsed);
        if (player->x < -1.33f) {
            player->x = 1.33f;
        }
    }
    else if (keys[SDL_SCANCODE_RIGHT] || keys[SDL_SCANCODE_D])
    {
        player->xDirect = 1.0f;
        player->move(elapsed);
        if (player->x > 1.33f) {
            player->x = -1.33f;
        }
    }
    else
    {
        player->xDirect = 0.0f;
    }
    
    if (keys[SDL_SCANCODE_Z])
    {
        alive = false;
        state = STATE_GAME_OVER;
    }
}

void spaceInvader::UpdateGameOver(float elapsed)
{
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            done = true;
        }
        else if (event.key.keysym.scancode == SDL_SCANCODE_SPACE && !event.key.repeat) {
            state = STATE_TITLE_SCREEN;
            Init();
        }
    }
}