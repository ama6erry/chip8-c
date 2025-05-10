#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>
#include "chip8.h"

SDL_Window* window;
SDL_Surface* screenSurface;
SDL_Renderer* renderer;

int running = 1;
int draw_flag = 0;
int display_scale = 8;

void init_display(){
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64*display_scale, 32*display_scale, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void draw_display(unsigned char display[64][32]){
  if(draw_flag){
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    for(int y = 0; y < 32; y++){
      for(int x = 0; x < 64; x++){
        if(display[x][y] == 1){
          SDL_Rect pixel;
          pixel.h = display_scale;
          pixel.w = display_scale;
          pixel.x = x*display_scale;
          pixel.y = y*display_scale;

          SDL_RenderFillRect(renderer, &pixel);
          SDL_RenderDrawRect(renderer, &pixel);
        }
      }
    }

    SDL_RenderPresent(renderer);
  }
  draw_flag = 0;
}

void stop_display(){
  SDL_DestroyWindow(window);
  SDL_Quit();
}

void event_handler(){
  SDL_Event event;
  
  if (SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT){
      running = 0;
    }
  }
}



