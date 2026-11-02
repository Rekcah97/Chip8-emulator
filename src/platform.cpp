#include "platform.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_error.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <stdexcept>
#include <vector>
using namespace std;

platform::platform(const char* title, int windowH , int windowW, int texH, int texW) 
    : texW(texW), texH(texH){
    


    if(SDL_Init(SDL_INIT_VIDEO) != 0){
        throw runtime_error(SDL_GetError());
    }

    window = SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,windowW,windowH,SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texW, texH);

    if(!window || !renderer || !texture){
        throw runtime_error(SDL_GetError());
    }
}

platform::~platform(){
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void platform::update(const uint8_t* screen){

    static vector<uint32_t> pixels(texW * texH);
    for (int i = 0; i < texW * texH; i++)
        pixels[i] = screen[i] ? 0xFFFFFFFF : 0x000000FF;   // RGBA: white / black

    SDL_UpdateTexture(texture, nullptr, pixels.data(), texW * sizeof(uint32_t));
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);   // stretches to window size
    SDL_RenderPresent(renderer);
}

bool platform::processInput(bool* keys) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) return false;
        if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) return false;
    }

    const Uint8* k = SDL_GetKeyboardState(nullptr);
    // CHIP-8 key -> keyboard (1234/QWER/ASDF/ZXCV layout)
    keys[0x1] = k[SDL_SCANCODE_1]; keys[0x2] = k[SDL_SCANCODE_2];
    keys[0x3] = k[SDL_SCANCODE_3]; keys[0xC] = k[SDL_SCANCODE_4];
    keys[0x4] = k[SDL_SCANCODE_Q]; keys[0x5] = k[SDL_SCANCODE_W];
    keys[0x6] = k[SDL_SCANCODE_E]; keys[0xD] = k[SDL_SCANCODE_R];
    keys[0x7] = k[SDL_SCANCODE_A]; keys[0x8] = k[SDL_SCANCODE_S];
    keys[0x9] = k[SDL_SCANCODE_D]; keys[0xE] = k[SDL_SCANCODE_F];
    keys[0xA] = k[SDL_SCANCODE_Z]; keys[0x0] = k[SDL_SCANCODE_X];
    keys[0xB] = k[SDL_SCANCODE_C]; keys[0xF] = k[SDL_SCANCODE_V];
    return true;
}