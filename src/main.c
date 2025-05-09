#include "chip8.h"
#include "io.h"
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  if(argc == 1){
    printf("Invalid parameters, have rom file name as parameter \n");
    return 0;
  }
  if(load_rom(argv[1]) == -1){
    printf("File not found\n");
    return 0;
  }

  init_display();

  while(running){
    run_cycle();
    draw_display(display);
    event_handler();
    usleep(1500);
  }

  stop_display();
}
