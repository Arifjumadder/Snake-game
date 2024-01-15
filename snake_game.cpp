#include<SDL2/SDL.h>
#include <iostream>
#include <vector>
using namespace std;
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int GRID_SIZE = 10;

SDL_Window* gWindow = nullptr;
SDL_Renderer* gRenderer = nullptr;
int score=0;
struct SnakeSegment {
    int x, y;
};

std::vector<SnakeSegment> snake;
int foodX, foodY;
int directionX, directionY;

void spawnFood() {
    foodX = rand() % (SCREEN_WIDTH / GRID_SIZE) * GRID_SIZE;
    foodY = rand() % (SCREEN_HEIGHT / GRID_SIZE) * GRID_SIZE;
}

void initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    gWindow = SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
    directionX = 1;
    directionY = 0;
    snake.push_back({SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2});
    spawnFood();
}

void close() {
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    SDL_Quit();
}

void handleInput() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            close();
            exit(0);
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_UP:
                    directionX = 0;
                    directionY = -1;
                    break;
                case SDLK_DOWN:
                    directionX = 0;
                    directionY = 1;
                    break;
                case SDLK_LEFT:
                    directionX = -1;
                    directionY = 0;
                    break;
                case SDLK_RIGHT:
                    directionX = 1;
                    directionY = 0;
                    break;
            }
        }
    }
}

void update() {
    int newHeadX = snake[0].x + directionX * GRID_SIZE;
    int newHeadY = snake[0].y + directionY * GRID_SIZE;

    
    if (newHeadX == foodX && newHeadY == foodY) {
        spawnFood();
        // Grow the snake
        snake.push_back({0, 0});
        score++;
        cout<<score<<endl;
    }
    else {
        
        for (int i = snake.size() - 1; i > 0; --i) { //move the snake
            snake[i] = snake[i - 1];
        }
    }

    
    snake[0].x = newHeadX;
    snake[0].y = newHeadY;

    
    if (newHeadX < 0 || newHeadX >= SCREEN_WIDTH || newHeadY < 0 || newHeadY >= SCREEN_HEIGHT) {
       cout<<"Game Over"<<endl;
        close();
        exit(0);
    }
    for (size_t i = 1; i < snake.size(); ++i) {
        if (newHeadX == snake[i].x && newHeadY == snake[i].y) {
            cout<<"Game Over"<<endl;
            close();
            exit(0);
        }
    }
}

void render() {
    SDL_SetRenderDrawColor(gRenderer,0, 0, 0, 255);
    SDL_RenderClear(gRenderer);

    
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 255, 255);
    for (const auto& segment : snake) {
        SDL_Rect rect = {segment.x, segment.y, GRID_SIZE, GRID_SIZE};
        SDL_RenderFillRect(gRenderer, &rect);
    }

   
    SDL_SetRenderDrawColor(gRenderer, 255, 255, 0, 255);
    SDL_Rect foodRect = {foodX, foodY, GRID_SIZE, GRID_SIZE};
    SDL_RenderFillRect(gRenderer, &foodRect);

    SDL_RenderPresent(gRenderer);
}
int main(int argc,char *argv[])
{
    initialize();

    while (true) {
        handleInput();
        update();
        render();
        SDL_Delay(100);
    }


}