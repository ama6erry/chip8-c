#include <stdio.h>

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
unsigned char V[16] = {0}; //Vx - The 16 standard registers that programs can use, however VF (Register 16) is reserved as a flag used by some instructions
unsigned short I = 0; //I register - Used to store memory addresses
unsigned char dt = 0; //Delay timer - When not 0, decrement at a rate of 60Hz
unsigned char st = 0; //Sound timer - When not 0, decrement at a rate of 60Hz

//Declaring pseudo registers
unsigned short pc = 0x200; //Program counter - Stores the address of the next instruction to be executed
unsigned char sp = 0; //Stack pointer - points to the top of the stack

//Other
unsigned short stack[16] = {0};
unsigned char memory[4096] = {0};
unsigned char display[64*32] = {0};

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

int load_rom(char* name){
  FILE* file;
  file = fopen("name", "rb");

  if(file == NULL){
    return -1;
  }

  fread(memory, 1, sizeof(memory) - 0x200, file);
}

void run_rom(){
  //Load the rom
  int load = load_rom("")
}

