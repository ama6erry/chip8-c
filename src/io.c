#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include "chip8.h"

SDL_Windows* window;
SDL_Surface* screenSurface;
SDL_Renderer* renderer;

void init_display(){
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64*8, 32*8, SDL_WINDOSHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void draw_display(unsigned char* display){
  SetRenderDrawColor(renderer, 255, 255, 255, 255);

  for(int x = 0; x < 64; x++){
    for(int y =0; y < 32; y++){
      if(display[x*32 + y] == 1){
        SDL_Rect pixel;
        pixel.h = 8;
        pixel.w = 8;
        pixel.x = x*8;
        pixel.y = y*8;

        SDL_RenderDrawRect(renderer, &pixel);
      }
    }
  }

  SDL_RenderPresent();
}


