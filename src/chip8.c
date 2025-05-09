#include <stdio.h>
#include <stdarg.h>
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

const int debug = 1;
const int step_through = 1;


void debuglog(const char *format, ...){
  if(debug){
    va_list args;

    va_start (args, format);
    vfprintf(stdout, format, args);
    va_end (args);
  }
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
  debuglog("Current instruction: %x\n", instruction);

  //Extracting Vx and Vy which may or may not be used
  unsigned short x = V[instruction & 0x0F00];
  unsigned short y = V[instruction & 0x00F0];

  //Extracting nibbles (values that instructions may use)
  unsigned short n = instruction & 0x000F;
  unsigned short nn = instruction & 0x00FF;
  unsigned short nnn = instruction & 0x0FFF;

  switch (instruction & 0xF000) { //Getting the nibble to determine the instruction type 
    case 0x0000:
      switch (instruction & 0x00FF){
        case 0x00E0: //Clear the display
          debuglog("clearing display\n");
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
    case 0x6000: //Set x register to NN
      debuglog("setting register\n");
      V[instruction & 0x0F00] = nn;
      break;
    case 0x7000: //Add NN to x register
      debuglog("adding to register\n");
      V[instruction & 0x0F00] += nn;
      break;
    case 0xA000: //Set I register to NNN
      debuglog("setting i register\n");
      I = nnn;
      break;
    case 0xD000: //Display sprite n-byte starting at memory location I at (Vx, Vy), if theres a collision, set VF to one
      V[0xF] = 0;
      draw_flag = 1;
      unsigned short count = I;
      unsigned char byte;
      debuglog("drawing sprite\n");
      debuglog("loops %d times\n", n);
      debuglog("Staring at (%d, %d)", x, y);
      for(int j = 0; j < n; j++){
        printf("drawing\n");
        byte = memory[count + j];
        for(int k = 0; k < 8; k++){
          if((byte >> (7-k) & 0x01) > 0){
            if (display[x+k][y+j] == 1){
              V[0xF] = 1;
            }

            display[x + k][y + j] ^= 1;
          }
        }
      }
      break;
      

  }





}
