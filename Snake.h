#ifndef Snake_h
#define Snake_h

#include <stdio.h>
#include <SDL2/SDL.h>
#include <string>

enum direction { stop, left, right, up, down, start };

struct Position {
    int x; int y; direction dir;
};

class Snake {
public:
    Snake();

    void setup();
    void loop();
    void draw();
    void logic();
    void reset();

    bool itemCollision();
    bool tailCollision();
    void createItem();
    void updateScore();
    void updateHighScore();
    void getHighScore();
    void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip);
    void renderTexture1(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip);
    SDL_Texture* renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer);
private:
    bool gameOver;
    bool turnOver;
    bool pause;
    int score;
    int highScore;
    int tailN;

    Position head;
    Position tail[16*32];

    int itemX;
    int itemY;

    SDL_Rect snakeClips[15];

    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    SDL_Texture *texture = NULL;
    SDL_Texture *printscore = NULL;
    SDL_Texture *printhighscore = NULL;
    SDL_Texture *printgameover = NULL;
    SDL_Texture *printreplay = NULL;
};

#endif
