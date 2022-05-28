#include "pong.h"

int main(int argc, char* argv[])
{
    // Creation de la fenetre et du renderer + test si pas d'erreur
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Window* pWindow{ nullptr };     
    SDL_Renderer* pRenderer{ nullptr };

    if (SDL_CreateWindowAndRenderer(_FIELD_WIDTH, _FIELD_HEIGHT, SDL_WINDOW_SHOWN, &pWindow, &pRenderer) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", SDL_GetError());        
        SDL_Quit();         
        return EXIT_FAILURE;
    }

    if (TTF_Init() < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        return EXIT_FAILURE;
    }

    // On change le nom de la fenetre
    SDL_SetWindowTitle(pWindow, "Pong NabV1");
    
    // On init nos objets ici.
    Ball ball(Vec2(2,2));
    Paddle pad1(Vec2(100, (_FIELD_HEIGHT / 2) - (_PADDLE_HEIGHT / 2)), Vec2(0.0f, 1.0f));
    Paddle pad2(Vec2(_FIELD_WIDTH - 100, (_FIELD_HEIGHT / 2) - (_PADDLE_HEIGHT / 2)), Vec2(0.0f, 1.0f));
    Score score;

    SDL_Event events;
    bool isOpen{ true };

    // Game Loops
    while (isOpen)
    {
        while (SDL_PollEvent(&events))
        {
            switch (events.type)
            {
            // Si on appui sur la croix pour fermer la fenetre ca se ferme bien
            case SDL_QUIT:
                isOpen = false;
                break;

            case SDL_KEYDOWN:
                if (events.key.keysym.sym == SDLK_ESCAPE)
                    isOpen = false;
                
                if (events.key.keysym.sym == SDLK_s)
                    pad1.Paddle::Move(Vec2(0.0f, 2.0f * _PADDLE_SPEED));

                if (events.key.keysym.sym == SDLK_z)
                    pad1.Paddle::Move(Vec2(0.0f, -2.0f * _PADDLE_SPEED));

                if (events.key.keysym.sym == SDLK_l)
                    pad2.Paddle::Move(Vec2(0.0f, 2.0f * _PADDLE_SPEED));

                if (events.key.keysym.sym == SDLK_o)
                    pad2.Paddle::Move(Vec2(0.0f, -2.0f * _PADDLE_SPEED));

                break;
            }
        }

        SDL_SetRenderDrawColor(pRenderer, 0, 0, 0, 255);
        SDL_RenderClear(pRenderer);
        
        drawLine(pRenderer);
        ball.Ball::Draw(pRenderer);
        pad1.Paddle::Draw(pRenderer);
        pad2.Paddle::Draw(pRenderer);
        score.Score::Draw(pRenderer);

        ball.Ball::Move(Vec2(_BALL_SPEED * 2, _BALL_SPEED));

        ball.Ball::collideWithWall(score.scoreP1, score.scoreP2);
        ball.Ball::collideWithPaddle(pad1);
        ball.Ball::collideWithPaddle(pad2);
        
        SDL_RenderPresent(pRenderer);
    }

    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    TTF_Quit();

    return EXIT_SUCCESS;

}