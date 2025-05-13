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

unsigned char keyboard = 0x1F; //Range of 0x00 - 0x1F : A value of 0x1F indicates no key is being pressed

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

  while(SDL_PollEvent(&event)){
    switch(event.type){
      case SDL_QUIT:
        running = 0;
        break;
      case SDL_KEYDOWN:
        handle_key_press(event.key);
        break;
      case SDL_KEYUP:
        keyboard = 0x1F;
        break;
    }
  }
  
  if (SDL_PollEvent(&event)){
    if(event.type == SDL_QUIT){
      running = 0;
    }
  }
}

//Maps keys
//1 2 3 C 4 5 6 D 7 8 9 E A 0 B F
//              ^
//              |
//              |
//1 2 3 4 Q W E R A S D F Z X C V              
void handle_key_press(SDL_KeyboardEvent e){
  switch(e.keysym.scancode){
    case SDL_SCANCODE_1:
      keyboard = 0x01;
      break;
    case SDL_SCANCODE_2:
      keyboard = 0x02;
      break;
    case SDL_SCANCODE_3:
      keyboard = 0x03;
      break;
    case SDL_SCANCODE_4:
      keyboard = 0x0C;
      break;
    case SDL_SCANCODE_Q:
      keyboard = 0x04;
      break;
    case SDL_SCANCODE_W:
      keyboard = 0x05;
      break;
    case SDL_SCANCODE_E:
      keyboard = 0x06;
      break;
    case SDL_SCANCODE_R:
      keyboard = 0x0D;
      break;
    case SDL_SCANCODE_A:
      keyboard = 0x07;
      break;
    case SDL_SCANCODE_S:
      keyboard = 0x08;
      break;
    case SDL_SCANCODE_D:
      keyboard = 0x09;
      break;
    case SDL_SCANCODE_F:
      keyboard = 0x0E;
      break;
    case SDL_SCANCODE_Z:
      keyboard = 0x0A;
      break;
    case SDL_SCANCODE_X:
      keyboard = 0x00;
      break;
    case SDL_SCANCODE_C:
      keyboard = 0x0B;
      break;
    case SDL_SCANCODE_V:
      keyboard = 0x0F;
      break;
  }
}



