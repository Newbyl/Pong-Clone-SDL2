#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

using namespace std;

const int _FIELD_WIDTH = 1280;
const int _FIELD_HEIGHT = 720;

const int _PADDLE_WIDTH = 10;
const int _PADDLE_HEIGHT = 150;

const float _BALL_SPEED = 1.5f;
const float _PADDLE_SPEED = 15.0f;

// Methode dans pong.cpp pour faire toute sorte de chose avec le terrain etc...
void drawLine(SDL_Renderer* renderer);

// Classe pour manipuler des vecteur 2D + simplement.
class Vec2
{
public:
	float x;
	float y;

	Vec2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vec2(float _x, float _y)
	{
		x = _x;
		y = _y;
	}
	
	Vec2 operator+ (Vec2 const &obj)
	{
		return Vec2(x + obj.x, y + obj.y);
	}

	Vec2& operator+=(Vec2 const &obj)
	{
		x += obj.x;
		y += obj.y;

		return *this;
	}

	Vec2 operator* (Vec2 const &obj)
	{
		return Vec2(x * obj.x, y * obj.y);
	}

	Vec2 operator- (Vec2 const &obj)
	{
		return Vec2(x - obj.x, y - obj.y);
	}

	void invertX()
	{
		x = -x;
	}

	void invertY()
	{
		y = -y;
	}
};

class Paddle
{
public:
	Vec2 coord;
	Vec2 velocity;

	const int PADDLE_WIDTH = _PADDLE_WIDTH;
	const int PADDLE_HEIGHT = _PADDLE_HEIGHT;

	Paddle(Vec2 _coord, Vec2 _velocity)
	{
		coord.x = _coord.x;
		coord.y = _coord.y;
		velocity = _velocity;
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_Rect pad{static_cast<int>(coord.x),  static_cast<int>(coord.y), PADDLE_WIDTH, PADDLE_HEIGHT};
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderFillRect(renderer, &pad);
	}

	void Move(Vec2 dt)
	{
		coord += velocity * dt;

		if (coord.y < 0)
		{
			coord = Vec2(coord.x, 0.0f);
		}
		if (coord.y > _FIELD_HEIGHT - _PADDLE_HEIGHT)
		{
			coord = Vec2(coord.x, _FIELD_HEIGHT - _PADDLE_HEIGHT);
		}
	}
};

class Score
{
public:
	int scoreP1;
	int scoreP2;

	Score()
	{
		scoreP1 = 0;
		scoreP2 = 0;
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		TTF_Font* font = TTF_OpenFont("font/hack.ttf", 35);
		
		SDL_Surface* sP1 = TTF_RenderText_Blended(font, &to_string(scoreP1)[0], SDL_Color{ 255, 255, 255, 255 });
		SDL_Surface* sP2 = TTF_RenderText_Blended(font, &to_string(scoreP2)[0], SDL_Color{ 255, 255, 255, 255 });
		
		SDL_Texture* textureSP1 = SDL_CreateTextureFromSurface(renderer, sP1);
		SDL_Texture* textureSP2 = SDL_CreateTextureFromSurface(renderer, sP2);
		
		SDL_Rect pos1{200, _FIELD_HEIGHT / 4, 35, 35};
		SDL_Rect pos2{_FIELD_WIDTH - 200, _FIELD_HEIGHT / 4, 35, 35};

		SDL_QueryTexture(textureSP1, nullptr, nullptr, &pos1.w, &pos1.h);
		SDL_QueryTexture(textureSP2, nullptr, nullptr, &pos2.w, &pos2.h);

		SDL_RenderCopy(renderer, textureSP1, nullptr, &pos1);
		SDL_RenderCopy(renderer, textureSP2, nullptr, &pos2);

		SDL_DestroyTexture(textureSP1);
		SDL_DestroyTexture(textureSP2);
		SDL_FreeSurface(sP1);
		SDL_FreeSurface(sP2);
		TTF_CloseFont(font);
	}
};

class Ball
{
public:
	Vec2 coord;
	Vec2 velocity;

	int BALL_WIDTH = 15;
	int BALL_HEIGHT = 15;

	Ball(Vec2 _velocity)
	{
		coord.x = _FIELD_WIDTH / 2 - BALL_WIDTH / 2;
		coord.y = _FIELD_HEIGHT / 2 - BALL_HEIGHT / 2;
		velocity = _velocity;
	}

	void Draw(SDL_Renderer* renderer)
	{
		SDL_Rect ball{static_cast<int>(coord.x),  static_cast<int>(coord.y), BALL_WIDTH, BALL_HEIGHT};
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

		SDL_RenderFillRect(renderer, &ball);
	}

	void Move(Vec2 dt)
	{
		coord += velocity * dt;
	}

	void collideWithWall(int &score1, int &score2)
	{
		if (coord.x <= 0)
		{
			coord.x = _FIELD_WIDTH / 2 - BALL_WIDTH / 2;
			coord.y = _FIELD_HEIGHT / 2 - BALL_HEIGHT / 2;
			score2++;
		}
			
		if (coord.x >= _FIELD_WIDTH - BALL_WIDTH)
		{
			coord.x = _FIELD_WIDTH / 2 - BALL_WIDTH / 2;
			coord.y = _FIELD_HEIGHT / 2 - BALL_HEIGHT / 2;
			score1++;
		}
			
		if (coord.y <= 0)
			velocity.invertY();
		if (coord.y >= _FIELD_HEIGHT - BALL_HEIGHT)
			velocity.invertY();
	}

	void collideWithPaddle(Paddle pad)
	{
		if (coord.x > pad.coord.x - _PADDLE_WIDTH && coord.x <= pad.coord.x
		&& coord.y > pad.coord.y && coord.y <= pad.coord.y + _PADDLE_HEIGHT)
			velocity.invertX();

		if (coord.x > pad.coord.x && coord.x <= pad.coord.x + _PADDLE_WIDTH
		&& coord.y > pad.coord.y && coord.y <= pad.coord.y + _PADDLE_HEIGHT)
			velocity.invertX();
	}
};
