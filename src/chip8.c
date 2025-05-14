#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include "io.h"

/*
 +---------------+= 0xFFF (4095) End of Chip-8 RAM
|               |
|               |
|               |
|               |
|               |
| 0x200 to 0xFFF|
|     Chip-8    |
| Program / Data|
|     Space     |
|               |
|               |
|               |
+- - - - - - - -+= 0x600 (1536) Start of ETI 660 Chip-8 programs
|               |
|               |
|               |
+---------------+= 0x200 (512) Start of most Chip-8 programs
| 0x000 to 0x1FF|
| Reserved for  |
|  interpreter  |
+---------------+= 0x000 (0) Start of Chip-8 RAM
 */


//Declaring registers
unsigned char V[0xF] = {0}; //Vx - The 16 standard registers that programs can use, however VF (Register 16) is reserved as a flag used by some instructions
unsigned short I = 0; //I register - Used to store memory addresses
unsigned char dt = 0; //Delay timer - When not 0, decrement at a rate of 60Hz
unsigned char st = 0; //Sound timer - When not 0, decrement at a rate of 60Hz

//Declaring pseudo registers
unsigned short pc = 0x200; //Program counter - Stores the address of the next instruction to be executed
unsigned char sp = 0; //Stack pointer - points to the top of the stack

//Other
unsigned short stack[16] = {0};
unsigned char memory[4096] = {0};
unsigned char display[64][32] = {0};

// Font set
unsigned char fontset[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

int debug = 0;
int step_through = 1;
FILE* logfile;

void debuglog(const char* format, ...){
  if(debug){
    va_list args;

    va_start (args, format);
    if(logfile != NULL){
      vfprintf(logfile, format, args);
    }
    va_end (args);

    va_start (args, format);
    vfprintf(stdout, format, args);
    va_end (args);
  }

}

int random_num(int min, int max){
  int randnum = rand() % (max - min + 1) + min;
  return randnum;
}

int load_rom(char* name){
  FILE* file;
  file = fopen(name, "rb");
  
  if(file == NULL){
    return -1;
  }

  int size = fread(memory + 0x200, 1, sizeof(memory) - 0x200, file);
  
  debuglog("file size %d\n", size);
  
  fclose(file);

  if(size == sizeof(memory - 0x200)){
    return 0;
    debuglog("file %s loaded, size = %d\n", name, size);
  } else {
    return 1;
    debuglog("file %s loaded though appears to be incorrect size? size = %d\n", name, size);
  }
}

void run_cycle(){
  //Each instruction is 0xyn, where x and y are values for Vx and Vy
  unsigned short instruction = memory[pc] << 8 | memory[pc + 1];
  pc += 2;
  debuglog("Current instruction: 0x%x\n", instruction);

  //Extracting Vx and Vy which may or may not be used
  unsigned short x = V[(instruction & 0x0F00) >> 8];
  unsigned short y = V[(instruction & 0x00F0) >> 4];
  debuglog("register %d value: %d \n", (instruction & 0x0F00) >> 8, x);
  debuglog("register %d value: %d \n", (instruction & 0x00F0) >> 4, y);
  
  
  //Extracting nibbles (values that instructions may use)
  unsigned short n = instruction & 0x000F;
  unsigned short nn = instruction & 0x00FF;
  unsigned short nnn = instruction & 0x0FFF;

  switch (instruction & 0xF000) { //Getting the nibble to determine the instruction type 
    case 0x0000:
      switch (instruction & 0x00FF){
        case 0x00E0: //Clear the display
          debuglog("clearing display\n");
          draw_flag = 1;
          for(int i = 0; i < 64; i++){
            for(int s = 0; s < 32; s++){
              display[i][s] = 0;
            }
          }
          break;
        case 0x00EE: //Return from subroutine
          debuglog("returning from subroutine\n");
          pc = stack[sp];
          sp -= 1;
          break;
      }
      break;
    case 0x1000: //Jump to address NNN
      debuglog("jumping address\n");
      pc = nnn;
      break;
    case 0x2000:
      debuglog("calling subroutine\n");
      sp += 1;
      stack[sp] = pc;
      pc = nnn;
      break;
    case 0x3000:
      if (x == nn){
        pc += 2;
      }
      break;
    case 0x4000:
      if (x != nn){
        pc += 2;
      }
      break;
    case 0x5000:
      if (x == y){
        pc += 2;
      }
      break;
    case 0x6000: //Set x register to NN
      debuglog("setting register %d to %d\n", (instruction & 0x0F00) >> 8, nn);
      V[(instruction & 0x0F00) >> 8] = nn;
      break;
    case 0x7000: //Add NN to x register
      debuglog("adding %d register %d\n", nn, (instruction & 0x0F00) >> 8);
      V[(instruction & 0x0F00) >> 8] += nn;
      break;
    case 0x8000:
      switch (instruction & 0x000F) {
        case 0x0000:
          V[(instruction & 0x0F00) >> 8] = y;
          break;
        case 0x0001:
          V[(instruction & 0x0F00) >> 8] = x | y;
          break;
        case 0x0002:
          V[(instruction & 0x0F00) >> 8] = x & y;
          break;
        case 0x0003:
          V[(instruction & 0x0F00) >> 8] = x ^ y;
          break;
        case 0x0004:
          V[(instruction & 0x0F00) >> 8] += y;
          if ((x + y) > 0xFF){
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          break;
        case 0x0005:
          V[(instruction & 0x0F00) >> 8] -= y;
          debuglog("setting register %d to %d - %d\n", (instruction & 0x0F00) >> 8, x, y);
          if (x >= y){
            debuglog("set flag to 1\n");
            V[0xF] = 1;
          } else {
            debuglog("set flag to 0\n");
            V[0xF] = 0;
          }
          break;
        case 0x0006:
          V[(instruction & 0x0F00) >> 8] >>= 1;
          if ((x & 0x01) == 1){
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          break;
        case 0x0007:
          V[(instruction & 0x0F00) >> 8] = y - x;
          if (y > V[(instruction & 0x0F00) >> 8]){
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          break;
        case 0x000E:
          V[(instruction & 0x0F00) >> 8] <<= 1;
          if ((x & 0x80) > 1){
            V[0xF] = 1;
          } else {
            V[0xF] = 0;
          }
          break;
      }
      break;
    case 0x9000:
      if(x != y){
        pc += 2;
      }
      break;
    case 0xA000: //Set I register to NNN
      debuglog("setting i register to %d\n", nnn);
      I = nnn;
      break;
    case 0xB000:
      pc = nnn + V[0];
      break;
    case 0xC000:
      V[(instruction & 0x0F00) >> 8] = random_num(0, 255) & nn;
      break;
    case 0xD000: //Display sprite n-byte starting at memory location I at (Vx, Vy), if theres a collision, set VF to one
      V[0xF] = 0;
      draw_flag = 1;
      unsigned short count = I;
      unsigned char byte;
      debuglog("drawing sprite\n");
      debuglog("loops %d times\n", n);
      debuglog("Staring at (%d, %d)\n", x%64, y%32);
      for(int j = 0; j < n; j++){
        byte = memory[count + j];
        for(int k = 0; k < 8; k++){
          if((byte >> (7-k) & 0x01) > 0){
           if (display[(x+k)%64][(y+j)%32] == 1){
              V[0xF] = 1;
            }
            display[(x + k)%64][(y + j)%32] ^= 1;
          }
        }
      }
      break;
    case 0xE000:
      switch(instruction & 0x00FF){
        case 0x009E:
          if(keyboard == x){
            pc += 2;
          }
          break;
        case 0x00A1:
          if(keyboard != x){
            pc += 2;
          }
          break;
      }
      break;
    case 0xF000:
      switch(instruction & 0x00FF){
        case 0x0007:
          V[(instruction & 0x0F00) >> 8] = dt;
          break;
        case 0x000A:
          do {
            if(dt > 0){
              --dt;
            }
            if(st > 0){
              --st;
            }
            event_handler();
            usleep(1500);
          } while (keyboard == 0x1F);
          V[(instruction & 0x0F00) >> 8] = keyboard;
          break;
        case 0x0015:
          dt = x;
          break;
        case 0x0018:
          st = x;
          break;
        case 0x001E:
          I += x;
          debuglog("Adding %d to I register\n", x);
          break;
        case 0x0029:
          I = x * 5;
          debuglog("setting I to store font %x\n", x);
          break;
        case 0x0033:
          debuglog("determing digits of %d\n", x);
          for(int i = 3; i > 0; --i){
            debuglog("digit %d extracted\n", x%10);
            memory[I + i - 1] = x%10;
            x /= 10;
          }
          break;
        case 0x0055:
          debuglog("writing V0 to V%d to memory : starting from memory[%d]\n", (instruction & 0x0F00) >> 8, I);
          for(int i = 0; i <= ((instruction & 0x0F00) >> 8) ; i++){
            debuglog("storing contents of register %d at memory location %d\n", i, I+i);
            memory[I + i] = V[i];
          }
          break;
        case 0x0065:
          debuglog("writing from memory to V0 to V%d : starting from memory[%d]\n", (instruction & 0x0F00) >> 8, I);
          for(int i = 0; i <= ((instruction & 0x0F00) >> 8); i++){
            debuglog("storing contents of memory location %d into register %d\n", I+i, i);
            V[i] = memory[I + i];
          }
          break;
      }
      break;
    default:
      printf("Instruction 0x%x not found\n", instruction);
      break;
  }





}
