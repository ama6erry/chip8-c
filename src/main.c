#include "chip8.h"
#include "io.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{

  int opt;
  int file_flag = 0;
  int result;
  char file_path[512];

  while((opt = getopt(argc, argv, "df:s:")) != -1) {
    switch (opt){
      case 'd':
        printf("Debug mode on \n");
        debug = 1;
        break;
      case 'f':
        file_flag = 1;
        strcpy(file_path, optarg);
        break;
      case 's':
        display_scale = atoi(optarg);
        break;
    }
  }

  if(file_flag){
    result = load_rom(file_path);
  } else {
    printf("Please enter the file path to the rom: \n");
    scanf("%s", file_path);
    result = load_rom(file_path);
  }

  if(result == -1){
    printf("File not found\n");
    return 0;
  }

  init_display();
  if(debug){
    logfile = fopen("log.txt", "w");
  }

  memcpy(memory, fontset, sizeof(fontset));

  while(running){
    if(dt > 0){
      --dt;
    }
    if(st > 0){
      --st;
    }
    run_cycle();
    draw_display(display);
    event_handler();
    usleep(1500);
  }

  if(debug){
    fclose(logfile);
  }
  stop_display();
}
