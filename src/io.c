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

unsigned char keyboard[0xF] = {0}; //Range of 0x0 - 0xF : If index is = 1, that key is currently down. 

void init_display(){
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64*display_scale, 32*display_scale, SDL_WINDOW_SHOWN);
  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
}

void draw_display(unsigned char display[64][32]){
  if(draw_flag){
  for(int y = 0; y < 32; y++){
    for(int x = 0; x < 64; x++){
      if(display[x][y] == 1){
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_Rect pixel;
        pixel.h = display_scale;
        pixel.w = display_scale;
        pixel.x = x*display_scale;
        pixel.y = y*display_scale;

        SDL_RenderFillRect(renderer, &pixel);
        SDL_RenderDrawRect(renderer, &pixel);
      } else {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

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

//Maps keys
//1 2 3 C 4 5 6 D 7 8 9 E A 0 B F
//              ^
//              |
//              |
//1 2 3 4 Q W E R A S D F Z X C V              
void handle_key_press(SDL_KeyboardEvent e){
  switch(e.keysym.scancode){
    case SDL_SCANCODE_1:
      debuglog("Key 1 pressed\n");
      keyboard[0x1] = 1;
      break;
    case SDL_SCANCODE_2:
      debuglog("Key 2 pressed\n");
      keyboard[0x2] = 1;
      break;
    case SDL_SCANCODE_3:
      debuglog("Key 3 pressed\n");
      keyboard[0x3] = 1;
      break;
    case SDL_SCANCODE_4:
      debuglog("Key 4 pressed\n");
      keyboard[0xC] = 1;
      break;
    case SDL_SCANCODE_Q:
      debuglog("Key Q pressed\n");
      keyboard[0x4] = 1;
      break;
    case SDL_SCANCODE_W:
      debuglog("Key W pressed\n");
      keyboard[0x5] = 1;
      break;
    case SDL_SCANCODE_E:
      debuglog("Key E pressed\n");
      keyboard[0x6] = 1;
      break;
    case SDL_SCANCODE_R:
      debuglog("Key R pressed\n");
      keyboard[0xD] = 1;
      break;
    case SDL_SCANCODE_A:
      debuglog("Key A pressed\n");
      keyboard[0x7] = 1;
      break;
    case SDL_SCANCODE_S:
      debuglog("Key S pressed\n");
      keyboard[0x8] = 1;
      break;
    case SDL_SCANCODE_D:
      debuglog("Key D pressed\n");
      keyboard[0x9] = 1;
      break;
    case SDL_SCANCODE_F:
      debuglog("Key F pressed\n");
      keyboard[0xE] = 1;
      break;
    case SDL_SCANCODE_Z:
      debuglog("Key Z pressed\n");
      keyboard[0xA] = 1;
      break;
    case SDL_SCANCODE_X:
      debuglog("Key X pressed\n");
      keyboard[0x0] = 1;
      break;
    case SDL_SCANCODE_C:
      debuglog("Key C pressed\n");
      keyboard[0xB] = 1;
      break;
    case SDL_SCANCODE_V:
      debuglog("Key V pressed\n");
      keyboard[0xF] = 1;
      break;
  }
}

void handle_key_release(SDL_KeyboardEvent e){
  switch(e.keysym.scancode){
    case SDL_SCANCODE_1:
      keyboard[0x1] = 0;
      break;
    case SDL_SCANCODE_2:
      keyboard[0x2] = 0;
      break;
    case SDL_SCANCODE_3:
      keyboard[0x3] = 0;
      break;
    case SDL_SCANCODE_4:
      keyboard[0xC] = 0;
      break;
    case SDL_SCANCODE_Q:
      keyboard[0x4] = 0;
      break;
    case SDL_SCANCODE_W:
      keyboard[0x5] = 0;
      break;
    case SDL_SCANCODE_E:
      keyboard[0x6] = 0;
      break;
    case SDL_SCANCODE_R:
      keyboard[0xD] = 0;
      break;
    case SDL_SCANCODE_A:
      keyboard[0x7] = 0;
      break;
    case SDL_SCANCODE_S:
      keyboard[0x8] = 0;
      break;
    case SDL_SCANCODE_D:
      keyboard[0x9] = 0;
      break;
    case SDL_SCANCODE_F:
      keyboard[0xE] = 0;
      break;
    case SDL_SCANCODE_Z:
      keyboard[0xA] = 0;
      break;
    case SDL_SCANCODE_X:
      keyboard[0x0] = 0;
      break;
    case SDL_SCANCODE_C:
      keyboard[0xB] = 0;
      break;
    case SDL_SCANCODE_V:
      keyboard[0xF] = 0;
      break;
  }
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
        handle_key_release(event.key);
        break;
    }
  }  
}




