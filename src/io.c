#include <stdio.h>
#include <SDL.h>
#include <stdbool.h>

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
      printf("placeholder");
    }
  }
}



int main(){
  SDL_Window* window = NULL;
  SDL_Surface* screenSurface = NULL;

  if( SDL_Init( SDL_INIT_VIDEO ) < 0){
    printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
  } else {
    //Create window
    window = SDL_CreateWindow( "CHIP-8", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 64*8, 32*8, SDL_WINDOW_SHOWN );
    if( window == NULL )
    {
      printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
    } else {
      screenSurface = SDL_GetWindowSurface(window);

      SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));

      SDL_UpdateWindowSurface(window);
      SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_QUIT ) quit = true; } }
    }
  }
      //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_Quit();

    return 0;
}

