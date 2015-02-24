#include "spaceInvaders.h"

int main(int argc, char *argv[])
{
    spaceInvader game;
    while(!game.UpdateAndRender()) {}
	return 0;
}