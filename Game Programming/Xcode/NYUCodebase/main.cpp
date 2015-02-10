
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

SDL_Window* displayWindow;

void drawSprite(GLuint texture, float x, float y, float scale, float rotation = 0) {
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(x, y, 0.0);
    glRotatef(rotation, 0.0, 0.0, 1.0);
    glScalef(scale, scale, 0.0);
    
    GLfloat quad[] = {-0.1f, 0.25f, -0.1f, -0.25f, 0.1f, -0.25f, 0.1f, 0.25f};
    glVertexPointer(2, GL_FLOAT, 0, quad);
    glEnableClientState(GL_VERTEX_ARRAY);
    
    GLfloat quadUVs[] = {0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0};
    glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    glDrawArrays(GL_QUADS, 0, 4);
    glDisable(GL_TEXTURE_2D);
}

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

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	bool done = false;
	SDL_Event event;
    
    GLuint image = loadTexture("p1_jump.png");
    GLuint image2 = loadTexture("blockerMad.png");
    GLuint image3 = loadTexture("p1_hurt.png");
    GLuint image4 = loadTexture("blockerSad.png");
    
    float lastFrameTicks = 0.0f;
    float tri_posx1 = -0.05f;
    float tri_posy1 = 1.0f;
    float tri_posx2 = -0.25f;
    float tri_posy2 = 0.5f;
    float tri_posx3 = 0.15f;
    float tri_posy3 = 0.5f;
    
    float leftImagePosx = -0.4;
    float leftImagePosy = -0.3;
    float rightImagePosx = 0.4;
    float rightImagePosy = 0.0;
    
    bool triFell = false;
    bool falling = true;
    
	while (!done) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
				done = true;
			}
		}
		SDL_GL_SwapWindow(displayWindow);
        
        glClear(GL_COLOR_BUFFER_BIT);
        
        float ticks = (float)SDL_GetTicks()/1000.0f;
        float elapsed = ticks - lastFrameTicks;
        lastFrameTicks = ticks;
        float emojiAngle;
        emojiAngle+= elapsed;
        
        // Create Falling Triangle with Vertex Colors
        glLoadIdentity();
        GLfloat triangle[] = {tri_posx1, tri_posy1, tri_posx2, tri_posy2, tri_posx3, tri_posy3};
        glVertexPointer(2, GL_FLOAT, 0, triangle);
        glEnableClientState(GL_VERTEX_ARRAY);
        GLfloat triangleColors[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0};
        glColorPointer(3, GL_FLOAT, 0, triangleColors);
        glEnableClientState(GL_COLOR_ARRAY);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableClientState(GL_COLOR_ARRAY);
        
        if (falling)
        {
            tri_posy1 -= 0.01f;
            tri_posy2 -= 0.01f;
            tri_posy3 -= 0.01f;
            if (tri_posy1 < 0.0f)
            {
                falling = false;
                triFell = true;
            }
        }
        else {
            if (tri_posy1 >= 2.0f)
            {
                tri_posy1 = 2.0f;
                tri_posy2 = 1.95f;
                tri_posy3 = 1.95f;
            }
            else
            {
                tri_posy1 += 0.02f;
                tri_posy2 += 0.02f;
                tri_posy3 += 0.02f;
            }
        }
        if (!triFell)
        {
            drawSprite(image, leftImagePosx, leftImagePosy, 0.5, 0.0);
            drawSprite(image2, rightImagePosx, rightImagePosy, 1.0, 0.0);
        }
        else {
            if (leftImagePosx == -1.0)
            {
                
            }
            else
            {
                leftImagePosx -= 0.01;
                leftImagePosy += 0.01;
                rightImagePosx += 0.01;
                rightImagePosy += 0.01;
                drawSprite(image3, leftImagePosx, leftImagePosy, 0.5, 0.0);
                drawSprite(image4, rightImagePosx, rightImagePosy, 1.0, 0.0);
            }
        }
        
    }
    
	SDL_Quit();
	return 0;
}
