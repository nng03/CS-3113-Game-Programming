#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <iostream>

#include "Entity.h"

SDL_Window* displayWindow;

GLuint loadTexture(const char *image_path)
{
    SDL_Surface *surface = IMG_Load(image_path);
    
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    SDL_FreeSurface(surface);
    
    return textureID;
}

void resetBoard(Entity* ball)
{
    ball->x = 0.0f;
    ball->y = 0.0f;
}

void setup()
{
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("PONG", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
    glViewport(0, 0, 800, 600);
    glMatrixMode(GL_PROJECTION);
    glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);
}

bool processEvents(Entity* paddle1, Entity* paddle2, Entity* pong)
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            return false;
        }
    }
    // Keyboard Inputs
    const Uint8 *keys = SDL_GetKeyboardState(NULL);
    if (keys[SDL_SCANCODE_W])
    {
        paddle1->directiony = 1.0f;
    }
    else if (keys[SDL_SCANCODE_S])
    {
        paddle1->directiony = -1.0f;
    }
    
    if (keys[SDL_SCANCODE_UP])
    {
        paddle2->directiony= 1.0f;
    }
    else if (keys[SDL_SCANCODE_DOWN])
    {
        paddle2->directiony = -1.0f;
    }
    

    if (keys[SDL_SCANCODE_SPACE])
    {
        resetBoard(pong);
        pong->directionx = -1.0f;
        pong->directiony = 1.0f;
    }
    return true;
}

void update(Entity* paddle1, Entity* paddle2, Entity* pong, float elapsed)
{
    paddle1->y += paddle1->directiony * elapsed;
    paddle2->y += paddle2->directiony * elapsed;
    pong->x += pong->directionx * elapsed;
    pong->y += pong->directiony * elapsed;
    
    if (paddle2->y > 1.0f)
    {
        paddle2->y = 1.0f;
    }
    if (paddle2->y < -1.0f)
    {
        paddle2->y = -1.0f;
    }
    if (paddle1->y > 1.0f)
    {
        paddle1->y = 1.0f;
    }
    if (paddle1->y < -1.0f)
    {
        paddle1->y = -1.0f;
    }
    
    // Paddle collisions don't work as intented. I do not know how to fix this problem
    
    if ((pong->x < (paddle1->x + paddle1->width)) &&
         ((pong->x + pong->width) > paddle1->x) &&
         (pong->y < (paddle1->y + paddle1->height)) &&
         ((pong->y + pong->height) > paddle1->y))
    {
        pong->directionx = -pong->directionx;
    }
    
    if ((pong->x > (paddle2->x + paddle2->width)) &&
        ((pong->x + pong->width) > paddle2->x) &&
        (pong->y < (paddle2->y + paddle2->height)) &&
        ((pong->y + pong->height) > paddle2->y))
    {
        pong->directionx = -pong->directionx;
    }
    
    if (((pong->x + pong->width) > (paddle1->x - paddle1->width * 0.5)) &&
        ((pong->y + pong->height * 0.5) > (paddle1->y - paddle1->height * 0.5)) &&
        ((pong->y - pong->height * 0.5) < (paddle1->y + paddle1->height * 0.5)))
    {
        pong->directionx = -pong->directionx;
    }
    
    if (pong->y+pong->height > 1.0f)
    {
        pong->directiony = -pong->directiony;
    }
    
    if (pong->y < -1.0f)
    {
        pong->directiony = -pong->directiony;
    }
    
    if (pong->x > paddle2->x)
    {
        resetBoard(pong);
        paddle1->score++;
    }
    if (pong->x < paddle1->x)
    {
        resetBoard(pong);
        paddle2->score++;
    }
}

void render(Entity* paddle1, Entity* paddle2, Entity* pong)
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Different colors based on scores
    if (paddle1->score > paddle2->score)
    {
        glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    }
    else if (paddle1->score < paddle2->score)
    {
        glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
    }
    else
    {
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    }
    
    paddle1->textureID = loadTexture("elementGlass018.png");
    paddle2->textureID = loadTexture("elementGlass018.png");
    pong->textureID = loadTexture("ballBowlingBlue.png");
    
    paddle1->draw();
    paddle2->draw();
    pong->draw();
    
    SDL_GL_SwapWindow(displayWindow);
}

void cleanUp()
{
    
}

int main(int argc, char *argv[])
{
    setup();
    
    Entity* player1 = new Entity;
    player1->score = 0;
    player1->x = -1.30f;
    player1->y = 0.0f;
    player1->width = 0.05f;
    player1->height = 0.3f;
    Entity* player2 = new Entity;
    player2->score = 0;
    player2->x = 1.30f;
    player2->y = 0.0f;
    player2->width = 0.05;
    player2->height = 0.3f;
    Entity* pong = new Entity;
    pong->width = 0.05f;
    pong->height = 0.05f;
    pong->directionx = 0.0f;
    pong->directiony = 0.0f;
    pong->x = 0.0f;
    pong->y = 0.0f;
    
    float lastFrameTicks = 0.0f;
    
    // PRESS SPACE TO START THE GAME
    
    while(processEvents(player1, player2, pong))
    {
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        
        processEvents(player1, player2, pong);
        update(player1, player2, pong, elapsed);
        render(player1, player2, pong);
    }
    
	SDL_Quit();
	return 0;
}