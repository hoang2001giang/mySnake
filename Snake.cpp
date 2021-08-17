#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <fstream>
#include <ctime>
#include "Snake.h"

const int SPRITE_SIZE = 32;
const int SQUARES = 16;
const int PADDING_TOP = 32;
const int PADDING_SIDES = 8;

const int SCREEN_WIDTH = SPRITE_SIZE * SQUARES * 2;
const int SCREEN_HEIGHT = SPRITE_SIZE * SQUARES;
const int SCREEN_CENTRE_X = PADDING_SIDES + 16 + (SCREEN_WIDTH/2)-(SPRITE_SIZE/2);
const int SCREEN_CENTRE_Y = PADDING_TOP + 16 + (SCREEN_HEIGHT/2)-(SPRITE_SIZE/2);

const int WINDOW_WIDTH = SCREEN_WIDTH + 2 * PADDING_SIDES;
const int WINDOW_HEIGHT = SCREEN_HEIGHT + PADDING_TOP + PADDING_SIDES;

const int FPS = 7;
const int FRAME_PERIOD = 1000.0f / FPS;

Snake::Snake()
: tailN {0}, score {0}, gameOver {false}, turnOver {false}, pause {false} {
    head.x = SCREEN_CENTRE_X; head.y = SCREEN_CENTRE_Y; head.dir = start;
    srand(time(NULL));
    setup();
    loop();
}

void Snake::setup() {
    SDL_INIT_VIDEO;
    TTF_Init();

    SDL_Window* splashwindow = SDL_CreateWindow("Splash", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 250, SDL_WINDOW_SHOWN);
    SDL_Surface* splashsurface = SDL_GetWindowSurface( splashwindow );
    SDL_Surface* splash = IMG_Load("phg's snake.png");

    SDL_BlitSurface( splash, NULL, splashsurface, NULL );
    SDL_UpdateWindowSurface( splashwindow );
    SDL_Delay(2000);

    SDL_FreeSurface(splash);
    SDL_FreeSurface(splashsurface);
    SDL_DestroyWindow(splashwindow);

    window = SDL_CreateWindow("snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr)
        std::cerr << "Error: Create Window";

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr)
        std::cerr << "Error: Create renderer";

    texture = IMG_LoadTexture(renderer, "sprites1.jpg");
    if (texture == nullptr)
        std::cerr << "Error: Load texture";

    updateScore();
    getHighScore();
    updateHighScore();


    snakeClips[0].x = 0;
    snakeClips[0].y = 0;
    snakeClips[0].w = SPRITE_SIZE;
    snakeClips[0].h = SPRITE_SIZE;

    snakeClips[1].x = 1*SPRITE_SIZE;
    snakeClips[1].y = 0*SPRITE_SIZE;
    snakeClips[1].w = SPRITE_SIZE;
    snakeClips[1].h = SPRITE_SIZE;

    snakeClips[2].x = 2*SPRITE_SIZE;
    snakeClips[2].y = 0*SPRITE_SIZE;
    snakeClips[2].w = SPRITE_SIZE;
    snakeClips[2].h = SPRITE_SIZE;

    snakeClips[3].x = 3*SPRITE_SIZE;
    snakeClips[3].y = 0*SPRITE_SIZE;
    snakeClips[3].w = SPRITE_SIZE;
    snakeClips[3].h = SPRITE_SIZE;

    snakeClips[4].x = 1*SPRITE_SIZE;
    snakeClips[4].y = 1*SPRITE_SIZE;
    snakeClips[4].w = SPRITE_SIZE;
    snakeClips[4].h = SPRITE_SIZE;

    snakeClips[5].x = 0*SPRITE_SIZE;
    snakeClips[5].y = 1*SPRITE_SIZE;
    snakeClips[5].w = SPRITE_SIZE;
    snakeClips[5].h = SPRITE_SIZE;

    snakeClips[6].x = 2*SPRITE_SIZE;
    snakeClips[6].y = 1*SPRITE_SIZE;
    snakeClips[6].w = SPRITE_SIZE;
    snakeClips[6].h = SPRITE_SIZE;

    snakeClips[7].x = 3*SPRITE_SIZE;
    snakeClips[7].y = 1*SPRITE_SIZE;
    snakeClips[7].w = SPRITE_SIZE;
    snakeClips[7].h = SPRITE_SIZE;

    snakeClips[8].x = 0*SPRITE_SIZE;
    snakeClips[8].y = 2*SPRITE_SIZE;
    snakeClips[8].w = SPRITE_SIZE;
    snakeClips[8].h = SPRITE_SIZE;

    snakeClips[9].x = 1*SPRITE_SIZE;
    snakeClips[9].y = 2*SPRITE_SIZE;
    snakeClips[9].w = SPRITE_SIZE;
    snakeClips[9].h = SPRITE_SIZE;

    snakeClips[10].x = 2*SPRITE_SIZE;
    snakeClips[10].y = 2*SPRITE_SIZE;
    snakeClips[10].w = SPRITE_SIZE;
    snakeClips[10].h = SPRITE_SIZE;

    snakeClips[11].x = 3*SPRITE_SIZE;
    snakeClips[11].y = 2*SPRITE_SIZE;
    snakeClips[11].w = SPRITE_SIZE;
    snakeClips[11].h = SPRITE_SIZE;

    snakeClips[12].x = 0*SPRITE_SIZE;
    snakeClips[12].y = 3*SPRITE_SIZE;
    snakeClips[12].w = SPRITE_SIZE;
    snakeClips[12].h = SPRITE_SIZE;

    snakeClips[13].x = 1*SPRITE_SIZE;
    snakeClips[13].y = 3*SPRITE_SIZE;
    snakeClips[13].w = SPRITE_SIZE;
    snakeClips[13].h = SPRITE_SIZE;

    snakeClips[14].x = 2*SPRITE_SIZE;
    snakeClips[14].y = 3*SPRITE_SIZE;
    snakeClips[14].w = SPRITE_SIZE;
    snakeClips[14].h = SPRITE_SIZE;

    createItem();
}

void Snake::draw() {
    SDL_SetRenderDrawColor(renderer, 52, 51, 50, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 225, 225, 0, 0);
    SDL_Rect screenfill { PADDING_SIDES, PADDING_TOP, SCREEN_WIDTH, SCREEN_HEIGHT };
    SDL_RenderFillRect(renderer, &screenfill);

    renderTexture1(printscore, renderer, PADDING_SIDES+3, 10, nullptr);
    renderTexture1(printhighscore, renderer, PADDING_SIDES + 160 , 10, nullptr);
    renderTexture1(texture, renderer, itemX, itemY, &snakeClips[14]);

    if(head.dir == up)
        renderTexture1(texture, renderer, head.x, head.y, &snakeClips[0]);
    if(head.dir == down)
        renderTexture1(texture, renderer, head.x, head.y, &snakeClips[1]);
    if(head.dir == left)
        renderTexture1(texture, renderer, head.x, head.y, &snakeClips[2]);
    if(head.dir == right)
        renderTexture1(texture, renderer, head.x, head.y, &snakeClips[3]);
    if(head.dir == start)
        renderTexture1(texture, renderer, head.x, head.y, &snakeClips[0]);

    if (tailN > 0){
        for (int i = 0; i < tailN - 1; ++i) {
            if(tail[i+1].dir == tail[i].dir) {
                if(tail[i].dir == up || tail[i].dir == down)
                    renderTexture1(texture, renderer, tail[i].x, tail[i].y, &snakeClips[4]);
                if(tail[i].dir == left || tail[i].dir == right)
                    renderTexture1(texture, renderer, tail[i].x, tail[i].y, &snakeClips[5]);
            } else {
                if((tail[i+1].dir == up && tail[i].dir == left) || (tail[i+1].dir == right && tail[i].dir == down))
                    renderTexture1(texture, renderer, tail[i].x, tail[i].y, &snakeClips[11]);
                if((tail[i+1].dir == left && tail[i].dir == up) || (tail[i+1].dir == down && tail[i].dir == right))
                    renderTexture1(texture, renderer, tail[i].x, tail[i].y, &snakeClips[10]);
                if((tail[i+1].dir == right && tail[i].dir == up) || (tail[i+1].dir == down && tail[i].dir == left))
                    renderTexture1(texture, renderer, tail[i].x, tail[i].y, &snakeClips[13]);
                if((tail[i+1].dir == left && tail[i].dir == down) || (tail[i+1].dir == up && tail[i].dir == right))
                    renderTexture1(texture, renderer, tail[i].x, tail[i].y, &snakeClips[12]);
            }
        }

        if(tail[tailN-1].dir == up)
            renderTexture1(texture, renderer, tail[tailN-1].x, tail[tailN-1].y, &snakeClips[6]);
        if(tail[tailN-1].dir == down)
            renderTexture1(texture, renderer, tail[tailN-1].x, tail[tailN-1].y, &snakeClips[7]);
        if(tail[tailN-1].dir == left)
            renderTexture1(texture, renderer, tail[tailN-1].x, tail[tailN-1].y, &snakeClips[8]);
        if(tail[tailN-1].dir == right)
            renderTexture1(texture, renderer, tail[tailN-1].x, tail[tailN-1].y, &snakeClips[9]);


     }


    SDL_Rect gameoverRect;
    SDL_Rect gameoverRect2;
    void getRect(const SDL_Texture t);
    SDL_QueryTexture(printgameover, NULL, NULL, &gameoverRect.w, &gameoverRect.h);
    SDL_QueryTexture(printreplay, NULL, NULL, &gameoverRect2.w, &gameoverRect2.h);
    gameoverRect.x = PADDING_SIDES + (SCREEN_WIDTH/2)-(gameoverRect.w/2);
    gameoverRect.y = PADDING_TOP + (SCREEN_HEIGHT/2)-(gameoverRect.h);
    gameoverRect2.x = (SCREEN_WIDTH/2)-(gameoverRect2.w/2);
    gameoverRect2.y = gameoverRect.y + gameoverRect.h + gameoverRect2.h;

    renderTexture(printgameover, renderer, gameoverRect, nullptr);
    renderTexture(printreplay, renderer, gameoverRect2, nullptr);

    SDL_RenderPresent(renderer);
}

void Snake::loop() {
    Uint32 frameStart, frameTime;
    SDL_Event event;
    while (gameOver == false){
        frameStart = SDL_GetTicks();
        while (SDL_PollEvent(&event)){

            if (event.type == SDL_QUIT)
                gameOver = true;

            if (event.type == SDL_KEYDOWN){
                if ((event.key.keysym.sym == SDLK_a || event.key.keysym.sym == SDLK_LEFT) && head.dir != right)
                    head.dir = left;
                if ((event.key.keysym.sym == SDLK_d || event.key.keysym.sym == SDLK_RIGHT) && head.dir != left)
                    head.dir = right;
                if ((event.key.keysym.sym == SDLK_w || event.key.keysym.sym == SDLK_UP) && head.dir != down)
                    head.dir = up;
                if ((event.key.keysym.sym == SDLK_s || event.key.keysym.sym == SDLK_DOWN) && head.dir != up)
                    head.dir = down;
                if (event.key.keysym.sym == SDLK_ESCAPE){
                    gameOver = true;
                    head.dir = stop;
                }
                if (event.key.keysym.sym == SDLK_SPACE){
                    if (turnOver == true)
                        reset();
                    else if (pause == true)
                        pause = false;
                    else if (pause == false)
                        pause = true;
                }
            }
        }
        if (pause == false && tailCollision() == false){
            logic();
            draw();
        }

        frameTime = SDL_GetTicks() - frameStart;
        if (frameTime < FRAME_PERIOD){
            SDL_Delay((int)(FRAME_PERIOD - frameTime));
        }
    }
    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(printscore);
    SDL_DestroyTexture(printgameover);
    SDL_DestroyTexture(printreplay);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
}

void Snake::logic() {
    if (itemCollision()){
        score += 10;
        if (score > highScore) updateHighScore();
        ++tailN;
        updateScore();
        createItem();
    }

    if (head.dir == left)
        head.x -= SPRITE_SIZE;
    else if (head.dir == right)
        head.x += SPRITE_SIZE;
    else if (head.dir == up)
        head.y -= SPRITE_SIZE;
    else if (head.dir == down)
        head.y += SPRITE_SIZE;

     for (int i = tailN-1; i > 0; --i){
            tail[i].x = tail[i-1].x;
            tail[i].y = tail[i-1].y;
            tail[i].dir = tail[i-1].dir;
     }

     if (head.dir == left){
     tail[0].x = head.x + SPRITE_SIZE;
     tail[0].y = head.y;
     }
     else if (head.dir == right){
     tail[0].x = head.x - SPRITE_SIZE;
     tail[0].y = head.y;
     }
     else if (head.dir == up){
     tail[0].x = head.x;
     tail[0].y = head.y + SPRITE_SIZE;
     }
     else if (head.dir == down){
     tail[0].x = head.x;
     tail[0].y = head.y - SPRITE_SIZE;
     }
     tail[0].dir = head.dir;

    if(tailCollision()) {
        SDL_Color fontcolour = { 255, 255, 255, 0 };
        printgameover = renderText("GAME OVER", "GreenFlame.ttf", fontcolour, 60, renderer);
        printreplay = renderText("Press spacebar to continue", "GreenFlame.ttf", fontcolour, 13, renderer);
        turnOver = true;
    }

    if (head.x < PADDING_SIDES)
        head.x = SCREEN_WIDTH + PADDING_SIDES - SPRITE_SIZE;
    if (head.x > (PADDING_SIDES + SCREEN_WIDTH - SPRITE_SIZE) )
        head.x = PADDING_SIDES;
    if (head.y < PADDING_TOP)
        head.y = SCREEN_HEIGHT + PADDING_TOP - SPRITE_SIZE;
    if (head.y > (PADDING_TOP + SCREEN_HEIGHT - SPRITE_SIZE) )
        head.y = PADDING_TOP;
}

bool Snake::itemCollision() {
    if ( head.y <= itemY - SPRITE_SIZE )
        return false;
    if ( head.y >= itemY + SPRITE_SIZE )
        return false;
    if ( head.x <= itemX - SPRITE_SIZE )
        return false;
    if ( head.x >= itemX + SPRITE_SIZE )
        return false;
    return true;
}

bool Snake::tailCollision() {
    for (int i = 0; i < tailN; ++i) {
        if (head.x == tail[i].x && head.y == tail[i].y)
            return true;
    }
    return false;
}

void Snake::reset() {
    printgameover = nullptr;
    printreplay = nullptr;
    head.x = SCREEN_CENTRE_X;
    head.y = SCREEN_CENTRE_Y;
    createItem();

    head.dir = start;
    tailN = 0;
    score = 0;
    updateScore();
}

void Snake::createItem() {
    int x = (rand() % (32 - 1) ) * SPRITE_SIZE + PADDING_SIDES;
    int y = (rand() % (16 - 1) ) * SPRITE_SIZE + PADDING_TOP;
    bool k = 1;
    for(int i = 0; i < tailN; ++i)
        if(x == tail[i].x && y == tail[i].y) {
            k = 0;
            break;
        }
    if(k == 0)
        createItem();
    if(k == 1) {
        itemX = x;
        itemY = y;
    }
}

void Snake::updateScore() {
    SDL_Color fontcolour = { 255, 255, 255, 0 };

    std::string scorestring = std::to_string(score);
    std::string scoremsg = "score: " + scorestring;
    printscore = renderText(scoremsg, "GreenFlame.ttf", fontcolour, 16, renderer);
}

void Snake::updateHighScore() {
    if (score >= highScore) {
        highScore = score;

        std::fstream ofile;
        ofile.open("highscores.txt");
        ofile << highScore;
        ofile.close();
    }

    SDL_Color fontcolour = { 255, 255, 255, 0 };

    std::string scorestring = std::to_string(highScore);
    std::string scoremsg = "highscore: " + scorestring;
    printhighscore = renderText(scoremsg, "GreenFlame.ttf", fontcolour, 16, renderer);
}

void Snake::getHighScore() {
    std::fstream ifile;
    ifile.open("highscores.txt");
    ifile >> highScore;
    ifile.close();
}

void Snake::renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr) {
    SDL_RenderCopy(ren, tex, clip, &dst);
}
void Snake::renderTexture1(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr) {
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    if (clip != nullptr){
        dst.w = clip->w;
        dst.h = clip->h;
    }
    else {
        SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
    }
    SDL_RenderCopy(ren, tex, clip, &dst);
}

SDL_Texture* Snake::renderText(const std::string &message, const std::string &fontFile, SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
    if (font == nullptr){
        std::cout << "TTF_OpenFont";
        return nullptr;
    }

    SDL_Surface *surf = TTF_RenderText_Solid(font, message.c_str(), color);
    if (surf == nullptr){
        TTF_CloseFont(font);
        std::cout << "TTF_RenderText";
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
    if (texture == nullptr){
        std::cout << "CreateTexture";
    }

    SDL_FreeSurface(surf);
    TTF_CloseFont(font);
    return texture;
}
