#include "pong.h"

void drawLine(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for (int y = 0; y < _FIELD_HEIGHT; ++y)
    {
        if (y % 5)
        {
            SDL_RenderDrawPoint(renderer, _FIELD_WIDTH / 2, y);
        }
    }
}