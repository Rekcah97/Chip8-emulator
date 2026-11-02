#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>


class platform{
    public:
    platform(const char* title, int windowH , int windowW, int texH, int texW);
    ~platform();

    void update(const uint8_t* screen);
    bool processInput(bool* keys);



    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    int texW, texH;

};